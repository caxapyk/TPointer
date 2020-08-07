#include "nodedialog.h"
#include "addnodedialog.h"
#include "ui_nodedialog.h"

#include <QDebug>
#include <QMessageBox>

AddNodeDialog::AddNodeDialog() : InsertNodeDialog()
{
    setWindowTitle(tr("Add record"));
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
    // save
}

