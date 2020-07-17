#ifndef FUNDPROXYMODEL_H
#define FUNDPROXYMODEL_H

#include <QObject>
#include <QSortFilterProxyModel>

class FundProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    FundProxyModel(QObject *parent = nullptr);
};

#endif // FUNDPROXYMODEL_H
