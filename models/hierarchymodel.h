#ifndef HIERARCHYMODEL_H
#define HIERARCHYMODEL_H

#include <QAbstractItemModel>
#include <QObject>

struct NodeInfo;
typedef QVector<NodeInfo> NodeInfoList;

enum NodeColumns
{
    IdColumn,
    NameColumn,
    FloorsColumn,
    ColumnCount
};

class HierarchyModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    HierarchyModel();
    ~HierarchyModel();
    NodeInfoList nodes;

    void setupModelData();
    int findRow(const NodeInfo *nodeInfo) const;

    virtual QModelIndex index(int row, int column, const QModelIndex &parent) const;
    virtual QModelIndex parent(const QModelIndex &child) const;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
};

#endif // HIERARCHYMODEL_H
