#include "fundtreemodel.h"

#include <QRegExp>
#include <QStandardItem>
#include <QSqlQuery>
#include <QDebug>

FundTreeModel::FundTreeModel() : QStandardItemModel()
{
    setColumnCount(1);

    f_pre_soviet = new QStandardItem(tr("Pre-Soviet period funds"));
    setItem(0, 0, f_pre_soviet);

    f_soviet = new QStandardItem(tr("Soviet period funds"));
    setItem(1, 0, f_soviet);

    f_consignment = new QStandardItem(tr("Сonsignment funds"));
    setItem(2, 0, f_consignment);

    f_others = new QStandardItem(tr("Others funds"));
    setItem(3, 0, f_others);
}

FundTreeModel::~FundTreeModel()
{
    delete f_pre_soviet;
    delete f_soviet;
    delete f_consignment;
    delete f_others;

    clear();
}

void FundTreeModel::select()
{
    //QSqlQuery query("SELECT fund.number, fund.id, REGEXP_SUBSTR(fund.number, '[[:digit:]]+') AS r_number FROM fund");
    QSqlQuery query("SELECT id, number FROM fund");
    query.exec();

    while (query.next()) {
        QList<QStandardItem *> items;

        /*QStandardItem *item_id = new QStandardItem(query.value(1).toString());
        items.append(item_id);*/
        QStandardItem *item_number = new QStandardItem(query.value(1).toString());
        items.append(item_number);
        //QStandardItem *item_r_number = new QStandardItem(query.value(2).toString());
        //items.append(item_r_number);

        QString value = query.value(1).toString();

        if(value.contains(QRegExp("^\\d+$"))) {
            f_pre_soviet->appendRow(items);
        } else if(value.contains(QRegExp("[Р|P]+"))) {// russian|english
            f_soviet->appendRow(items);
        } else if(value.contains(QRegExp("П+"))) {
            f_consignment->appendRow(items);
        } else {
            f_others->appendRow(items);
        }
    }

    setHeaderData(1, Qt::Horizontal, tr("Funds (%1)").arg(query.size()));
}
