#include "fundmodel.h"

FundModel::FundModel() : BaseModel()
{
    setTable("fund");

    setItemName(tr("New fund"));
    setEditStrategy(QSqlTableModel::OnFieldChange);

    setHeaderData(1, Qt::Horizontal, tr("Number"));
    setHeaderData(2, Qt::Horizontal, tr("Name"));

    setSort(1, Qt::AscendingOrder);
}
