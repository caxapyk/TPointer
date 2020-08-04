#ifndef DATAFILTERMODEL_H
#define DATAFILTERMODEL_H

#include "filterstruct.h"

#include <QObject>
#include <QStandardItemModel>

class DataFilterModel : public QStandardItemModel
{
    Q_OBJECT
public:
    DataFilterModel(const FilterStruct &fs);
};

#endif // DATAFILTERMODEL_H
