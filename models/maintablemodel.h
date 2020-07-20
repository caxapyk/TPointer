#ifndef MAINTABLEMODEL_H
#define MAINTABLEMODEL_H

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
};

#endif // MAINTABLEMODEL_H
