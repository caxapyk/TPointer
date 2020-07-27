#include "fundproxymodel.h"
#include <QDebug>

FundProxyModel::FundProxyModel() : QSortFilterProxyModel()
{
}

bool FundProxyModel::lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const
{
    if ((source_left.column() == 0 || source_left.column() == 1) and (source_right.column() == 0 || source_right.column() == 1)) {

        bool l_alpha = source_left.data().toString().contains(QRegExp("\\D+"));
        bool r_alpha = source_right.data().toString().contains(QRegExp("\\D+"));

        if ((l_alpha && r_alpha) || (!l_alpha && !r_alpha)) {

            QString left = source_left.data().toString().remove(QRegExp("\\D+"));
            QString right = source_right.data().toString().remove(QRegExp("\\D+"));

            return left.toInt() > right.toInt();
        } else if (l_alpha && !r_alpha) {
            return true;
        } else if (!l_alpha && r_alpha) {
            return false;
        }
    }


    return QSortFilterProxyModel::lessThan(source_left, source_right);
}
