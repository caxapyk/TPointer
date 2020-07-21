#ifndef MAINTABLEMODEL_H
#define MAINTABLEMODEL_H

#include "models/filterstruct.h"

#include <QObject>
#include <QSqlRelationalTableModel>
#include <QSqlDatabase>
#include <QVariant>

class MainTableModel : public QSqlRelationalTableModel
{

public:
    MainTableModel(QObject *parent = nullptr, QSqlDatabase db = QSqlDatabase());
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    int count() const;
    void _setFilter(const FilterStruct &fs);
};

#endif // MAINTABLEMODEL_H
