#include "tablemodelcontroller.h"

#include <QDebug>
#include <QMessageBox>
#include <QSortFilterProxyModel>

TableModelController::TableModelController(QObject *parent) : QObject(parent)
{
}

bool TableModelController::createItem(QAbstractItemView *v)
{
    TableModel *model = qobject_cast<TableModel*> (v->model());

    if(!model) { // check model is QSortFilterProxyModel
        QSortFilterProxyModel *m_proxy = qobject_cast<QSortFilterProxyModel*> (v->model());

        if (m_proxy) {
            model = qobject_cast<TableModel*> (m_proxy->sourceModel());

            if(model && model->insertRows(model->rowCount(), 1) && model->submit()) {
                QModelIndex index = m_proxy->mapFromSource(model->index(model->rowCount() - 1, 1));
                v->setCurrentIndex(index);
                v->scrollTo(index);

                return true;
            }
        }
    } else if (model->insertRows(model->rowCount(), 1) && model->submit()) {
        QModelIndex index = model->index(model->rowCount() - 1, 1);
        v->setCurrentIndex(index);
        v->scrollTo(index);

        return true;
    }
    return false;
}

bool TableModelController::removeItem(QAbstractItemView *v)
{
    QModelIndex index = v->currentIndex();

       if(index.isValid()) {
        TableModel *model = qobject_cast<TableModel*> (v->model());

        if(model) {
            if (model->removeRows(index.row(), 1)) {
                model->select();
                v->setCurrentIndex(v->model()->index(index.row() - 1, index.column()));
                return true;
            } else {
                model->select();
                v->setCurrentIndex(QModelIndex());
                return false;
            }

        } else {
            // check model is QSortFilterProxyModel
            QSortFilterProxyModel *m_proxy = qobject_cast<QSortFilterProxyModel*> (v->model());
            if (m_proxy) {
                model = qobject_cast<TableModel*> (m_proxy->sourceModel());
                if (model && m_proxy->removeRows(index.row(), 1)) {
                    model->select();
                    v->setCurrentIndex(v->model()->index(index.row() - 1, index.column()));
                    return true;
                }
            } else {
                model->select();
                v->setCurrentIndex(QModelIndex());
                return false;
            }
        }
    }

    return false;
}

void TableModelController::moveUp(QAbstractItemView *view)
{
    QAbstractItemView *v = qobject_cast<QAbstractItemView*> (view);
    TableModel *model = qobject_cast<TableModel*> (v->model());

    QModelIndex index = v->currentIndex();
    QModelIndex up_index = model->moveUp(index.row());

    if (up_index.isValid()) {
        v->setCurrentIndex(up_index);
    }
}

void TableModelController::moveDown(QAbstractItemView *view)
{
    QAbstractItemView *v = qobject_cast<QAbstractItemView*> (view);
    TableModel *model = qobject_cast<TableModel*> (v->model());

    QModelIndex index = v->currentIndex();
    QModelIndex down_index = model->moveDown(index.row());

    if (down_index.isValid()) {
        v->setCurrentIndex(down_index);
    }
}
