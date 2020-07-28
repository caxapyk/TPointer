#ifndef TABLEMODELCONTROLLER_H
#define TABLEMODELCONTROLLER_H

#include <QObject>

class TableModelController : public QObject
{
    Q_OBJECT
public:
    explicit TableModelController(QObject *parent = nullptr);

signals:

};

#endif // TABLEMODELCONTROLLER_H
