#include "storagemodel.h"

StorageModel::StorageModel(QObject *parent) : BaseModel(parent)
{
    setTable("storage");
    setItemName(tr("New storage"));
    setSort(2, Qt::AscendingOrder);

    setEditStrategy(QSqlTableModel::OnFieldChange);
}

void StorageModel::setDefaultRecord(int, QSqlRecord &record) {
    record.setValue(1, parentId()); //corpus_id
    record.setGenerated(1, true);
    record.setValue(2, rowCount()); //position
    record.setGenerated(2, true);
    record.setValue(3, itemName()); //name
    record.setGenerated(3, true);
};
