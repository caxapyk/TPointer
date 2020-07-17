#include "maintablemodel.h"

#include <QSqlQuery>

MainTableModel::MainTableModel(QObject *parent, QSqlDatabase db) :  QSqlTableModel(parent, db)
{
    setTable("tpointer");
    setEditStrategy(QSqlTableModel::OnManualSubmit);
}

int MainTableModel::count()
{
    QSqlQuery query("SELECt COUNT(id) from tpointer");
    query.exec();

    query.first();
    return query.value(0).toInt();
}
