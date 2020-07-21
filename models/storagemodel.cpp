#include "storagemodel.h"

#include <QSqlRelation>
#include <QDebug>

StorageModel::StorageModel(QObject *parent) : BaseModel(parent)
{
    setTable("storage");
    setRelation(1, QSqlRelation("corpus", "id", "name as corpus_name"));

    setItemName(tr("New storage"));
    setSort(2, Qt::AscendingOrder);
    setPositionColumn(2);

    setEditStrategy(QSqlTableModel::OnFieldChange);

    setHeaderData(3, Qt::Horizontal, tr("Name"));
    setHeaderData(4, Qt::Horizontal, tr("Rooms count"));
    setHeaderData(5, Qt::Horizontal, tr("Floors"));
}

bool StorageModel::select()
{
    // insert column for Corpus/Storage value
    if (BaseModel::select() && insertColumns(columnCount(), 1)) {
        return true;
    } else {
        return false;
    }
}

QVariant StorageModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole && index.column() == 6) {
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
