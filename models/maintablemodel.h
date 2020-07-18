#ifndef MAINTABLEMODEL_H
#define MAINTABLEMODEL_H

#include <QObject>
#include <QSqlRelationalTableModel>
#include <QSqlDatabase>

class MainTableModel : public QSqlRelationalTableModel
{

public:
    MainTableModel(QObject *parent = nullptr, QSqlDatabase db = QSqlDatabase());
    int count() const;
};

#endif // MAINTABLEMODEL_H
