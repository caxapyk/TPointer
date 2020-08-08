#ifndef MAINTABLEPROXYMODEL_H
#define MAINTABLEPROXYMODEL_H

#include <QObject>
#include <QSortFilterProxyModel>

class DataProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    DataProxyModel();
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

protected:
    bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const override;
};

#endif // MAINTABLEPROXYMODEL_H
