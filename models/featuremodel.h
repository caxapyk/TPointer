#ifndef FEATUREMODEL_H
#define FEATUREMODEL_H

#include "basemodel.h"

#include <QObject>
#include <QSqlRecord>

class FeatureModel : public BaseModel
{
    Q_OBJECT
public:
    FeatureModel();

public slots:
    void setDefaultRecord(int, QSqlRecord&) override;

};

#endif // FEATUREMODEL_H
