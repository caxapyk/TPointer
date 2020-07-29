#include "application.h"
#include "maintablemodel.h"

#include <QDebug>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlRelation>

MainTableModel::MainTableModel() :  BaseModel()
{
    setTable("tpointer");
    setRelation(2, QSqlRelation("storage", "id", "name AS storage_name"));
    setRelation(7, QSqlRelation("fund", "id", "number AS fund_name"));
    setRelation(11, QSqlRelation("feature", "id", "name AS feature_name"));

    setJoinMode(QSqlRelationalTableModel::LeftJoin);
    setEditStrategy(QSqlTableModel::OnRowChange);

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

void MainTableModel::_setFilter(const FilterStruct &fs)
{
    QString filter;
    // storage
    if (!fs.storage.isNull()) {
        filter += QString(" storage=%1 ").arg(fs.storage.toInt());
    }
    // compartment
    if (!fs.compartment.isNull()) {
        if (!filter.isNull()) filter += QString("AND");
        filter += QString(" compartment=%1 ").arg(fs.compartment.toString());
    /// Хрень!!!!!!!!!!!!!!!!!!! убирай пустой compartment
     /*check that now loading by shelving, not fund,
     * because when loading by fund compartment cannot present in query
     */
    } else if (fs.compartment.isNull() && fs.fund.isNull()) {
        if (!filter.isNull()) filter += QString("AND");
        filter += QString(" compartment IS NULL ");
    }
    // shelving
    if (!fs.shelving.isNull()) {
        if (!filter.isNull()) filter += QString("AND");
        filter += QString(" shelving=%1 ").arg(fs.shelving.toString());
    }
    // fund
    if (!fs.fund.isNull() && fs.fund_strict) {
        if (!filter.isNull()) filter += QString("AND");
        filter += QString(" fund='%1' ").arg(fs.fund.toString());
    } else if (!fs.fund.isNull() && !fs.fund_strict) {
        if (!filter.isNull()) filter += QString("AND");
        filter += QString(" fund LIKE '%%1%' ").arg(fs.fund.toString());
    }
    // feature
    if (!fs.feature.isNull()) {
        if (!filter.isNull()) filter += QString("AND");
        filter += QString(" feature=%1 ").arg(fs.feature.toInt());
    }

    if(!filter.isNull()) {
        setFilter(filter);
    }
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
    // rename 0 floor to 'Basement'
    if (role == Qt::DisplayRole && index.column() == 1) {
        if (record(index.row()).value(index.column()).toInt() == 0) {
            return QVariant(tr("Basement"));
        } else {
            return QVariant(tr("%1 floor").arg(record(index.row()).value(1).toString()));
        }
    //rename 0 compartmnet/shelving to blank line
    } else if (role == Qt::DisplayRole && (index.column() == 3 || index.column() == 4)){
        if (record(index.row()).value(index.column()).toInt() == 0) {
            return QVariant();
        }
    }

    return QSqlRelationalTableModel::data(index, role);
}
