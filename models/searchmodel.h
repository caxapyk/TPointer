#ifndef SEARCHMODEL_H
#define SEARCHMODEL_H

#include "datamodel.h"

#include <QObject>
#include <QSqlQueryModel>

class SearchModel : public QSqlQueryModel
{
    Q_OBJECT
public:
    SearchModel();

private:
    QString m_filter;
};

#endif // SEARCHMODEL_H
