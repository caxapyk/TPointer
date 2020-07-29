#ifndef MAINTABLEPROXYMODEL_H
#define MAINTABLEPROXYMODEL_H

#include <QObject>
#include <QSortFilterProxyModel>

class MainTableProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    MainTableProxyModel();
};

#endif // MAINTABLEPROXYMODEL_H
