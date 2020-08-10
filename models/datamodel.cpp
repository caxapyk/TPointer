#include "application.h"
#include "datamodel.h"

#include <QDebug>
#include <QSqlRecord>
#include <QSqlError>

DataModel::DataModel() :  QAbstractTableModel()
{
    setHeaderData(0, Qt::Horizontal, tr("Id"));
    setHeaderData(1, Qt::Horizontal, tr("Corpus"));
    setHeaderData(2, Qt::Horizontal, tr("Storage"));
    setHeaderData(3, Qt::Horizontal, tr("Floor"));
    setHeaderData(4, Qt::Horizontal, tr("Compartment"));
    setHeaderData(5, Qt::Horizontal, tr("Shelving"));
    setHeaderData(6, Qt::Horizontal, tr("Cupboard"));
    setHeaderData(7, Qt::Horizontal, tr("Shelf"));
    setHeaderData(8, Qt::Horizontal, tr("Fund"));
    setHeaderData(9, Qt::Horizontal, tr("Inventory"));
    setHeaderData(10, Qt::Horizontal, tr("Records"));
    setHeaderData(11, Qt::Horizontal, tr("Note"));
    setHeaderData(12, Qt::Horizontal, tr("Features"));
}

DataModel::~DataModel()
{
    clear();
}

void DataModel::clear()
{
    beginResetModel();

    // delete nodes
    while (!nodeList.isEmpty())
         delete nodeList.takeFirst();

    nodeList.clear();
    nodeList.squeeze();
    endResetModel();
}

