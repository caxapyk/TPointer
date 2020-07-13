#ifndef STRUCTUREDIALOG_H
#define STRUCTUREDIALOG_H

#include "models/basemodel.h"

#include <QButtonGroup>
#include <QDialog>
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
    BaseModel *m_corpus_model = nullptr;
    BaseModel *m_storage_model = nullptr;
    QButtonGroup *m_storage_controls;
    QVariant m_parent;

private slots:
    void selectCorpus(const QModelIndex &index);
    void createItem(QWidget *widget);
    void removeItem(QWidget *widget);
    void setControlsState(const QModelIndex &index);
};

#endif // STRUCTUREDIALOG_H
