#include "basedialog.h"

#include <QDebug>
#include <QMessageBox>
#include <QSortFilterProxyModel>

BaseDialog::BaseDialog(QWidget *parent) : QDialog(parent)
{
}

BaseDialog::~BaseDialog()
{
}


bool BaseDialog::createItem(QAbstractItemView *view)
{
    QAbstractItemView *v = qobject_cast<QAbstractItemView*> (view);
    BaseModel *model = qobject_cast<BaseModel*> (v->model());

    if(!model) { // check model is QSortFilterProxyModel
        QSortFilterProxyModel *m_proxy = qobject_cast<QSortFilterProxyModel*> (v->model());

        if (m_proxy) {
            model = qobject_cast<BaseModel*> (m_proxy->sourceModel());

            if(model && model->insert()) {
                QModelIndex index = m_proxy->mapFromSource(model->index(model->rowCount() - 1, 1));
                v->setCurrentIndex(index);
                v->scrollTo(index);

                return true;
            }
        }
    } else if (model->insert()) {
        QModelIndex index = model->index(model->rowCount() - 1, 1);
        v->setCurrentIndex(index);

        return true;
    }

    QMessageBox::warning(this, tr("Warning"), tr("Could not create item"), QMessageBox::Ok);
    return false;
}

bool BaseDialog::removeItem(QAbstractItemView *view)
{
    QAbstractItemView *v = qobject_cast<QAbstractItemView*> (view);
    QModelIndexList indexes = v->selectionModel()->selectedIndexes();

    if(!indexes.isEmpty()) {
        BaseModel *model = qobject_cast<BaseModel*> (v->model());

        if(!model) { // check model is QSortFilterProxyModel
            QSortFilterProxyModel *m_proxy = qobject_cast<QSortFilterProxyModel*> (v->model());

            if (m_proxy) {
                model = qobject_cast<BaseModel*> (m_proxy->sourceModel());

                /* create new QModelIndexList with source model indexes */
                QModelIndexList s_indexes;
                for (int i=0; i<indexes.length();++i) {
                    s_indexes.append(m_proxy->mapToSource(indexes.at(0)));
                }

                if(model && model->remove(s_indexes)) {
                    v->selectionModel()->clearSelection();
                    model->select();

                    return true;
                }
            }
        } else if (model->remove(indexes)) {
            v->selectionModel()->clearSelection();
            model->select();

            return true;
        }


        model->select();

        QMessageBox::warning(this, tr("Warning"), tr("Could not remove item"), QMessageBox::Ok);

        // select previous item
        v->selectionModel()->select(model->index(
                                        indexes.at(0).row()-1, indexes.at(0).column()),
                                    QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
    }

    return false;
}

void BaseDialog::moveUp(QAbstractItemView *view)
{
    QAbstractItemView *v = qobject_cast<QAbstractItemView*> (view);
    BaseModel *model = qobject_cast<BaseModel*> (v->model());

    QModelIndex index = v->currentIndex();
    QModelIndex up_index = model->moveUp(index.row());

    if (up_index.isValid()) {
        v->setCurrentIndex(up_index);
    }
}

void BaseDialog::moveDown(QAbstractItemView *view)
{
    QAbstractItemView *v = qobject_cast<QAbstractItemView*> (view);
    BaseModel *model = qobject_cast<BaseModel*> (v->model());

    QModelIndex index = v->currentIndex();
    QModelIndex down_index = model->moveDown(index.row());

    if (down_index.isValid()) {
        v->setCurrentIndex(down_index);
    }
}

