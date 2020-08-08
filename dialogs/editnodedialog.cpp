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
    mapper->addMapping(ui->lE_compartment, DataModel::Compartment);
    mapper->addMapping(ui->lE_shelving, DataModel::Shelving);
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
    ui->lE_compartment->setDisabled(true);
    ui->lE_shelving->setDisabled(true);
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
    // set empty index before mapper sumbitted if current index = 0
    if(ui->cB_feature->currentIndex() == 0) {
        ui->cB_feature->setCurrentIndex(-1);
    }

    mapper->submit();
}
