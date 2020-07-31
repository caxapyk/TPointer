#include "application.h"
#include "datamodel.h"

#include <QDebug>
#include <QSqlRecord>

DataModel::DataModel() :  QAbstractTableModel()
{
    setHeaderData(0, Qt::Horizontal, tr("Id"));
    setHeaderData(1, Qt::Horizontal, tr("Floor"));
    setHeaderData(2, Qt::Horizontal, tr("Storage"));
    setHeaderData(3, Qt::Horizontal, tr("Compartment"));
    setHeaderData(4, Qt::Horizontal, tr("Shelving"));
    setHeaderData(5, Qt::Horizontal, tr("Cupboard"));
    setHeaderData(6, Qt::Horizontal, tr("Shelf"));
    setHeaderData(7, Qt::Horizontal, tr("Fund"));
    setHeaderData(8, Qt::Horizontal, tr("Inventory"));
    setHeaderData(9, Qt::Horizontal, tr("Records"));
    setHeaderData(10, Qt::Horizontal, tr("Note"));
    setHeaderData(11, Qt::Horizontal, tr("Features"));
}

DataModel::~DataModel()
{
    clear();
}

void DataModel::clear()
{
    beginResetModel();
    nodeList.clear();
    nodeList.squeeze();
    endResetModel();
}

bool DataModel::select()
{
    clear();
    m_query.prepare(QString("SELECT tpointer.`id`,tpointer.`floor`,storage.`name` AS storage_name,tpointer.`compartment`,tpointer.`shelving`,tpointer.`cupboard`,tpointer.`shelf`,fund.`number` AS fund_number,tpointer.`inventory`,tpointer.`records`,tpointer.`note`,feature.`name` AS feature_name FROM tpointer "
                  "LEFT JOIN storage ON tpointer.`storage`=storage.`id` "
                  "LEFT JOIN fund ON tpointer.`fund`=fund.`id` "
                  "LEFT JOIN feature ON tpointer.`feature`=feature.`id` "
                  "%1"
                  "ORDER BY storage.`name`, tpointer.`shelving`, tpointer.`cupboard`, tpointer.`shelf`")
              .arg(!filter().isNull() ? "WHERE " + filter() : ""));

    if (m_query.exec()) {
        beginResetModel();
        //qDebug() << m_query.lastQuery();
        while (m_query.next()) {
            Node node;
            for (int i = 0; i < DataModel::ColumnsCount; ++i) {
                node.append(m_query.value(i));
            }

            nodeList.append(node);
        }
        endResetModel();

        return true;
    }

    return false;
}

int DataModel::columnCount(const QModelIndex&) const
{
   return DataModel::ColumnsCount;
}

int DataModel::rowCount(const QModelIndex&) const
{
   return nodeList.size();
}

int DataModel::count() const
{
    QSqlQuery query("SELECT COUNT(id) from tpointer");
    query.exec();

    query.first();
    return query.value(0).toInt();
}

QVariant DataModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole ) {
        QVariant value = nodeList.at(index.row()).at(index.column());

        if (index.column() == 1 && value.toInt() == 0) {
            return QVariant(tr("Basement"));
        }

        return value;
    }

    return QVariant();
}

bool DataModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int)
{
    if ((section < 0)
            || ((orientation == Qt::Horizontal) && (section >= columnCount()))
            || ((orientation == Qt::Vertical) && (section >= rowCount()))) {
            return false;
    }

    if (orientation == Qt::Horizontal) {
        columnHeaders.insert(section, value);
        emit headerDataChanged(orientation, section, section);
        return true;
     }

     return false;
}

QVariant DataModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if ((section < 0)
            || ((orientation == Qt::Horizontal) && (section >= columnCount()))
            || ((orientation == Qt::Vertical) && (section >= rowCount()))) {
        return QVariant();
    }

    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        return columnHeaders.value(section);
     }

    return QVariant();
}

void DataModel::setFilter(const FilterStruct &fs)
{
    m_filterStruct = fs;
    m_filter.clear();

    // storage
    if (!fs.storage.isNull()) {
        m_filter += QString(" tpointer.`storage`=%1 ").arg(fs.storage.toInt());
    }
    // compartment
    if (!fs.compartment.isNull()) {
        if (!m_filter.isNull()) m_filter += QString("AND");
        m_filter += QString(" tpointer.`compartment`=%1 ").arg(fs.compartment.toString());
    }
    // shelving
    if (!fs.shelving.isNull()) {
        if (!m_filter.isNull()) m_filter += QString("AND");
        m_filter += QString(" tpointer.`shelving`=%1 ").arg(fs.shelving.toString());
    }
    // fund
    if (!fs.fund.isNull() && fs.fund_strict) {
        if (!m_filter.isNull()) m_filter += QString("AND");
        m_filter += QString(" fund='%1' ").arg(fs.fund.toString());
    } else if (!fs.fund.isNull() && !fs.fund_strict) {
        if (!m_filter.isNull()) m_filter += QString("AND");
        m_filter += QString(" fund LIKE '%%1%' ").arg(fs.fund.toString());
    }
    // feature
    if (!fs.feature.isNull()) {
        if (!m_filter.isNull()) m_filter += QString("AND");
        m_filter += QString(" tpointer.`feature`=%1 ").arg(fs.feature.toInt());
    }
}
