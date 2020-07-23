#ifndef CORPUSMODEL_H
#define CORPUSMODEL_H

#include "basemodel.h"
#include <QObject>

class CorpusModel : public BaseModel
{
    Q_OBJECT

public:
    CorpusModel();

public slots:
    void setDefaultRecord(int, QSqlRecord&) override;
};

#endif // CORPUSMODEL_H
