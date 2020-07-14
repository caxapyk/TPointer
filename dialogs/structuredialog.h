#ifndef STRUCTUREDIALOG_H
#define STRUCTUREDIALOG_H

#include "models/basemodel.h"

#include <QButtonGroup>
#include <QDialog>
#include <QModelIndex>
#include <QWidget>

namespace Ui {
class StructureDialog;
}

class StructureDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StructureDialog(QWidget *parent = nullptr);
    ~StructureDialog();

private:
    void loadCorpuses();
    void loadStorages(QVariant id);

    Ui::StructureDialog *ui;
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

#endif // STRUCTUREDIALOG_H
