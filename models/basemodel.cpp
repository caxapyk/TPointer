#include "basemodel.h"

#include <QSqlRecord>

BaseModel::BaseModel(QObject *parent, QSqlDatabase db) :  QSqlRelationalTableModel(parent, db)
{
    connect(this, SIGNAL(primeInsert(int, QSqlRecord&)),
            this, SLOT(setDefaultRecord(int, QSqlRecord&)));
}

bool BaseModel::insert() {
    if(insertRows(rowCount(), 1)) {
        submit();
        return true;
    }

    return false;
}

bool BaseModel::remove(QModelIndexList &list)
{
    for (int i = 0; i < list.size(); ++i) {
        if (!removeRows(list.at(i).row(), 1)) {
            select();
            return false;
        }
     }
    select();

    return true;
}

void BaseModel::setParentId(int column, QVariant id)
{
     m_parent_id=id;
     setFilter(record().fieldName(column) + "=" + id.toString());
}
