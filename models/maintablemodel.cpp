#include "maintablemodel.h"

#include <QSqlQuery>
#include <QSqlRelation>

MainTableModel::MainTableModel(QObject *parent, QSqlDatabase db) :  QSqlRelationalTableModel(parent, db)
{
    setTable("tpointer");
    setRelation(11, QSqlRelation("feature", "id", "name AS feature_name"));

    setJoinMode(QSqlRelationalTableModel::LeftJoin);
    setEditStrategy(QSqlTableModel::OnManualSubmit);
}

int MainTableModel::count() const
{
    QSqlQuery query("SELECT COUNT(id) from tpointer");
    query.exec();

    query.first();
    return query.value(0).toInt();
}
