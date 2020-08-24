#include "datamodel.h"
#include "dataproxymodel.h"
#include "utils/fundsorter.h"

#include <QColor>
#include <QDebug>

DataProxyModel::DataProxyModel() : QSortFilterProxyModel()
{

}

QVariant DataProxyModel::data(const QModelIndex &index, int role) const
{
    return QSortFilterProxyModel::data(index, role);
}

QVariant DataProxyModel::headerData(int section, Qt::Orientation orientation, int role) const
{
   if (role == Qt::DisplayRole && orientation == Qt::Vertical) {
       return QVariant(section + 1);
   }

   return QSortFilterProxyModel::headerData(section, orientation, role);
}

bool DataProxyModel::lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const
{
    // floor
    if (source_left.column() == DataModel::Floor && source_right.column() == DataModel::Floor) {
        // 'Basement' convert to int = 0
        if(source_left.data().toInt() == 0) {
            return false;
        } else {
            return source_left.data().toInt() > source_right.data().toInt();
        }
    // fund
    } else if (source_left.column() == DataModel::Fund && source_left.column() == DataModel::Fund) {
        FundSorter sorter;
        return sorter.lessThen(source_left.data().toString(), source_right.data().toString());
    // inventory, records
    } else if ((source_left.column() == DataModel::Inventory && source_left.column() == DataModel::Inventory)
               || (source_left.column() == DataModel::Records && source_left.column() == DataModel::Records)) {
        QString left = source_left.data().toString().remove(QRegExp("\\D+"));
        QString right = source_right.data().toString().remove(QRegExp("\\D+"));

        return left.toInt() > right.toInt();
    }

    return QSortFilterProxyModel::lessThan(source_left, source_right);
}


