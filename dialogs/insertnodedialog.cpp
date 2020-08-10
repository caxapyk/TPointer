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

        record.setValue("floor",  ui->cB_floor->currentText().remove(QRegExp("\\D+")));
        record.setValue("storage", m_storageModel->index(ui->cB_storage->currentIndex(), 0).data().toString());
        record.setValue("compartment", QVariant(ui->sB_compartment->value()));
        record.setValue("shelving",  QVariant(ui->sB_shelving->value()));
        record.setValue("cupboard", ui->lE_cupboard->text());
        record.setValue("shelf", ui->lE_shelf->text());
        record.setValue("fund", m_fundModel->index(ui->cB_fund->currentIndex() -1, 0).data().toString());
        record.setValue("inventory", ui->lE_inventory->text());
        record.setValue("records", ui->lE_records->text());
        record.setValue("note", ui->tE_note->toPlainText());
        record.setValue("feature", m_featureModel->index(ui->cB_feature->currentIndex() -1, 0).data().toString());

        QVariant res = dataModel()->primaryInsert(record);

        if(res.isValid()) {
            // set empty index before mapper sumbitted if current index = 0
            if(ui->cB_feature->currentIndex() == 0) {
                ui->cB_feature->setCurrentIndex(-1);
            }

            DataModel::Node node;
            node.append(res);
            node.append(QVariant(ui->cB_corpus->currentText()));
            node.append(QVariant(ui->cB_storage->currentText()));
            node.append(QVariant(ui->cB_floor->currentText().remove(QRegExp("\\D+"))));
            node.append(QVariant(ui->sB_compartment->text()));
            node.append(QVariant(ui->sB_shelving->text()));
            node.append(QVariant(ui->lE_cupboard->text()));
            node.append(QVariant(ui->lE_shelf->text()));
            node.append(QVariant(ui->cB_fund->currentText()));
            node.append(QVariant(ui->lE_inventory->text()));
            node.append(QVariant(ui->lE_records->text()));
            node.append(QVariant(ui->tE_note->toPlainText()));
            node.append(QVariant(ui->cB_feature->currentText()));

            dataModel()->setNode(node);
        }

    } else {
        QMessageBox()
                .critical(this,
                          tr("Add record"),
                          tr("Fill all required fields!"),
                          QMessageBox::Ok);
    }
}
