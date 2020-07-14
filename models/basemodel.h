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
    BaseModel(QObject *parent = nullptr, QSqlDatabase db = QSqlDatabase());
    bool insert();
    QString itemName() const { return m_item_name; };
    QModelIndex moveUp(int row, int column);
    QModelIndex  moveDown(int row, int column);
    QVariant parentId() const { return m_parent_id; };
    bool remove(QModelIndexList &list);
    void setItemName(QString name) { m_item_name=name; };
    void setParentId(int column, QVariant id);

public slots:
    virtual void setDefaultRecord(int, QSqlRecord&){};

private:
    QModelIndex baseMove(SortDirection direction, int row, int column);
    QString m_item_name;
    QVariant m_parent_id;
};

#endif // BASEMODEL_H
