#ifndef MAINTABLEMODEL_H
#define MAINTABLEMODEL_H

#include "models/filterstruct.h"

#include <QAbstractTableModel>
#include <QObject>
#include <QSqlQuery>
#include <QSqlRecord>

class DataModel : public QAbstractTableModel
{

    Q_OBJECT

public:
    enum DataFields{
        Id,
        Corpus,
        Storage,
        Floor,
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

    enum DataMetaFields{
        CorpusName
    };

    typedef QList<QVariant> Node;

    DataModel();
    ~DataModel();
    void clear();
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    int count() const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QString filter() const { return m_filter; };
    FilterStruct filterStruct() { return m_filterStruct; };
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    QVariant primaryInsert(QSqlRecord &record);
    bool primaryUpdate(QSqlRecord &record);
    QSqlRecord record();
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    bool select();
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    void setFilter(const FilterStruct &filter);
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole) override;
    QSqlQuery query() const { return m_query; };

private:
    FilterStruct m_filterStruct;

    QVector<Node*> nodeList;
    QMap<int, QVariant> meta;
    QMap<int, QVariant> columnHeaders;
    QString m_filter;
    QSqlQuery m_query;
};

#endif // MAINTABLEMODEL_H
