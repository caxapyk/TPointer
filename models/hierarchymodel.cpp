#include "hierarchymodel.h"
#include "models/corpusmodel.h"
#include "models/storagemodel.h"

#include <QColor>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QRegularExpression>

HierarchyModel::HierarchyModel()
{
    root = new HierarchyRootNode;

    setHeaderData(0, Qt::Horizontal, tr("Place path"));
    setHeaderData(1, Qt::Horizontal, tr("Floors"));
}

HierarchyModel::~HierarchyModel()
{
    clear();
    delete root;
}

void HierarchyModel::select()
{
    clear();
    setupModelData();
}

void HierarchyModel::clear()
{
    beginResetModel();
    for(int i=0; i<root->children.length();++i) {
        recursivelyRemoveNodes(root->children.at(i));
    }
    root->children.clear();
    endResetModel();
}

/* Removes all nodes objects from memory recursively */
void HierarchyModel::recursivelyRemoveNodes(HierarchyNode *node)
{
    for(int i=0; i<node->children.length();++i) {
        recursivelyRemoveNodes(node->children.at(i));
    }

    delete node;
}

void HierarchyModel::setupModelData(const QModelIndex &index)
{
    HierarchyNode *parentNode = (index.isValid()) ? static_cast<HierarchyNode*>(index.internalPointer()) : root;
    int level = (index.isValid()) ? parentNode->level + 1 : HierarchyModel::CorpusLevel;

    switch (level) {
    case (HierarchyModel::CorpusLevel):
    {
        CorpusModel model;
        model.select();

        for (int i=0; i < model.rowCount(); ++i) {
            HierarchyNode *node = new HierarchyNode();

            node->id = model.record(i).value(0);
            node->name = model.record(i).value(1);

            node->level = level;
            node->row = i;

            node->mapped = false;

            root->children.append(node);
        }
        break;
    }
    case (HierarchyModel::StorageLevel):
    {
        StorageModel model;
        model.setFilter("corpus_id=" + parentNode->id.toString());
        model.setSort(2, Qt::AscendingOrder);
        model.select();

        for (int i=0; i < model.rowCount(); ++i) {
            HierarchyNode *node = new HierarchyNode();

            node->id = model.record(i).value(0);
            node->name = model.record(i).value(3);

            /* replace 0 floor with 'basement', -n floor with '-n floor' */
            QStringList floors = model.record(i).value(5).toString().split(",");
            floors = floors.replaceInStrings(QRegExp("^[0]+$"), tr("bsmnt"));
            floors = floors.replaceInStrings(QRegExp("^(-\\d)+$"), tr("\\1 fl."));

            node->floor = QVariant(floors.join(", "));

            node->level = level;
            node->row = i;

            node->parent = parentNode;

            parentNode->mapped = true;
            parentNode->children.append(node);
        }
        break;
    }
    case (HierarchyModel::CompartmentLevel):
    {
        QSqlQueryModel model;

        model.setQuery("SELECT DISTINCT compartment FROM tpointer WHERE storage=" + parentNode->id.toString()
                       + " ORDER BY compartment");

        for (int i=0; i < model.rowCount(); ++i) {
            HierarchyNode *node = new HierarchyNode();

            node->name = (!model.record(i).value(0).isNull()) ? model.record(i).value(0) : QVariant(); // check compartment is null

            node->level = level;
            node->row = i;

            node->parent = parentNode;

            parentNode->mapped = true;
            parentNode->children.append(node);
        }
        break;
    }
    case (HierarchyModel::ShelvingLevel):
    {
        QSqlQueryModel model;

        QVariant storage_id = parentNode->parent->id;
        QVariant compartment = parentNode->name;

        model.setQuery("SELECT DISTINCT shelving FROM tpointer WHERE storage=" + storage_id.toString()
                       + ((!compartment.isNull()) ? " AND compartment='" + compartment.toString() + "'" : " AND compartment IS NULL")
                       + " ORDER BY shelving");

        for (int i=0; i < model.rowCount(); ++i) {
            HierarchyNode *node = new HierarchyNode();

            node->name = model.record(i).value(0);

            node->level = level;
            node->row = i;

            node->parent = parentNode;

            parentNode->mapped = true;
            parentNode->children.append(node);
        }
        break;
    }
    }
}

