#include "datamodel.h"
#include "dataproxymodel.h"
#include "utils/fundsorter.h"

#include <QDebug>

DataProxyModel::DataProxyModel() : QSortFilterProxyModel()
{

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
    }

    return QSortFilterProxyModel::lessThan(source_left, source_right);
}


