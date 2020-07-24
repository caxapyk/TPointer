#include "fundproxymodel.h"
#include <QDebug>

FundProxyModel::FundProxyModel() : QSortFilterProxyModel()
{
}

bool FundProxyModel::lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const
{
    /* custom sort for fund number column */
    if (source_left.column() == 0 and source_right.column() == 0) {

        QModelIndex sl_index = source_left.siblingAtColumn(2); // left sort index for column `r_number`
        QModelIndex sr_index = source_right.siblingAtColumn(2); // right sort index for column `r_number`

        /* group sorting by alphabet/non-alphabet funds */
        bool l_alpha = source_left.data().toString().contains(QRegExp("\\D+")); // contains non-alphabet symbols
        bool r_alpha = source_right.data().toString().contains(QRegExp("\\D+")); // contains non-alphabet symbols

        if ((l_alpha && r_alpha) || (!l_alpha && !r_alpha)) {
            return (sl_index.data().toInt() > sr_index.data().toInt());
        } else if (l_alpha && !r_alpha) {
            return true;
        } else if (!l_alpha && r_alpha) {
            return false;
        }
    }

    return QSortFilterProxyModel::lessThan(source_left, source_right);
}
