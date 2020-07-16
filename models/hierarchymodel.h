#ifndef HIERARCHYMODEL_H
#define HIERARCHYMODEL_H

#include <QAbstractItemModel>
#include <QObject>
#include <QString>
#include <QVariant>
#include <QVector>

struct HierarchyNode
{
    QVariant id;
    QVariant name;
    QVector<HierarchyNode*> children;
    HierarchyNode* parent;
    int level;
    int row;

    bool mapped;
};
typedef HierarchyNode HierarchyRootNode;

class HierarchyModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    HierarchyModel();
    ~HierarchyModel();

    HierarchyRootNode root;
    enum NodeLevels { CorpusLevel, StorageLevel, CompartmentLevel, ShelvingLevel };

    void setupModelData(const QModelIndex &index=QModelIndex());

    bool canFetchMore(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    void fetchMore(const QModelIndex &parent);
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    bool hasChildren(const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole);

    void select();
    void clear();

private:
    void recursiveDeleteNodes(HierarchyNode *node=nullptr);
    QVector<QVariant> columnHeaders;

    enum NodeColumns { NameColumn, FloorsColumn, ColumnCount };
};

#endif // HIERARCHYMODEL_H
