#include "nodedialog.h"
#include "addnodedialog.h"
#include "ui_nodedialog.h"

#include <QDebug>
#include <QMessageBox>
#include <QSqlRecord>

AddNodeDialog::AddNodeDialog(DataModel *model) : InsertNodeDialog()
{
    setWindowTitle(tr("Add record"));
    m_dataModel = model;
}

void AddNodeDialog::applyFilter(const FilterStruct &fs)
{
    // corpus
    if(fs.corpus.isValid()) {
        for (int i = 0; i < m_corpusModel->rowCount(); ++i) {
            if(m_corpusModel->index(i, 0).data() == fs.corpus) {
                ui->cB_corpus->setCurrentIndex(i + 1);
                ui->cB_corpus->setDisabled(true);
                break;
            }
        }
    }

    // storage
    if(fs.storage.isValid()) {
        for (int i = 0; i < m_storageModel->rowCount(); ++i) {
            if(m_storageModel->index(i, 0).data() == fs.storage) {
                ui->cB_storage->setCurrentIndex(i);
                ui->cB_storage->setDisabled(true);
                break;
            }
        }
    }
    // compartment
    if(fs.compartment.isValid()) {
        ui->lE_compartment->setText(fs.compartment.toString());
        ui->lE_compartment->setDisabled(true);
    }
    // shelving
    if(fs.shelving.isValid()) {
        ui->lE_shelving->setText(fs.shelving.toString());
        ui->lE_shelving->setDisabled(true);
    }
}

void AddNodeDialog::revert()
{
    int res = QMessageBox()
            .critical(this,
                      tr("Add record"),
                      tr("Are you shure that you want to revert changes?"),
                      QMessageBox::No | QMessageBox::Yes);

    if (res == QMessageBox::Yes) {
        ui->lE_cupboard->clear();
        ui->lE_shelf->clear();
        ui->cB_fund->setCurrentText(0);
        ui->lE_inventory->clear();
        ui->lE_records->clear();
        ui->cB_feature->setCurrentIndex(0);
    }
}

void AddNodeDialog::save()
{
    if (ui->cB_fund->currentIndex() > 0)    {
    QSqlRecord record = m_dataModel->record();
    record.remove(0);

    record.setValue("floor",  ui->cB_floor->currentText().remove(QRegExp("\\D+")));
    record.setValue("storage", m_storageModel->index(ui->cB_storage->currentIndex(), 0).data().toString());
    record.setValue("compartment", ui->lE_compartment->text());
    record.setValue("shelving", ui->lE_shelving->text());
    record.setValue("cupboard", ui->lE_cupboard->text());
    record.setValue("shelf", ui->lE_shelf->text());
    record.setValue("fund", m_fundModel->index(ui->cB_fund->currentIndex() -1, 0).data().toString());
    record.setValue("inventory", ui->lE_inventory->text());
    record.setValue("records", ui->lE_records->text());
    record.setValue("note", ui->tE_note->toPlainText());
    record.setValue("feature", m_featureModel->index(ui->cB_feature->currentIndex() -1, 0).data().toString());

    m_dataModel->primaryInsert(record);

    /*DataModel::Node node;
    node.append(QVariant());
    node.append(QVariant(ui->cB_corpus->currentText()));
    node.append(QVariant(ui->cB_corpus->currentText()));
    node.append(QVariant(ui->cB_corpus->currentText()));
    node.append(QVariant(ui->cB_corpus->currentText()));
    node.append(QVariant(ui->cB_corpus->currentText()));

    m_dataModel->setNode(node);*/

    } else {
        int res = QMessageBox()
                .critical(this,
                          tr("Add record"),
                          tr("Fill all required fields!"),
                          QMessageBox::Ok);
    }
}

