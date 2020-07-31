#ifndef MODELCONTROLLER_H
#define MODELCONTROLLER_H

#include "models/tablemodel.h"

#include <QAbstractItemView>
#include <QDialog>

#include <QObject>

class TableModelController : public QObject
{
    Q_OBJECT
public:
    explicit TableModelController(QObject *parent = nullptr);

    bool createItem(QAbstractItemView *view);
    bool removeItem(QAbstractItemView *view);
    void moveUp(QAbstractItemView *view);
    void moveDown(QAbstractItemView *view);
};

#endif // MODELCONTROLLER_H
