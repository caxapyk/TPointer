#include "noitemlistmodel.h"

NoItemListModel::NoItemListModel() : QAbstractListModel()
{
}
NoItemListModel::~NoItemListModel()
{
}

int NoItemListModel::rowCount(const QModelIndex&) const
{
   return baseModel()->rowCount(QModelIndex()) + 1;
}

QVariant NoItemListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    if (index.row() == 0 && role == Qt::DisplayRole) {
           return QVariant(tr("Select item..."));
    }
    return baseModel()->data(baseModel()->index(index.row() - 1, baseColumn()), role);
}
