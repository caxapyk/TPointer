#include "nodedialog.h"
#include "addnodedialog.h"
#include "models/filterstruct.h"
#include "ui_nodedialog.h"

#include <QDebug>

AddNodeDialog::AddNodeDialog() : InsertNodeDialog()
{
    setWindowTitle(tr("Add record"));
}

void AddNodeDialog::applyFilter(const FilterStruct &fs)
{
    // storage
    if(fs.storage.isValid()) {
        for (int i = 0; i < m_nILstorageModel->rowCount(); ++i) {
            if(m_storageModel->index(i, 0).data() == fs.storage) {
                ui->cB_storage->setCurrentIndex(i + 1);
                ui->cB_storage->setDisabled(true);
                break;
            }
        }
    }
    // floor - disable floor if count = 1
    if (m_floorModel->rowCount() == 1) {
        ui->cB_floor->setDisabled(true);
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
    // fund
    if(fs.fund.isValid()) {
        for (int i = 0; i < m_nILfundModel->rowCount(); ++i) {
            if(m_fundModel->index(i, 0).data() == fs.fund) {
                ui->cB_fund->setCurrentIndex(i + 1);
                ui->cB_fund->setDisabled(true);
                ui->pB_openFundList->setDisabled(true);
                break;
            }
        }
    }
}
