#include "featuremodel.h"

FeatureModel::FeatureModel() : TableModel()
{
    setTable("feature");
    setSort(1, Qt::AscendingOrder);
    setPositionColumn(1);

    setEditStrategy(QSqlTableModel::OnFieldChange); //!important for sorting
}

void FeatureModel::setDefaultRecord(int, QSqlRecord &record) {
    record.setValue(1, rowCount()); //position
    record.setGenerated(1, true);
    record.setValue(2, tr("Feature %1").arg(itemMaxNum(2))); //name
    record.setGenerated(2, true);
};
