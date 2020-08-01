#include "fundproxymodel.h"
#include "utils/fundsorter.h"
#include <QDebug>

FundProxyModel::FundProxyModel() : QSortFilterProxyModel()
{
}

bool FundProxyModel::lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const
{
    // 0 - fundmodel, 1 - fundtreemodel
    if ((source_left.column() == 0 || source_left.column() == 1) and (source_right.column() == 0 || source_right.column() == 1)) {
        FundSorter sorter;
        return sorter.lessThen(source_left.data().toString(), source_right.data().toString());
    }


    return QSortFilterProxyModel::lessThan(source_left, source_right);
}
