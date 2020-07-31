#ifndef FUNDMODEL_H
#define FUNDMODEL_H

#include "tablemodel.h"

#include <QObject>
#include <QSqlQuery>

class FundModel : public TableModel
{
    Q_OBJECT

public:
    FundModel();

public slots:
    void setDefaultRecord(int, QSqlRecord&) override;
};

#endif // FUNDMODEL_H
