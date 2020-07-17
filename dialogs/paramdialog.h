#ifndef PARAMDIALOG_H
#define PARAMDIALOG_H

#include "models/basemodel.h"

#include <QButtonGroup>
#include <QDialog>
#include <QModelIndex>
#include <QWidget>

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
    void loadCorpuses();
    void loadStorages(QVariant id);

    Ui::ParamDialog *ui;
    BaseModel *m_corpus_model;
    BaseModel *m_storage_model;
    QButtonGroup *m_storage_controls;

private slots:
    void createItem(QWidget *widget);
    void moveUp();
    void moveDown();
    void selectCorpus(const QModelIndex &index);
    void removeItem(QWidget *widget);
    void setControlsState(const QModelIndex&);
};

#endif // SEARCHDIALOG_H
