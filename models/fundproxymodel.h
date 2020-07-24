#ifndef FUNDPROXYMODEL_H
#define FUNDPROXYMODEL_H

#include <QObject>
#include <QSortFilterProxyModel>

class FundProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    FundProxyModel();

protected:
    bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const override;
};

#endif // FUNDPROXYMODEL_H
