#ifndef DATAFILTERMODEL_H
#define DATAFILTERMODEL_H

#include <QObject>
#include <QStandardItemModel>

class DataFilterModel : public QStandardItemModel
{
    Q_OBJECT
public:
    DataFilterModel();
};

#endif // DATAFILTERMODEL_H
