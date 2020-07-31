#include "featuremodel.h"

FeatureModel::FeatureModel() : TableModel()
{
    setTable("feature");
    setItemName(tr("New feature"));
    setSort(1, Qt::AscendingOrder);
    setPositionColumn(1);

    setEditStrategy(QSqlTableModel::OnFieldChange); //!important for sorting
}

void FeatureModel::setDefaultRecord(int, QSqlRecord &record) {
    record.setValue(1, rowCount()); //position
    record.setGenerated(1, true);
    record.setValue(2, itemName()); //name
    record.setGenerated(2, true);
};