bool DataModel::select()
{
    clear();
    m_query.prepare(QString("SELECT tpointer.`id`, corpus.`name` AS corpus_name, storage.`name` AS storage_name, tpointer.`floor`, tpointer.`compartment`, tpointer.`shelving`, tpointer.`cupboard`, tpointer.`shelf`, fund.`number` AS fund_number, tpointer.`inventory`, tpointer.`records`, tpointer.`note`, feature.`name` AS feature_name FROM tpointer "
                  "LEFT JOIN storage ON tpointer.`storage`=storage.`id` "
                  "LEFT JOIN corpus ON storage.`corpus`=corpus.`id` "
                  "LEFT JOIN fund ON tpointer.`fund`=fund.`id` "
                  "LEFT JOIN feature ON tpointer.`feature`=feature.`id` "
                  "%1"
                  "ORDER BY storage.`name`, tpointer.`floor`, tpointer.`compartment`, tpointer.`shelving`, tpointer.`cupboard`, tpointer.`shelf`")
              .arg(!filter().isNull() ? "WHERE" + filter() : QString()));

    if (m_query.exec()) {
        beginResetModel();

        while (m_query.next()) {
            Node *node = new Node();

            for (int i = 0; i < DataModel::ColumnsCount; ++i) {
                    node->append(m_query.value(i));
            }

            nodeList.append(node);
        }

        endResetModel();

        return true;


    } else {
        qDebug() << m_query.lastError().text();
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
    if (role == Qt::DisplayRole) {
        Node *node = nodeList.at(index.row());

        QVariant value = (node->at(index.column()).isValid()) ? node->at(index.column()) : QVariant();

        if (index.column() == DataModel::Floor && value.toInt() == 0) {
            return QVariant(tr("Basement"));
        }

        return value;
    }

    return QVariant();
}

QVariant DataModel::primaryInsert(QSqlRecord &record)
{
    QSqlQuery query;

    QList<QString> f; // fields
    QList<QString> v; // values

    for (int i = 0; i < record.count(); ++i) {
        f.append(record.fieldName(i));
        v.append("?");
    }

    QString q = QString("INSERT INTO tpointer (%1) VALUES (%2)")
            .arg(QStringList(f).join(","))
            .arg(QStringList(v).join(","));

    query.prepare(q);

    for (int i = 0; i < v.length(); ++i) {
        QVariant value = (!record.value(i).toString().isEmpty()) ? record.value(i) : QVariant();
        query.addBindValue(value);
    }

    if (query.exec()) {
        return query.lastInsertId();
    }

    qDebug() << query.lastError().text();
    return QVariant();
}

bool DataModel::primaryUpdate(QSqlRecord &record)
{
    QSqlQuery query;

    QList<QString> fs;  // field=value

    for (int i = 0; i < record.count(); ++i) {
        fs.append(QString("%1=?").arg(record.fieldName(i)));
    }

    QString q = QString("UPDATE tpointer SET %1 WHERE id=%2")
            .arg(QStringList(fs).join(","))
            .arg(record.value("id").toString());

    query.prepare(q);

    for (int i = 0; i < record.count(); ++i) {
        QVariant value = (!record.value(i).toString().isEmpty()) ? record.value(i) : QVariant();
        query.addBindValue(value);
    }

    if (query.exec()) {
        return true;
    }

    qDebug() << query.lastError().text();
    return false;
}

QSqlRecord DataModel::record()
{
    QSqlQuery query("SELECT * FROM tpointer WHERE id=-1;");

    if (query.exec()) {
        return query.record();
    }

    return QSqlRecord();
}

bool DataModel::setNode(Node &node)
{
    int row = rowCount();

    if(insertRows(row, 1)) {
        for(int i = 0; i < node.length(); ++i) {
            if(!setData(index(row, i), node.at(i)))
                return false;
        }
        return true;
    }

    return false;
}

bool DataModel::removeRows(int row, int count, const QModelIndex &parent)
{
    QSqlQuery r_query;
    QList<QString> ids;

    // fill ids to list
    for(int i = row; i < ( row + count ); ++i) {
        ids.append(index(i, DataModel::Id).data().toString());
    }

    r_query.prepare(
                QString("DELETE FROM tpointer WHERE id IN (%1)")
                .arg(ids.join(",")));

    if (r_query.exec()) {
        beginRemoveRows(parent, row, row + count - 1);
        nodeList.remove(row, count);
        endRemoveRows();

        return true;
    }

    qDebug() << r_query.lastError().text();

    return false;
}

bool DataModel::insertRows(int row, int count, const QModelIndex &parent)
{
    if (row < 0 || count <= 0 || row > rowCount() || parent.isValid())
            return false;

    beginInsertRows(parent, row, row + count - 1);

    // new empty node
    Node *node = new Node();

    for(int i = 0; i < DataModel::ColumnsCount; ++i) {
        node->append(QVariant());
    }

    nodeList.append(node);

    endInsertRows();

    return true;
}

bool DataModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!index.isValid() || role != Qt::EditRole) {
        return false;
    }

    Node *node = nodeList.at(index.row());
    node->replace(index.column(), value);

    emit dataChanged(index, index);

    return true;
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
    if (!fs.corpus.isNull()) {
        if (!m_filter.isNull()) m_filter += QString("AND");
        m_filter += QString(" storage.`corpus`=%1 ").arg(fs.corpus.toInt());
    }
    // storage
    if (!fs.storage.isNull()) {
        if (!m_filter.isNull()) m_filter += QString("AND");
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
    // fund_name
    if (!fs.fund_name.isNull() && fs.fund_strict) {
        if (!m_filter.isNull()) m_filter += QString("AND");
        m_filter += QString(" fund.`number`='%1' ").arg(fs.fund_name.toString());
    } else if (!fs.fund_name.isNull() && !fs.fund_strict) {
        if (!m_filter.isNull()) m_filter += QString("AND");
        m_filter += QString(" fund.`number` LIKE '%%1%' ").arg(fs.fund_name.toString());
    }
    // feature
    if (!fs.feature.isNull()) {
        if (!m_filter.isNull()) m_filter += QString("AND");
        m_filter += QString(" tpointer.`feature`=%1 ").arg(fs.feature.toInt());
    }
}
