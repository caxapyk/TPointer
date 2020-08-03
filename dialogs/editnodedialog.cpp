#include "editnodedialog.h"
#include "ui_nodedialog.h"
#include "nodedelegate.h"

#include <QtDebug>

EditNodeDialog::EditNodeDialog(DataModel *model, int index) : NodeDialog()
{
    setWindowTitle(tr("Edit record"));

    setDataModel(model);

    mapper = new QDataWidgetMapper;

    mapper->setModel(dataModel());
    mapper->setItemDelegate(new NodeDelegate());

    mapper->addMapping(ui->cB_storage, 1);
    mapper->addMapping(ui->lE_compartment, 3);
    mapper->addMapping(ui->lE_shelving, 4);
    mapper->addMapping(ui->lE_cupboard, 5);
    mapper->addMapping(ui->lE_shelf, 6);
    mapper->addMapping(ui->cB_fund, 7);
    mapper->addMapping(ui->lE_inventory, 8);
    mapper->addMapping(ui->lE_records, 9);
    mapper->addMapping(ui->tE_note, 10);
    mapper->addMapping(ui->cB_feature, 11);

    mapper->setCurrentIndex(index);

    /* Disable static fields*/
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
