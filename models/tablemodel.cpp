#include "tablemodel.h"

#include <QSqlRecord>
#include <QVariant>

#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>

TableModel::TableModel() :  QSqlRelationalTableModel()
{
    connect(this, SIGNAL(primeInsert(int, QSqlRecord&)),
            this, SLOT(setDefaultRecord(int, QSqlRecord&)));
}

QModelIndex TableModel::baseMove(SortDirection direction, int row, int column)
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

QModelIndex TableModel::moveUp(int row)
{
    return baseMove(SortDirection::SortUp, row, getPositionColumn());
}

QModelIndex TableModel::moveDown(int row)
{
    return baseMove(SortDirection::SortDown, row, getPositionColumn());
}

void TableModel::setPositionColumn(int column)
{
    position_column = column;
}
