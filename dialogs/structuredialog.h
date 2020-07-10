#ifndef STRUCTUREDIALOG_H
#define STRUCTUREDIALOG_H

#include <QDialog>
#include <QSqlTableModel>
#include <QButtonGroup>

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
    QSqlTableModel *m_corpus_model = nullptr;
    QSqlTableModel *m_storage_model = nullptr;

    QButtonGroup *m_storage_controls;

    QVariant m_parent;


private slots:
    void selectCorpus(const QModelIndex &index);
    void addCorpus();
    void removeCorpus();
    void addStorage();
    void setStorageControlsState(const QModelIndex &index);
    void saveStorage();
    void initDefaultStorageRow(int row, QSqlRecord &record);
};

#endif // STRUCTUREDIALOG_H
