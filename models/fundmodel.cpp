#include "fundmodel.h"

#include <QSqlRecord>

FundModel::FundModel() : TableModel()
{
    setTable("fund");
    setEditStrategy(QSqlTableModel::OnFieldChange);

    setHeaderData(1, Qt::Horizontal, tr("Number"));
    setHeaderData(2, Qt::Horizontal, tr("Name"));
}

void FundModel::setDefaultRecord(int, QSqlRecord &record) {
    record.setValue(1, tr("New fund %1").arg(itemMaxNum(1, QRegExp("\\D+\\s\\D+\\d+"))));
    record.setGenerated(1, true);
};
