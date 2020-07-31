#ifndef STORAGEMODEL_H
#define STORAGEMODEL_H

#include "tablemodel.h"

#include <QObject>
#include <QSqlRecord>

class StorageModel : public TableModel
{

    Q_OBJECT;

public:
    StorageModel();

public slots:
    void setDefaultRecord(int, QSqlRecord&) override;
};

#endif // STORAGEMODEL_H
