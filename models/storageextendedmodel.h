#ifndef STORAGESEARCHMODEL_H
#define STORAGESEARCHMODEL_H

#include "storagemodel.h"

#include <QObject>

class StorageExtendedModel : public StorageModel
{
    Q_OBJECT
public:
    StorageExtendedModel();
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

public slots:
    bool select() override;
};

#endif // STORAGESEARCHMODEL_H
