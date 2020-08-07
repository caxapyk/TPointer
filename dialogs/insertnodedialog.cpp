#include "insertnodedialog.h"
#include "ui_nodedialog.h"

#include <QMessageBox>

InsertNodeDialog::InsertNodeDialog() : NodeDialog()
{
    setWindowTitle(tr("Insert record"));
}

void InsertNodeDialog::revert()
{
    int res = QMessageBox()
            .critical(this,
                      tr("Insert record"),
                      tr("Are you shure that you want to revert changes?"),
                      QMessageBox::No | QMessageBox::Yes);

    if (res == QMessageBox::Yes) {
        ui->cB_corpus->setCurrentIndex(0); // auto clears storage and floor
        ui->lE_compartment->clear();
        ui->lE_shelving->clear();
        ui->lE_cupboard->clear();
        ui->lE_shelf->clear();
        ui->cB_fund->setCurrentIndex(0);
        ui->lE_inventory->clear();
        ui->lE_records->clear();
        ui->cB_feature->setCurrentIndex(0);
    }
}

void InsertNodeDialog::save()
{
    // save
}
