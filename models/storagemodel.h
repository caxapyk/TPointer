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

public slots:
    void setDefaultRecord(int, QSqlRecord&) override;
};

#endif // STORAGEMODEL_H
