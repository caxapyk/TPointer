#ifndef FEATUREMODEL_H
#define FEATUREMODEL_H

#include "tablemodel.h"

#include <QObject>
#include <QSqlRecord>

class FeatureModel : public TableModel
{
    Q_OBJECT
public:
    FeatureModel();

public slots:
    void setDefaultRecord(int, QSqlRecord&) override;

};

#endif // FEATUREMODEL_H
