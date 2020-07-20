#ifndef STORAGEMODEL_H
#define STORAGEMODEL_H

#include "basemodel.h"

#include <QObject>
#include <QSqlRecord>

class StorageModel : public BaseModel
{

    Q_OBJECT;

public:
    StorageModel(QObject *parent = nullptr);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;


public slots:
    void setDefaultRecord(int, QSqlRecord&) override;
    bool select() override;
};

#endif // STORAGEMODEL_H
