#ifndef STRUCTUREDIALOG_H
#define STRUCTUREDIALOG_H

#include <QDialog>
#include <QSqlTableModel>
#include <QSqlRelationalTableModel>

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
    void setupModels();
    void loadStorages(QVariant id);

    Ui::StructureDialog *ui;
    QSqlTableModel *m_corpus_model = nullptr;
    QSqlRelationalTableModel *m_storage_model = nullptr;

private slots:
    void selectCorpus(const QModelIndex &index);
    void addCorpus();
    void removeCorpus();
};

#endif // STRUCTUREDIALOG_H
