#ifndef BASEMODEL_H
#define BASEMODEL_H

#include <QModelIndex>
#include <QModelIndexList>
#include <QSqlRelationalTableModel>
#include <QString>
#include <QVariant>

enum SortDirection{SortUp,SortDown};

class TableModel : public QSqlRelationalTableModel
{
    Q_OBJECT

public:
    TableModel();
    QModelIndex moveUp(int row);
    QModelIndex  moveDown(int row);
    int parentId() { return m_parent; };
    void setPositionColumn(int column);
    void setParentId(int parent) { m_parent=parent; };
    int getPositionColumn() const { return position_column; };

public slots:
    virtual void setDefaultRecord(int, QSqlRecord&) {};

protected:
    int itemMaxNum(int column) const;

private:
    QModelIndex baseMove(SortDirection direction, int row, int column);

    int position_column;
    int m_parent;
};

#endif // BASEMODEL_H
