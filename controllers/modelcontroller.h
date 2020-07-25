#ifndef MODELCONTROLLER_H
#define MODELCONTROLLER_H

#include "models/basemodel.h"

#include <QAbstractItemView>
#include <QDialog>

#include <QObject>

class ModelController : public QObject
{
    Q_OBJECT
public:
    explicit ModelController(QObject *parent = nullptr);

    bool createItem(QAbstractItemView *view);
    bool removeItem(QAbstractItemView *view);
    void moveUp(QAbstractItemView *view);
    void moveDown(QAbstractItemView *view);
};

#endif // MODELCONTROLLER_H
