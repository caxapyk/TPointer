#include "storageextendedmodel.h"

StorageExtendedModel::StorageExtendedModel() : QSqlQueryModel()
{

}

void StorageExtendedModel::select()
{
    setQuery("SELECT storage.`id`, corpus.`name`, storage.`position`, storage.`name`, storage.`rooms`, storage.`floors` from storage "
             "JOIN corpus ON storage.`corpus_id`=corpus.`id` "
             "ORDER BY corpus.`name`, storage.`position`");

    // insert column for Corpus/Storage value
    insertColumns(columnCount(), 1);
}

QVariant StorageExtendedModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole && index.column() == 6) {
        // return Corpus/Storage value
        return QVariant(index.siblingAtColumn(1).data().toString()
                        + "/"
                        + index.siblingAtColumn(3).data().toString());
    }

    return QSqlQueryModel::data(index, role);
}
