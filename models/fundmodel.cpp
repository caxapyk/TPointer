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

QString FundModel::getFundName(const QString &num)
{
    QSqlQuery query(QString("SELECT name from fund WHERE number='%1'").arg(num));
    if(query.exec() && query.first()) {
        return query.value(0).toString();
    }
    return QString();
}
