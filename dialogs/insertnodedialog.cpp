#include "insertnodedialog.h"
#include "ui_nodedialog.h"
#include "models/datamodel.h"

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
        ui->sB_compartment->clear();
        ui->sB_shelving->clear();
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
    // note that other required fields are filled automaticly thats why no need to check its
    if (ui->cB_corpus->currentIndex() > 0 && ui->cB_fund->currentIndex() > 0)    {
        QSqlRecord record = dataModel()->record();
        record.remove(0);

        // check basement
        QVariant floor = ui->cB_floor->currentText().contains(QRegExp("\\d+")) ? QVariant(ui->cB_floor->currentText()) : QVariant(0);
        record.setValue("floor",  floor);
        record.setGenerated("floor", true);

        record.setValue("storage", m_storageModel->index(ui->cB_storage->currentIndex(), 0).data());
        record.setGenerated("storage", true);

        record.setValue("compartment", QVariant(ui->sB_compartment->value()));
        record.setGenerated("compartment", true);

        record.setValue("shelving",  QVariant(ui->sB_shelving->value()));
        record.setGenerated("shelving", true);

        record.setValue("cupboard", QVariant(ui->lE_cupboard->text()));
        record.setGenerated("cupboard", true);

        record.setValue("shelf", QVariant(ui->lE_shelf->text()));
        record.setGenerated("shelf", true);

        record.setValue("fund", m_fundModel->index(ui->cB_fund->currentIndex() -1, 0).data());
        record.setGenerated("fund", true);

        record.setValue("inventory", QVariant(ui->lE_inventory->text()));
        record.setGenerated("inventory", true);

        record.setValue("records", QVariant(ui->lE_records->text()));
        record.setGenerated("records", true);

        record.setValue("note", QVariant(ui->tE_note->toPlainText()));
        record.setGenerated("note", true);

        record.setValue("feature", m_featureModel->index(ui->cB_feature->currentIndex() -1, 0).data());
        record.setGenerated("feature", true);

        QVariant res = dataModel()->primaryInsert(record);

        if(res.isValid() && m_dataModel->insertRows(m_dataModel->rowCount(), 1)) {
            // set empty index before mapper sumbitted if current index = 0
            if(ui->cB_feature->currentIndex() == 0) {
                ui->cB_feature->setCurrentIndex(-1);
            }

            int row = m_dataModel->rowCount() - 1;

            m_dataModel->setData(m_dataModel->index(row, 0), res);
            m_dataModel->setData(m_dataModel->index(row, 1), QVariant(ui->cB_corpus->currentText()));
            m_dataModel->setData(m_dataModel->index(row, 2), QVariant(ui->cB_storage->currentText()));
            m_dataModel->setData(m_dataModel->index(row, 3), QVariant(ui->cB_floor->currentText().remove(QRegExp("\\D+"))));
            m_dataModel->setData(m_dataModel->index(row, 4), QVariant(ui->sB_compartment->text()));
            m_dataModel->setData(m_dataModel->index(row, 5), QVariant(ui->sB_shelving->text()));
            m_dataModel->setData(m_dataModel->index(row, 6), QVariant(ui->lE_cupboard->text()));
            m_dataModel->setData(m_dataModel->index(row, 7), QVariant(ui->lE_shelf->text()));
            m_dataModel->setData(m_dataModel->index(row, 8), QVariant(ui->cB_fund->currentText()));
            m_dataModel->setData(m_dataModel->index(row, 9), QVariant(ui->lE_inventory->text()));
            m_dataModel->setData(m_dataModel->index(row, 10), QVariant(ui->lE_records->text()));
            m_dataModel->setData(m_dataModel->index(row, 11), QVariant(ui->tE_note->toPlainText()));
            m_dataModel->setData(m_dataModel->index(row, 12), QVariant(ui->cB_feature->currentText()));
        } else {
            QMessageBox()
                    .critical(this,
                              tr("Add record"),
                              tr("Can not add record!"),
                              QMessageBox::Ok);
        }

    } else {
        QMessageBox()
                .critical(this,
                          tr("Add record"),
                          tr("Fill all required fields!"),
                          QMessageBox::Ok);
    }
}
