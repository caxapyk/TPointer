#ifndef PARAMDIALOG_H
#define PARAMDIALOG_H

#include "views/tablemodelcontroller.h"
#include "models/tablemodel.h"
#include "widgets/buttonscontrol.h"

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
    TableModel *m_corpus_model;
    TableModel *m_storage_model;
    TableModel *m_feature_model;

    ButtonsControl *cp_controls;
    ButtonsControl *st_controls;
    ButtonsControl *ft_controls;

    TableModelController *controller;

private slots:
    void selectCorpus(const QItemSelection &selected, const QItemSelection &deselected);
    void createItem(QAbstractItemView *view);
    void removeItem(QAbstractItemView *view);
    void moveUp(QAbstractItemView *view);
    void moveDown(QAbstractItemView *view);
};

#endif // SEARCHDIALOG_H
