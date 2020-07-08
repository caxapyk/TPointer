#ifndef MAINTABLEMODEL_H
#define MAINTABLEMODEL_H

#include <QObject>
#include <QSqlTableModel>
#include <QSqlDatabase>

class MainTableModel : public QSqlTableModel
{

public:
    MainTableModel(QObject *parent = nullptr, QSqlDatabase db = QSqlDatabase());
};

#endif // MAINTABLEMODEL_H
