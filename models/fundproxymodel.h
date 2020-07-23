#ifndef FUNDPROXYMODEL_H
#define FUNDPROXYMODEL_H

#include <QObject>
#include <QSortFilterProxyModel>

class FundProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    FundProxyModel();
};

#endif // FUNDPROXYMODEL_H
