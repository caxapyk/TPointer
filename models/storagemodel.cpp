#include "storagemodel.h"

#include <QSqlRelation>
#include <QDebug>

StorageModel::StorageModel(QObject *parent) : BaseModel(parent)
{
    setTable("storage");
    setRelation(1, QSqlRelation("corpus", "id", "name"));

    setItemName(tr("New storage"));
    setSort(2, Qt::AscendingOrder);
    setPositionColumn(2);

    setEditStrategy(QSqlTableModel::OnFieldChange);
}

bool StorageModel::select()
{
    if (BaseModel::select() && insertColumn(columnCount())) {
        return true;
    } else {
        return false;
    }
}

QVariant StorageModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole && index.column() == columnCount() - 1) {
        // return Corpus/Storage value
        return QVariant(index.siblingAtColumn(1).data().toString()
                        + "/"
                        + index.siblingAtColumn(3).data().toString());
    }

    return BaseModel::data(index, role);
}

void StorageModel::setDefaultRecord(int, QSqlRecord &record) {
    record.setValue(1, parentId()); //corpus_id
    record.setGenerated(1, true);
    record.setValue(2, rowCount()); //position
    record.setGenerated(2, true);
    record.setValue(3, itemName()); //name
    record.setGenerated(3, true);
};
