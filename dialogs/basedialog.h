#ifndef BASEDIALOG_H
#define BASEDIALOG_H

#include "models/basemodel.h"

#include <QAbstractItemView>
#include <QDialog>
#include <QObject>

class BaseDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BaseDialog(QWidget *parent = nullptr);
    ~BaseDialog();

protected slots:
    bool createItem(QAbstractItemView *view);
    bool removeItem(QAbstractItemView *view);
    void moveUp(QAbstractItemView *view);
    void moveDown(QAbstractItemView *view);

};

#endif // BASEDIALOG_H
