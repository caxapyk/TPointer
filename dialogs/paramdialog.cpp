#include "paramdialog.h"
#include "ui_paramdialog.h"
#include "commalistdelegate.h"
#include "models/corpusmodel.h"
#include "models/storagemodel.h"

#include <QDebug>
#include <QAbstractItemView>
#include <QListView>
#include <QMessageBox>
#include <QPushButton>
#include <QModelIndexList>
#include <QSqlRelationalDelegate>
#include <QVariant>

ParamDialog::ParamDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ParamDialog)
{
    ui->setupUi(this);

    m_storage_controls = new QButtonGroup();

    /* Initialize models */
    m_corpus_model = new CorpusModel(this);
    m_storage_model = new StorageModel(this);

    loadCorpuses();

    connect(ui->pB_corpusAdd, &QPushButton::released, [=] {
        createItem(ui->lV_corpuses);
    });
    connect(ui->pB_corpusRemove, &QPushButton::released, [=] {
        removeItem(ui->lV_corpuses);
    });
    connect(ui->pB_storageAdd, &QPushButton::released, [=] {
        createItem(ui->tV_storages);
    });
    connect(ui->pB_storageRemove, &QPushButton::released, [=] {
        removeItem(ui->tV_storages);
    });

    connect(ui->pB_storageUp, &QPushButton::released, this, &ParamDialog::moveUp);
    connect(ui->pB_storageDown, &QPushButton::released, this, &ParamDialog::moveDown);

    connect(ui->lV_corpuses, &QListView::clicked, this, &ParamDialog::selectCorpus);
    connect(ui->lV_corpuses, &QListView::clicked, this, &ParamDialog::setControlsState);
    connect(ui->tV_storages, &QListView::clicked, this, &ParamDialog::setControlsState);
}

void ParamDialog::loadCorpuses() {
    m_corpus_model->select();

    ui->lV_corpuses->setModel(m_corpus_model);
    ui->lV_corpuses->setModelColumn(1);

    /* Load the first entry */
    QModelIndex index = m_corpus_model->index(0, 0);
    loadStorages(index.data());

}

void ParamDialog::selectCorpus(const QModelIndex &index)
{
    if (index.isValid()){
        QVariant id = index.sibling(index.row(), 0).data();
        loadStorages(id);
    }
}

void ParamDialog::loadStorages(QVariant id)
{
    m_storage_model->setParentId(1, id);
    m_storage_model->select();

    m_storage_model->setHeaderData(3, Qt::Horizontal, tr("Name"));
    m_storage_model->setHeaderData(4, Qt::Horizontal, tr("Rooms count"));
    m_storage_model->setHeaderData(5, Qt::Horizontal, tr("Floors"));

    ui->tV_storages->setModel(m_storage_model);
    ui->tV_storages->setItemDelegateForColumn(5, new CommaListDelegate());

    ui->tV_storages->hideColumn(0);
    ui->tV_storages->hideColumn(1);
    ui->tV_storages->hideColumn(2);
    ui->tV_storages->setColumnWidth(3, 200);
    ui->tV_storages->setColumnWidth(4, 90);
    ui->tV_storages->setColumnWidth(5, 90);

    m_storage_controls->addButton(ui->pB_storageAdd);
    m_storage_controls->setId(ui->pB_storageAdd, 0);
    m_storage_controls->addButton(ui->pB_storageRemove);
    m_storage_controls->setId(ui->pB_storageRemove, 1);
    m_storage_controls->addButton(ui->pB_storageUp);
    m_storage_controls->setId(ui->pB_storageUp, 2);
    m_storage_controls->addButton(ui->pB_storageDown);
    m_storage_controls->setId(ui->pB_storageDown, 3);
}

void ParamDialog::createItem(QWidget *widget)
{
    QAbstractItemView *view = qobject_cast<QAbstractItemView*> (widget);
    BaseModel *model = qobject_cast<BaseModel*> (view->model());

    if (model->insert()) {
        QModelIndex index = model->index(model->rowCount() - 1, 1);
        view->setCurrentIndex(index);
    } else {
        QMessageBox::warning(this, tr("Storage structure"), tr("Could not create item"), QMessageBox::Ok);
    }
}

void ParamDialog::moveUp()
{
    QModelIndex index = ui->tV_storages->currentIndex();
    QModelIndex up_index = m_storage_model->moveUp(index.row(), 2);

    if (up_index.isValid()) {
        ui->tV_storages->setCurrentIndex(up_index);
    }
}

void ParamDialog::moveDown()
{
    QModelIndex index = ui->tV_storages->currentIndex();
    QModelIndex down_index = m_storage_model->moveDown(index.row(), 2);

    if (down_index.isValid()) {
        ui->tV_storages->setCurrentIndex(down_index);
    }
}

void ParamDialog::removeItem(QWidget *widget)
{
    QAbstractItemView *view = qobject_cast<QAbstractItemView*> (widget);
    BaseModel *model = qobject_cast<BaseModel*> (view->model());

    QModelIndexList indexes = view->selectionModel()->selectedIndexes();
    if (!model->remove(indexes)) {
        QMessageBox::warning(this, tr("Storage structure"), tr("Could not remove item"), QMessageBox::Ok);
    }
}

void ParamDialog::setControlsState(const QModelIndex&)
{
    bool isValid = ui->tV_storages->currentIndex().isValid();
    for (int i = 1; i < 4; ++i) {
        m_storage_controls->button(i)->setEnabled(isValid);
    }
}

ParamDialog::~ParamDialog()
{
    delete ui;
    delete m_corpus_model;
    delete m_storage_model;
    delete m_storage_controls;
}
