#include "fundmodel.h"

#include <QSqlRecord>

FundModel::FundModel() : BaseModel()
{
    setTable("fund");

    setItemName(tr("New fund"));
    setEditStrategy(QSqlTableModel::OnFieldChange);

    setHeaderData(1, Qt::Horizontal, tr("Number"));
    setHeaderData(2, Qt::Horizontal, tr("Name"));

    setSort(1, Qt::AscendingOrder);
}

void FundModel::setDefaultRecord(int, QSqlRecord &record) {
    record.setValue(1, itemName());
    record.setGenerated(1, true);
};
