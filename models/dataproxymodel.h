#ifndef MAINTABLEPROXYMODEL_H
#define MAINTABLEPROXYMODEL_H

#include <QObject>
#include <QSortFilterProxyModel>

class DataProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    DataProxyModel();
};

#endif // MAINTABLEPROXYMODEL_H
