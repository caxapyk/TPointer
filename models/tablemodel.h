#ifndef BASEMODEL_H
#define BASEMODEL_H

#include <QModelIndex>
#include <QModelIndexList>
#include <QSqlRelationalTableModel>
#include <QString>
#include <QVariant>

enum SortDirection{SortUp,SortDown};

class BaseModel : public QSqlRelationalTableModel
{
    Q_OBJECT

public:
    BaseModel();
    bool insert();
    QString itemName() const { return m_item_name; };
    QModelIndex moveUp(int row);
    QModelIndex  moveDown(int row);
    int parentId() { return m_parent; };
    bool remove(QModelIndexList &list);
    void setItemName(QString name) { m_item_name=name; };
    void setPositionColumn(int column);
    void setParentId(int parent) { m_parent=parent; };
    int getPositionColumn() const { return position_column; };

public slots:
    virtual void setDefaultRecord(int, QSqlRecord&) {};

private:
    QModelIndex baseMove(SortDirection direction, int row, int column);
    QString m_item_name;

    int position_column;
    int m_parent;
};

#endif // BASEMODEL_H
