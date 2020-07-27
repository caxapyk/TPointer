#include "fundtreemodel.h"
#include "fundmodel.h"

#include <QRegExp>
#include <QStandardItem>
#include <QSqlRecord>
#include <QDebug>

FundTreeModel::FundTreeModel() : QStandardItemModel()
{
    setColumnCount(1);
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
    clear();

    FundModel model;
    model.select();

    f_pre_soviet = new QStandardItem(tr("Pre-Soviet period funds"));
    setItem(0, 0, f_pre_soviet);

    f_soviet = new QStandardItem(tr("Soviet period funds"));
    setItem(1, 0, f_soviet);

    f_consignment = new QStandardItem(tr("Сonsignment funds"));
    setItem(2, 0, f_consignment);

    f_others = new QStandardItem(tr("Others funds"));
    setItem(3, 0, f_others);

    for (int i=0; i<model.rowCount(); ++i) {
        QStandardItem *item = new QStandardItem(model.record(i).value(1).toString());

        QVariant id = model.record(i).value(0);
        QVariant value = model.record(i).value(1);

        if(value.toString().contains(QRegExp("^\\d+$"))) {
            f_pre_soviet->appendRow(item);
            item->setData(id);
        } else if(value.toString().contains(QRegExp("[Р|P]+"))) {// russian|english
            f_soviet->appendRow(item);
            item->setData(id);
        } else if(value.toString().contains(QRegExp("П+"))) {
            f_consignment->appendRow(item);
            item->setData(id);
        } else {
            f_others->appendRow(item);
            item->setData(id);
        }
    }

    setHeaderData(0, Qt::Horizontal, tr("Funds (%1)").arg(model.rowCount()));
}
