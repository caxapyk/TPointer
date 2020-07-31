#ifndef CORPUSMODEL_H
#define CORPUSMODEL_H

#include "tablemodel.h"
#include <QObject>

class CorpusModel : public TableModel
{
    Q_OBJECT

public:
    CorpusModel();

public slots:
    void setDefaultRecord(int, QSqlRecord&) override;
};

#endif // CORPUSMODEL_H
