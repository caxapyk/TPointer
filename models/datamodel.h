#ifndef MAINTABLEMODEL_H
#define MAINTABLEMODEL_H

#include "models/basemodel.h"
#include "models/filterstruct.h"

#include <QObject>
#include <QSqlRelationalTableModel>
#include <QSqlDatabase>
#include <QVariant>

class MainTableModel : public BaseModel
{

public:
    MainTableModel();
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    int count() const;
    void _setFilter(const FilterStruct &fs);
};

#endif // MAINTABLEMODEL_H
