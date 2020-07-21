#include "basemodel.h"

#include <QSqlRecord>
#include <QVariant>

#include <QDebug>
#include <QSqlError>

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

/*
* Saves positon to database in <column>.
* Uses <direction> to sort <row> up/down
*/

QModelIndex BaseModel::baseMove(SortDirection direction, int row, int column)
{
    signed int x_row;

    switch (direction) {
        case (SortDirection::SortUp):
            x_row = (row == 0) ? rowCount() - 1 : row - 1;
            break;
        case (SortDirection::SortDown):
            x_row = (row == (rowCount()-1)) ? 0 : row + 1;
            break;
        default: x_row = -1;
    }

    if (row < 0)
        return QModelIndex();

    QModelIndex current = this->index(row, column);
    QModelIndex x_index = this->index(x_row, column);

    QVariant old_pos = current.data();
    QVariant new_pos = x_index.data();

    if (!setData(current, new_pos) || !setData(x_index, old_pos))
        return QModelIndex();

    sort(column, Qt::AscendingOrder);
    return x_index;
}

QModelIndex BaseModel::moveUp(int row)
{
    return baseMove(SortDirection::SortUp, row, getPositionColumn());
}

QModelIndex BaseModel::moveDown(int row)
{
    return baseMove(SortDirection::SortDown, row, getPositionColumn());
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

void BaseModel::setPositionColumn(int column)
{
    position_column = column;
}
