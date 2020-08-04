#include "datafiltermodel.h"

DataFilterModel::DataFilterModel(const FilterStruct &fs) : QStandardItemModel()
{
    QStandardItem *root = invisibleRootItem();
    QList<QStandardItem*> filterList;

    filterList.append(new QStandardItem(fs.corpus.toString()));
    filterList.append(new QStandardItem(fs.storage.toString()));
    filterList.append(new QStandardItem(fs.compartment.toString()));
    filterList.append(new QStandardItem(fs.shelving.toString()));
    filterList.append(new QStandardItem(fs.fund.toString()));

    root->appendRow(filterList);
}
