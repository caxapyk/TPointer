#include "paramdialog.h"
#include "ui_paramdialog.h"

#include "application.h"
#include "intdelegate.h"
#include "intlistdelegate.h"
#include "models/corpusmodel.h"
#include "models/storagemodel.h"

#include <QDebug>
#include <QModelIndexList>
#include <QMessageBox>
#include <QSettings>

ParamDialog::ParamDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ParamDialog)
{
    ui->setupUi(this);

    // model controller
    controller = new TableModelController();

    setupModels();
    setupControls();

    loadCorpuses();
    loadOther();

    // corpuses
    connect(cp_controls, &ButtonsControl::addRequested, this,  &ParamDialog::createItem);
    connect(cp_controls, &ButtonsControl::removeRequested, this,  &ParamDialog::removeItem);

    connect(ui->lV_corpuses->selectionModel(), &QItemSelectionModel::selectionChanged, this, &ParamDialog::selectCorpus);

    // storages
    connect(st_controls, &ButtonsControl::addRequested, this,  &ParamDialog::createItem);
    connect(st_controls, &ButtonsControl::removeRequested, this,  &ParamDialog::removeItem);
    connect(st_controls, &ButtonsControl::upRequested, this,  &ParamDialog::moveUp);
    connect(st_controls, &ButtonsControl::downRequested, this,  &ParamDialog::moveDown);

    // other triggers
    connect(ui->cB_blockFund, &QCheckBox::clicked, this,  &ParamDialog::saveBlockFund);

}

ParamDialog::~ParamDialog()
{
    delete ui;
    delete m_corpus_model;
    delete m_storage_model;
    delete cp_controls;
    delete st_controls;
    delete controller;
}

void ParamDialog::setupControls()
{
    cp_controls = new ButtonsControl(
                ButtonsControl::Add | ButtonsControl::Remove, Qt::Horizontal);

    st_controls = new ButtonsControl(
                ButtonsControl::Add | ButtonsControl::Remove | ButtonsControl::Up | ButtonsControl::Down);

    ui->gB_corpuses->layout()->addWidget(cp_controls);
    ui->gB_storages->layout()->addWidget(st_controls);
}

void ParamDialog::setupModels()
{
    m_corpus_model = new CorpusModel;
    m_storage_model = new StorageModel;
}

void ParamDialog::loadCorpuses() {
    m_corpus_model->select();

    ui->lV_corpuses->setModel(m_corpus_model);
    ui->lV_corpuses->setModelColumn(1);

    cp_controls->assetView(ui->lV_corpuses);
    cp_controls->setEnabled(true, ButtonsControl::Add);

    /* Load the first entry */
    if (m_corpus_model->rowCount() > 0) {
        QModelIndex index = m_corpus_model->index(0, 0);
        loadStorages(index.data());
    }
}

void ParamDialog::selectCorpus(const QItemSelection &selected, const QItemSelection&)
{
    if (!selected.indexes().isEmpty()) {
        QModelIndex current = selected.indexes().at(0);

        if (current.isValid()){

            QVariant id = current.sibling(current.row(), 0).data();
            loadStorages(id);
        }
    }
}

void ParamDialog::loadStorages(QVariant id)
{
    m_storage_model->setFilter("corpus=" + id.toString());
    m_storage_model->setParentId(id.toInt());

    if (m_storage_model->select()) {
        ui->tV_storages->setModel(m_storage_model);
        ui->tV_storages->setItemDelegateForColumn(4, new IntDelegate());
        ui->tV_storages->setItemDelegateForColumn(5, new IntListDelegate());

        ui->tV_storages->hideColumn(0);
        ui->tV_storages->hideColumn(1);
        ui->tV_storages->hideColumn(2);
        ui->tV_storages->hideColumn(6);

        ui->tV_storages->resizeColumnToContents(3);
        ui->tV_storages->setColumnWidth(4, 120);

        st_controls->assetView(ui->tV_storages);
        st_controls->setEnabled(true, ButtonsControl::Add);
    }
}

void ParamDialog::loadOther()
{
    application->applicationSettings()->beginGroup("Params");
    if(application->applicationSettings()->contains("blockFund")) {
        ui->cB_blockFund->setChecked(application->applicationSettings()->value("blockFund").toBool());
    }
    application->applicationSettings()->endGroup();
}

void ParamDialog::saveBlockFund()
{
    application->applicationSettings()->beginGroup("Params");
    application->applicationSettings()->setValue("blockFund",  ui->cB_blockFund->isChecked() ? 1 : 0);
    application->applicationSettings()->endGroup();
}

void ParamDialog::createItem(QAbstractItemView *view)
{
    if (!controller->createItem(view)) {
        QMessageBox::warning(this, tr("Parameters"), tr("Could not create item"), QMessageBox::Ok);
    }
}

void ParamDialog::removeItem(QAbstractItemView *view)
{
    int res = QMessageBox()
            .critical(this,
                      tr("Delete items"),
                      tr("Are you shure that you want to delete %1 item?").arg(view->selectionModel()->selectedRows().length()),
                      QMessageBox::No | QMessageBox::Yes);

    if (res == QMessageBox::Yes && !controller->removeItem(view)) {
        QMessageBox::warning(this, tr("Parameters"), tr("Could not remove item"), QMessageBox::Ok);
    }
}

void ParamDialog::moveUp(QAbstractItemView *view)
{
   controller->moveUp(view);
}

void ParamDialog::moveDown(QAbstractItemView *view)
{
    controller->moveDown(view);
}

