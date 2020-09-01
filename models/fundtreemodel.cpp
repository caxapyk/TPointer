#include "fundtreemodel.h"

#include <QRegExp>
#include <QStandardItem>
#include <QSqlRecord>
#include <QDebug>

FundTreeModel::FundTreeModel() : QStandardItemModel()
{
    setColumnCount(1);
    setHeaderData(0, Qt::Horizontal, tr("Funds"));

    m_model = new FundModel;
}

FundTreeModel::~FundTreeModel()
{
    delete m_model;
    clear();
}

void FundTreeModel::initialize()
{
    f_pre_soviet = new QStandardItem(tr("Pre-Soviet period funds"));
    setItem(0, 0, f_pre_soviet);

    f_soviet = new QStandardItem(tr("Soviet period funds"));
    setItem(1, 0, f_soviet);

    f_consignment = new QStandardItem(tr("Сonsignment funds"));
    setItem(2, 0, f_consignment);

    f_others = new QStandardItem(tr("Others funds"));
    setItem(3, 0, f_others);
}

void FundTreeModel::select()
{
    QVariant col0Header = headerData(0, Qt::Horizontal);

    clear();
    initialize();

    m_model->select();

    for (int i=0; i<m_model->rowCount(); ++i) {
        QStandardItem *item = new QStandardItem(m_model->record(i).value(1).toString());

        QVariant id = m_model->record(i).value(0);
        QVariant value = m_model->record(i).value(1);

        if(value.toString().contains(QRegExp("^\\d+$"))) {
            f_pre_soviet->appendRow(item);
            item->setData(id);
        } else if(value.toString().contains(QRegExp("[Р|P]+"))) { // russian|english
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

    setHeaderData(0, Qt::Horizontal, col0Header);
}

void FundTreeModel::setFilterByStorageCompartment(const QVariant &storage, const QVariant &comp)
{
    QString filter = QString("fund.`id` IN (SELECT fund.`id` FROM fund LEFT JOIN tpointer on tpointer.`fund`=fund.`id` WHERE tpointer.`storage`=%1 %2)")
            .arg(storage.toString())
            .arg((comp.isNull()) ? "" : QString(" AND tpointer.`compartment`=%1").arg(comp.toString()));

    m_model->setFilter(filter);

    storageId = storage;
    compart = comp;
}

QVariant FundTreeModel::data(const QModelIndex &index, int role) const
{
    if (index.column() == 0 && role == Qt::DecorationRole) {
        if (!index.parent().isValid()) {
            return QIcon(":/icons/folder-16.png");
        } else {
            return QIcon(":/icons/documents-16.png");
        }
    }

    return QStandardItemModel::data(index, role);
}
