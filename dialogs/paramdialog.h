#ifndef PARAMDIALOG_H
#define PARAMDIALOG_H

#include "models/basemodel.h"
#include "widgets/itemcontroller.h"

#include <QAbstractItemView>
#include <QButtonGroup>
#include <QDialog>
#include <QModelIndex>

namespace Ui {
class ParamDialog;
}

class ParamDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ParamDialog(QWidget *parent = nullptr);
    ~ParamDialog();

private:
    void setupControls();
    void setupModels();
    void loadCorpuses();
    void loadFeatures();
    void loadStorages(QVariant id);

    Ui::ParamDialog *ui;
    BaseModel *m_corpus_model;
    BaseModel *m_storage_model;
    BaseModel *m_feature_model;

    ItemController *corpus_controls;
    ItemController *storage_controls;
    ItemController *feature_controls;

private slots:
    void createItem(QAbstractItemView *view);
    void moveUp(QAbstractItemView *view);
    void moveDown(QAbstractItemView *view);
    void selectCorpus(const QItemSelection &selected, const QItemSelection &deselected);
    void removeItem(QAbstractItemView *view);
};

#endif // SEARCHDIALOG_H
