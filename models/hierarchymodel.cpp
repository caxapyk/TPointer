#include "hierarchymodel.h"

#include <QDebug>

struct NodeInfo
{
    QVariant name;
    QString floors;
    QVector<NodeInfo> children;
    NodeInfo* parent;

    bool mapped;
};

HierarchyModel::HierarchyModel()
{

}

HierarchyModel::~HierarchyModel()
{

}

void HierarchyModel::setupModelData() {
    NodeInfo info1;
    NodeInfo info2;
    info2.name = "name2";
    info2.floors = "3,4";
    info2.parent = &info1;
    info2.children = QVector<NodeInfo>();;

    info1.name = "name1";
    info1.floors = "1,2";
    info1.parent = nullptr;
    info1.children.append(info2);

    nodes.append(info1);

}

QModelIndex HierarchyModel::index(int row, int column, const QModelIndex &parent) const
{
    /*if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    if (!parent.isValid()) {
        return createIndex(row, column, const_cast<NodeInfo*>(&nodes[row]));
    }

   NodeInfo* parentInfo = static_cast<NodeInfo*>(parent.internalPointer());

    return createIndex(row, column, &parentInfo->children[row]);*/
    return createIndex(row, column, nullptr);
}

QModelIndex HierarchyModel::parent(const QModelIndex &child) const
{
    /*if (!child.isValid()) {
        return QModelIndex();
    }

    NodeInfo* childInfo = static_cast<NodeInfo*>(child.internalPointer());
    NodeInfo* parentInfo = childInfo->parent;
    if (parentInfo != nullptr) {
        return createIndex(0, IdColumn, parentInfo);
    }
    else {
        return QModelIndex();
    }*/
    return QModelIndex();
}

int HierarchyModel::rowCount(const QModelIndex &parent) const
{
    //if (!parent.isValid()) {
    //    return nodes.size();
    //}
    //const NodeInfo* parentInfo = static_cast<const NodeInfo*>(parent.internalPointer());
    //return parentInfo->children.size();
    return parent.isValid() ? 0 : 1;
}

int HierarchyModel::columnCount(const QModelIndex &) const
{
    return ColumnCount;
}

QVariant HierarchyModel::data(const QModelIndex &index, int role) const
{
    /*if (!index.isValid()) {

        return QVariant();
    }

    const NodeInfo* nodeInfo = static_cast<NodeInfo*>(index.internalPointer());
    //const QFileInfo& fileInfo = nodeInfo->fileInfo;

    qDebug() << "ya here::" << index.column();
    return QVariant("ssdfdsf");
    /*switch (index.column()) {
    case IdColumn:
        return "test";//nameData(fileInfo, role);
    /*case ModificationDateColumn:
        if (role == Qt::DisplayRole) {
            return fileInfo.lastModified();
        }
        break;
    case SizeColumn:
        if (role == Qt::DisplayRole) {
            return fileInfo.isDir()? QVariant(): fileInfo.size();
        }
        break;
    default:
    */
    /*    break;
    }*/
    //return QVariant();
    if (role == Qt::DisplayRole) {
        return QVariant("test");
    }
    return QVariant();
}

