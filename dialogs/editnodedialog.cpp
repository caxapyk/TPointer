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
    mapper->addMapping(ui->cB_floor, DataModel::Floor);
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
    QSqlRecord record = dataModel()->record();

    record.setValue("id",  dataModel()->index(mapper->currentIndex(), DataModel::Id).data());

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

    bool res = dataModel()->primaryUpdate(record);

    if(res) {
        // set empty index before mapper sumbitted if current index = 0
        if(ui->cB_feature->currentIndex() == 0) {
            ui->cB_feature->setCurrentIndex(-1);
        }

        mapper->submit();
    }  else {
        QMessageBox()
                .critical(this,
                          tr("Edit record"),
                          tr("Can not edit record!"),
                          QMessageBox::Ok);
    }
}
