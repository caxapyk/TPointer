#ifndef HIERARCHYMODEL_H
#define HIERARCHYMODEL_H

#include <QAbstractItemModel>
#include <QObject>
#include <QString>
#include <QVariant>
#include <QVector>

struct HierarchyNode
{
    int id;
    QString name;
    QString floors;
    QVector<HierarchyNode*> children;
    HierarchyNode* parent;
    int row;

    bool mapped;
};

typedef QVector<HierarchyNode*> HierarchyNodeList;

class HierarchyModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    HierarchyModel();
    ~HierarchyModel();

    HierarchyNodeList nodes;
    HierarchyNodeList all_nodes;

    void setupModelData();
    int findRow(const HierarchyNode *HierarchyNode) const;

    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole);
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

private:
    void recursiveDeleteNodes(HierarchyNode *node=nullptr);
    QVector<QVariant> columnHeaders;

    enum NodeColumns { NameColumn, FloorsColumn, ColumnCount };
    enum NodeLevels { CorpusLevel, StorageLevel, CompartmentLevel, StellaryLevel };
};

#endif // HIERARCHYMODEL_H
