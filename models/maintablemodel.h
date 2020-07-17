#ifndef MAINTABLEMODEL_H
#define MAINTABLEMODEL_H

#include <QObject>
#include <QSqlTableModel>
#include <QSqlDatabase>

class MainTableModel : public QSqlTableModel
{

public:
    MainTableModel(QObject *parent = nullptr, QSqlDatabase db = QSqlDatabase());
    int count();
};

#endif // MAINTABLEMODEL_H
