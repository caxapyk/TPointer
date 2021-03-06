#include "hierarchymodel.h"
#include "models/corpusmodel.h"
#include "models/storagemodel.h"
#include "utils/floorsparser.h"

#include <QColor>
#include <QDebug>
#include <QIcon>
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
        model.setFilter("corpus=" + parentNode->id.toString());
        model.setSort(2, Qt::AscendingOrder);
        model.select();

        for (int i=0; i < model.rowCount(); ++i) {
            HierarchyNode *node = new HierarchyNode();

            node->id = model.record(i).value(0);
            node->name = model.record(i).value(3);

            FloorsParser parser;
            QStringList floors = parser.process(model.record(i).value(5).toString());

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
                       + " AND compartment=" + compartment.toString()
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
            query_str = "SELECT COUNT(id) FROM storage WHERE corpus=" + parentNode->id.toString();
            break;
        }
        case (HierarchyModel::StorageLevel):
        {
            query_str = "SELECT COUNT(DISTINCT compartment) FROM tpointer WHERE storage=" + parentNode->id.toString();
            break;
        }
        case (HierarchyModel::CompartmentLevel):
        {
            query_str = "SELECT COUNT(DISTINCT shelving) FROM tpointer WHERE storage=" + parentNode->parent->id.toString()
                    + " AND compartment=" + parentNode->name.toString();

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
    if (query.size() > 0 && query.value(0).toInt() > 0) {
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
    switch (role) {
    case Qt::DisplayRole:
        if (index.column() == HierarchyModel::NameColumn) {
            const HierarchyNode* currentNode = static_cast<HierarchyNode*>(index.internalPointer());

            if (currentNode->level == HierarchyModel::StorageLevel) {
                return QVariant(tr("Storage №") + currentNode->name.toString());
            } else if (currentNode->level == HierarchyModel::CompartmentLevel) {
                return QVariant(tr("Compartment №") + currentNode->name.toString());
            } else if (currentNode->level == HierarchyModel::ShelvingLevel) {
                return QVariant(tr("Shelving №") + currentNode->name.toString());
            } else {
                return currentNode->name;
            }
        } else if (index.column() == HierarchyModel::FloorsColumn) {
            const HierarchyNode* currentNode = static_cast<HierarchyNode*>(index.internalPointer());

            if (currentNode->level == HierarchyModel::StorageLevel) {
                return currentNode->floor;
            }
        }
        break;
    case Qt::DecorationRole:
        if (index.column() == HierarchyModel::NameColumn) {
            const HierarchyNode* currentNode = static_cast<HierarchyNode*>(index.internalPointer());

            if (currentNode->level == HierarchyModel::CorpusLevel
                    || currentNode->level == HierarchyModel::StorageLevel
                    || currentNode->level == HierarchyModel::CompartmentLevel) {
                return QIcon(":/icons/folder-16.png");
            } else if (currentNode->level == HierarchyModel::ShelvingLevel) {
                return QIcon(":/icons/shelving-16.png");
            }
        }
        break;
    case Qt::ForegroundRole:
        if (index.column() == HierarchyModel::FloorsColumn) {
            return QVariant( QColor( Qt::gray ) );
        }
        break;
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
        columnHeaders.insert(section, value);
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
        return columnHeaders.value(section);
     }

    return QVariant();
}



