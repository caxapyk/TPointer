#include "storageextendedmodel.h"

StorageExtendedModel::StorageExtendedModel() : StorageModel()
{

}

bool StorageExtendedModel::select()
{
    // insert column for Corpus/Storage value
    if (StorageModel::select() && insertColumns(columnCount(), 1)) {
        return true;
    } else {
        return false;
    }
}

QVariant StorageExtendedModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole && index.column() == 6) {
        // return Corpus/Storage value
        return QVariant(index.siblingAtColumn(1).data().toString()
                        + "/"
                        + index.siblingAtColumn(3).data().toString());
    }

    return BaseModel::data(index, role);
}
