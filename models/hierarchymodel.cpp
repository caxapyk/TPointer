#include "hierarchymodel.h"
#include "models/corpusmodel.h"
#include "models/storagemodel.h"

#include <QDebug>
#include <QSqlQuery>
#include <QSqlQueryModel>

HierarchyModel::HierarchyModel()
{

}

HierarchyModel::~HierarchyModel()
{

}

void HierarchyModel::select()
{
    setupModelData();
}

void HierarchyModel::clear()
{
    //recursive remove nodes
}

void HierarchyModel::setupModelData(const QModelIndex &index)
{
    HierarchyNode *parentNode = (index.isValid()) ? static_cast<HierarchyNode*>(index.internalPointer()) : &root;
    int level = (index.isValid()) ? parentNode->level + 1 : HierarchyModel::CorpusLevel;

    switch (level) {
    case (HierarchyModel::CorpusLevel):
    {
        CorpusModel model;
        model.select();

        for (int i=0; i < model.rowCount(); ++i) {
            HierarchyNode *node = new HierarchyNode();

            node->id = model.data(model.index(i, 0));
            node->name = model.data(model.index(i, 1));

            node->level = level;
            node->row = i;

            node->mapped = false;

            root.children.append(node);
        }
        break;
    }
    case (HierarchyModel::StorageLevel):
    {
        StorageModel model;
        model.setParentId(1, parentNode->id);
        model.select();

        for (int i=0; i < model.rowCount(); ++i) {
            HierarchyNode *node = new HierarchyNode();

            node->id = model.data(model.index(i, 0));
            node->name = model.data(model.index(i, 3));

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

        QVariant storage_id = parentNode->parent->id;

        //fix storage=1
        model.setQuery("SELECT DISTINCT compartment FROM tpointer WHERE storage=1 ORDER BY compartment");

        for (int i=0; i < model.rowCount(); ++i) {
            HierarchyNode *node = new HierarchyNode();

            node->name = model.data(model.index(i, 0)).toString();

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

        //fix storage=1
        model.setQuery("SELECT DISTINCT shelving FROM tpointer WHERE storage=1 AND compartment="
                       + compartment.toString()
                       + " ORDER BY shelving");

        for (int i=0; i < model.rowCount(); ++i) {
            HierarchyNode *node = new HierarchyNode();

            node->name = model.data(model.index(i, 0)).toString();

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
            return !root.children.isEmpty();
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
        // fix storage=1
        query_str = "SELECT COUNT(DISTINCT compartment) FROM tpointer WHERE storage=1";
        break;
    }
    case (HierarchyModel::CompartmentLevel):
    {
        QVariant storage_id = parentNode->parent->parent->id;
        QVariant compartment = parentNode->name;

        // fix storage=1
        query_str = "SELECT COUNT(DISTINCT shelving) FROM tpointer WHERE storage=1 AND compartment=" + compartment.toString();
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

   if (query.value(0).toInt() > 0) {
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
    //const HierarchyNode *parentNode = static_cast<const HierarchyNode*>(parent.internalPointer());

    setupModelData(parent);
}

QModelIndex HierarchyModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    if (!parent.isValid()) {
        return createIndex(row, column, const_cast<HierarchyNode*>(root.children[row]));
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
        return root.children.size();
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
    if (role == Qt::DisplayRole) {
        const HierarchyNode* currentNode = static_cast<HierarchyNode*>(index.internalPointer());

        switch (index.column()) {
        case NameColumn:
        {
            if (currentNode->level == HierarchyModel::CompartmentLevel) {
                return QVariant(tr("Compartment ") + currentNode->name.toString());
            } else if (currentNode->level == HierarchyModel::ShelvingLevel) {
                return QVariant(tr("Shelving ") + currentNode->name.toString());
            } else {
                return currentNode->name;
            }
            break;
        }
        case FloorsColumn:
        {
            return QVariant();
            break;
        }
        }
    }

    return QVariant();
}

bool HierarchyModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
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
            qDebug() << "sdfdsfsdf";
        return QVariant();
    }

    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        return columnHeaders.at(section);
     }

    return QVariant();
}



