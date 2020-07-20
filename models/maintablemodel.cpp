#include "maintablemodel.h"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlRelation>

MainTableModel::MainTableModel(QObject *parent, QSqlDatabase db) :  QSqlRelationalTableModel(parent, db)
{
    setTable("tpointer");
    setRelation(2, QSqlRelation("storage", "id", "name AS storage_name"));
    setRelation(11, QSqlRelation("feature", "id", "name AS feature_name"));

    setJoinMode(QSqlRelationalTableModel::LeftJoin);
    setEditStrategy(QSqlTableModel::OnManualSubmit);

    setHeaderData(1, Qt::Horizontal, tr("Floor"));
    setHeaderData(2, Qt::Horizontal, tr("Storage"));
    setHeaderData(3, Qt::Horizontal, tr("Compartment"));
    setHeaderData(4, Qt::Horizontal, tr("Shelving"));
    setHeaderData(5, Qt::Horizontal, tr("Cupboard"));
    setHeaderData(6, Qt::Horizontal, tr("Shelf"));
    setHeaderData(7, Qt::Horizontal, tr("Fund"));
    setHeaderData(8, Qt::Horizontal, tr("Inventory"));
    setHeaderData(9, Qt::Horizontal, tr("Records"));
    setHeaderData(10, Qt::Horizontal, tr("Note"));
    setHeaderData(11, Qt::Horizontal, tr("Features"));
}

int MainTableModel::count() const
{
    QSqlQuery query("SELECT COUNT(id) from tpointer");
    query.exec();

    query.first();
    return query.value(0).toInt();
}

QVariant MainTableModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole && index.column() == 1) {
        if (record(index.row()).value(1).toInt() == 0) {
            return QVariant(tr("Basement"));
        } else {
            return QVariant(tr("%1 floor").arg(record(index.row()).value(1).toString()));
        }
    }

    return QSqlRelationalTableModel::data(index, role);
}
