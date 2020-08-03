#include "storagemodel.h"

#include <QSqlRelation>
#include <QDebug>

StorageModel::StorageModel() : TableModel()
{
    setTable("storage");
    setRelation(1, QSqlRelation("corpus", "id", "name as corpus_name"));
    setSort(2, Qt::AscendingOrder);
    setPositionColumn(2);

    setEditStrategy(QSqlTableModel::OnFieldChange); //!important for sorting

    setHeaderData(3, Qt::Horizontal, tr("Storage number"));
    setHeaderData(4, Qt::Horizontal, tr("Rooms count"));
    setHeaderData(5, Qt::Horizontal, tr("Floors"));
}

void StorageModel::setDefaultRecord(int, QSqlRecord &record) {
    record.setValue(1, parentId()); //corpus_id
    record.setGenerated(1, true);
    record.setValue(2, rowCount()); //position
    record.setGenerated(2, true);
    record.setValue(3, itemMaxNum(3)); //name
    record.setGenerated(3, true);
};
