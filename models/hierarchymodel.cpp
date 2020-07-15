#include "hierarchymodel.h"

#include <QDebug>
#include <QSqlQueryModel>

HierarchyModel::HierarchyModel()
{

}

HierarchyModel::~HierarchyModel()
{
    qDeleteAll(nodes);
    // сделай рекурсивно!!
    qDeleteAll(all_nodes);
}

void HierarchyModel::setupModelData()
{
    if (!nodes.isEmpty()) {
        nodes.clear();
    }

    QSqlQueryModel corpus_model;
    QSqlQueryModel storage_model;

    corpus_model.setQuery("SELECT * FROM corpus");

    for (int i=0; i < corpus_model.rowCount(); ++i) {
        HierarchyNode *corpus_node = new HierarchyNode();
        QVariant id = corpus_model.data(corpus_model.index(i, 0));

        corpus_node->id = id.toInt();
        corpus_node->name = corpus_model.data(corpus_model.index(i, 1)).toString();

        corpus_node->row = i;


        storage_model.setQuery("SELECT * FROM storage WHERE corpus_id=" + id.toString());

        for (int j=0; j < storage_model.rowCount(); ++j) {
            HierarchyNode *storage_node = new HierarchyNode();
            QVariant id = storage_model.data(storage_model.index(j, 0));

            storage_node->id = id.toInt();
            storage_node->name = storage_model.data(storage_model.index(j, 3)).toString();
            storage_node->row = j;

            corpus_node->children.append(storage_node);
            storage_node->parent = corpus_node;

            all_nodes.append(storage_node);
        }

        nodes.append(corpus_node);
    }
}

QModelIndex HierarchyModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    if (!parent.isValid()) {
        return createIndex(row, column, const_cast<HierarchyNode*>(nodes[row]));
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
        return nodes.size();
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
            return currentNode->name;
            break;
        case FloorsColumn:
            return currentNode->floors;
            break;
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



