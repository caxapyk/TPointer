#include "storagemodel.h"

#include <QSqlRecord>
#include <QSqlRelation>

StorageModel::StorageModel(QObject *parent) : BaseModel(parent)
{
    setTable("storage");
    setItemName(tr("New storage"));

    setEditStrategy(QSqlTableModel::OnFieldChange);
}

void StorageModel::setDefaultRecord(int, QSqlRecord &record) {
    record.setValue(1, parentId());
    record.setGenerated(1, true);
    record.setValue(2, itemName());
    record.setGenerated(2, true);
};
