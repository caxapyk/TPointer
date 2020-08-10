#include "editnodedialog.h"
#include "ui_nodedialog.h"
#include "nodedelegate.h"

#include <QDebug>
#include <QMessageBox>

EditNodeDialog::EditNodeDialog(DataModel *model, int index) : NodeDialog()
{
    setWindowTitle(tr("Edit record"));

    setDataModel(model);

    mapper = new QDataWidgetMapper;
    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);

    mapper->setModel(dataModel());
    mapper->setItemDelegate(new NodeDelegate());

    mapper->addMapping(ui->cB_corpus, DataModel::Corpus);
    mapper->addMapping(ui->cB_storage, DataModel::Storage);
    mapper->addMapping(ui->sB_compartment, DataModel::Compartment);
    mapper->addMapping(ui->sB_shelving, DataModel::Shelving);
    mapper->addMapping(ui->lE_cupboard, DataModel::Cupboard);
    mapper->addMapping(ui->lE_shelf, DataModel::Shelf);
    mapper->addMapping(ui->cB_fund, DataModel::Fund);
    mapper->addMapping(ui->lE_inventory, DataModel::Inventory);
    mapper->addMapping(ui->lE_records, DataModel::Records);
    mapper->addMapping(ui->tE_note, DataModel::Note);
    mapper->addMapping(ui->cB_feature, DataModel::Features);

    mapper->setCurrentIndex(index);

    /* Disable static fields*/
    ui->cB_corpus->setDisabled(true);
    ui->cB_storage->setDisabled(true);
    // disable floor if count = 1
    if (m_floorModel->rowCount() == 1) {
        ui->cB_floor->setDisabled(true);
    }
    ui->sB_compartment->setDisabled(true);
    ui->sB_shelving->setDisabled(true);
    ui->cB_fund->setDisabled(true);
    ui->pB_openFundList->setDisabled(true);
}

EditNodeDialog::~EditNodeDialog()
{
    delete mapper;
}

void EditNodeDialog::revert()
{
    int res = QMessageBox()
            .critical(this,
                      tr("Edit record"),
                      tr("Are you shure that you want to revert changes?"),
                      QMessageBox::No | QMessageBox::Yes);

    if (res == QMessageBox::Yes) {
        mapper->revert();
    }
}

void EditNodeDialog::save()
{
    if (ui->cB_fund->currentIndex() > 0)    {
        QSqlRecord record = m_dataModel->record();

        record.setValue("id",  m_dataModel->index(mapper->currentIndex(), DataModel::Id).data());
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

        QVariant res = m_dataModel->primaryUpdate(record);

        if(res.isValid()) {
            // set empty index before mapper sumbitted if current index = 0
            if(ui->cB_feature->currentIndex() == 0) {
                ui->cB_feature->setCurrentIndex(-1);
            }

            mapper->submit();
        }

    } else {
        QMessageBox()
                .critical(this,
                          tr("Add record"),
                          tr("Fill all required fields!"),
                          QMessageBox::Ok);
    }
}
