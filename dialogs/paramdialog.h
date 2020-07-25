#ifndef PARAMDIALOG_H
#define PARAMDIALOG_H

#include "controllers/modelcontroller.h"
#include "models/basemodel.h"
#include "widgets/itemcontroller.h"

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

    ItemController *cp_controls;
    ItemController *st_controls;
    ItemController *ft_controls;

    ModelController *controller;

private slots:
    void selectCorpus(const QItemSelection &selected, const QItemSelection &deselected);
    void createItem(QAbstractItemView *view);
    void removeItem(QAbstractItemView *view);
    void moveUp(QAbstractItemView *view);
    void moveDown(QAbstractItemView *view);
};

#endif // SEARCHDIALOG_H
