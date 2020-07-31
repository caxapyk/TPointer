#ifndef HIERARCHYMODEL_H
#define HIERARCHYMODEL_H

#include <QAbstractItemModel>
#include <QObject>
#include <QString>
#include <QVariant>
#include <QVector>

class HierarchyModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    struct HierarchyNode
    {
        QVariant id;
        QVariant name;
        QVariant floor;
        QVector<HierarchyNode*> children;
        HierarchyNode* parent;
        int level;
        int row;

        bool mapped;
    };
    typedef HierarchyNode HierarchyRootNode;

    enum NodeColumns { NameColumn, FloorsColumn, ColumnCount };

    HierarchyModel();
    ~HierarchyModel();

    HierarchyRootNode *root;
    enum NodeLevels { CorpusLevel, StorageLevel, CompartmentLevel, ShelvingLevel };

    void setupModelData(const QModelIndex &index=QModelIndex());

    bool canFetchMore(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    void fetchMore(const QModelIndex &parent) override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    bool hasChildren(const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole) override;

    void select();
    void clear();

private:
    void recursivelyRemoveNodes(HierarchyNode *node=nullptr);
    QMap<int, QVariant> columnHeaders;
};

#endif // HIERARCHYMODEL_H
