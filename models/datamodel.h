#ifndef MAINTABLEMODEL_H
#define MAINTABLEMODEL_H

#include "models/filterstruct.h"

#include <QAbstractTableModel>
#include <QObject>
#include <QSqlQuery>

class DataModel : public QAbstractTableModel
{

public:
    enum DataFields{
        Id,
        Floor,
        Storage,
        Compartment,
        Shelving,
        Cupboard,
        Shelf,
        Fund,
        Inventory,
        Records,
        Note,
        Features,
        ColumnsCount
    };

    typedef QList<QVariant> Node;

    DataModel();
    ~DataModel();
    void clear();
    int count() const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QString filter() const { return m_filter; };
    FilterStruct filterStruct() { return m_filterStruct; };
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    bool select();
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole) override;
    void setFilter(const FilterStruct &filter);
    QSqlQuery query() const { return m_query; };

private:
    FilterStruct m_filterStruct;

    QVector<Node> nodeList;
    QMap<int, QVariant> columnHeaders;
    QString m_filter;
    QSqlQuery m_query;
};

#endif // MAINTABLEMODEL_H