bool HierarchyModel::hasChildren(const QModelIndex &parent) const
{
    if (!parent.isValid()) {
            return !root->children.isEmpty();
    }

    HierarchyNode *parentNode = static_cast<HierarchyNode*>(parent.internalPointer());

    QString query_str;

    switch (parentNode->level) {
        case (HierarchyModel::CorpusLevel):
        {
            query_str = "SELECT COUNT(id) FROM storage WHERE corpus_id=" + parentNode->id.toString();
            break;
        }
        case (HierarchyModel::StorageLevel):
        {
            query_str = "SELECT COUNT(DISTINCT compartment) FROM tpointer WHERE storage=" + parentNode->id.toString();
            break;
        }
        case (HierarchyModel::CompartmentLevel):
        {
            query_str = "SELECT COUNT(DISTINCT shelving) FROM tpointer WHERE storage=" + parentNode->parent->id.toString();
            if (!parentNode->name.isNull()) { //NULL compartment
                query_str += " AND compartment='" + parentNode->name.toString() + "'";
            } else {
                   query_str += " AND compartment IS NULL";
            }
            break;
        }
        case (HierarchyModel::ShelvingLevel):
        {
            return false;
            break;
        }
    }

    QSqlQuery query(query_str);
    query.exec();
    query.first();

    if (query.isValid() && query.value(0).toInt() > 0) {
        return true;
    }

    return false;
}

bool HierarchyModel::canFetchMore(const QModelIndex &parent) const
{
    if (!parent.isValid()) {
        return false;
    }

    const HierarchyNode *parentNode = static_cast<const HierarchyNode*>(parent.internalPointer());

    return !parentNode->mapped;
}

void HierarchyModel::fetchMore(const QModelIndex &parent)
{
    setupModelData(parent);
}

QModelIndex HierarchyModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    if (!parent.isValid()) {
        return createIndex(row, column, const_cast<HierarchyNode*>(root->children[row]));
    }

    HierarchyNode *parentNode = static_cast<HierarchyNode*>(parent.internalPointer());

    return createIndex(row, column, parentNode->children[row]);
}

QModelIndex HierarchyModel::parent(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return QModelIndex();
    }

    HierarchyNode *childNode = static_cast<HierarchyNode*>(index.internalPointer());
    HierarchyNode *parentNode = childNode->parent;

    if (parentNode != nullptr) {
        return createIndex(parentNode->row, 0, parentNode);
    }

    return QModelIndex();
}

int HierarchyModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid()) {
        return root->children.size();
    }

    const HierarchyNode* parentNode = static_cast<const HierarchyNode*>(parent.internalPointer());
    return parentNode->children.size();
}

int HierarchyModel::columnCount(const QModelIndex &) const
{
    return ColumnCount;
}

QVariant HierarchyModel::data(const QModelIndex &index, int role) const
{
    if (index.column() == HierarchyModel::NameColumn && role == Qt::DisplayRole) {
        const HierarchyNode* currentNode = static_cast<HierarchyNode*>(index.internalPointer());

        if (currentNode->level == HierarchyModel::StorageLevel) {
            return QVariant(tr("Storage №") + currentNode->name.toString());
        } else if (currentNode->level == HierarchyModel::CompartmentLevel) {
            if (currentNode->name.toInt() != 0) {
                return QVariant(tr("Compartment №") + currentNode->name.toString());
            } else {
                return QVariant(tr("Undefined compartment"));
            }
        } else if (currentNode->level == HierarchyModel::ShelvingLevel) {
            if (currentNode->name.toInt() != 0) {
                return QVariant(tr("Shelving №") + currentNode->name.toString());
            } else {
                return currentNode->name;
            }
        } else {
            return currentNode->name;
        }
    } else if (index.column() == HierarchyModel::FloorsColumn && role == Qt::DisplayRole) {
        const HierarchyNode* currentNode = static_cast<HierarchyNode*>(index.internalPointer());

        if (currentNode->level == HierarchyModel::StorageLevel) {
            return currentNode->floor;
        }
    } else if (index.column() == HierarchyModel::FloorsColumn && role == Qt::ForegroundRole) {
        return QVariant( QColor( Qt::gray ) );
    }

    return QVariant();
}

bool HierarchyModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int)
{
    if ((section < 0)
            || ((orientation == Qt::Horizontal) && (section >= columnCount()))
            || ((orientation == Qt::Vertical) && (section >= rowCount()))) {
            return false;
    }

    if (orientation == Qt::Horizontal) {
        columnHeaders.append(value);
        emit headerDataChanged(orientation, section, section);
        return true;
     }

     return false;
}

QVariant HierarchyModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if ((section < 0)
            || ((orientation == Qt::Horizontal) && (section >= columnCount()))
            || ((orientation == Qt::Vertical) && (section >= rowCount()))) {
        return QVariant();
    }

    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        return columnHeaders.at(section);
     }

    return QVariant();
}



