#ifndef STORAGESEARCHMODEL_H
#define STORAGESEARCHMODEL_H

#include <QObject>
#include <QSqlQueryModel>

class StorageExtendedModel : public QSqlQueryModel
{
    Q_OBJECT
public:
    StorageExtendedModel();
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

public slots:
    void select();
};

#endif // STORAGESEARCHMODEL_H
