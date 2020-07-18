#ifndef PARAMDIALOG_H
#define PARAMDIALOG_H

#include "models/basemodel.h"

#include <QAbstractItemView>
#include <QButtonGroup>
#include <QDialog>
#include <QModelIndex>

#include "widgets/buttoncontrols.h"

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

    ButtonControls *storage_controls;
    ButtonControls *feature_controls;

private slots:
    void createItem(QAbstractItemView *view);
    void moveUp(QAbstractItemView *view);
    void moveDown(QAbstractItemView *view);
    void selectCorpus(const QModelIndex &index);
    void removeItem(QAbstractItemView *view);
};

#endif // SEARCHDIALOG_H
