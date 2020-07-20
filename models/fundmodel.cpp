#include "fundmodel.h"

#include <QSqlQuery>
#include <QStandardItem>

FundModel::FundModel(QObject *parent) : QStandardItemModel(parent)
{

}

void FundModel::select()
{
    QStandardItem *parentItem = invisibleRootItem();

    /* Substring digits from fund name to order by */
    QSqlQuery query("SELECT DISTINCT(fund), REGEXP_SUBSTR(fund, '[[:digit:]]+') as r_fund FROM tpointer ORDER BY CAST(fund as unsigned), CAST(r_fund as unsigned)");
    query.exec();

    while (query.next()) {
        QStandardItem *item = new QStandardItem(query.value(0).toString());
        parentItem->appendRow(item);
    }

    setHeaderData(0, Qt::Horizontal, tr("Funds"));
}
