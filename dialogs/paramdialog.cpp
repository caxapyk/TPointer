#include "paramdialog.h"
#include "ui_paramdialog.h"
#include "intdelegate.h"
#include "intlistdelegate.h"
#include "models/corpusmodel.h"
#include "models/storagemodel.h"
#include "models/featuremodel.h"

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

    setupControls();
    setupModels();
    loadCorpuses();
    loadFeatures();

    /* triggers for placement tab */

    // corpuses
    connect(ui->pB_corpusAdd, &QPushButton::released, [=] {
        createItem(ui->lV_corpuses);
    });
    connect(ui->pB_corpusRemove, &QPushButton::released, [=] {
        removeItem(ui->lV_corpuses);
    });

    connect(ui->lV_corpuses, &QListView::clicked, this, &ParamDialog::selectCorpus);

    // storages
    connect(storage_controls->buttonAdd(), &QPushButton::released, [=] {
        createItem(ui->tV_storages);
    });
    connect(storage_controls->buttonRemove(), &QPushButton::released, [=] {
        removeItem(ui->tV_storages);
    });
    connect(storage_controls->buttonUp(), &QPushButton::released, this, [=] {
        moveUp(ui->tV_storages);
    });
    connect(storage_controls->buttonDown(), &QPushButton::released, this, [=] {
        moveDown(ui->tV_storages);
    });

    connect(ui->tV_storages, &QListView::clicked, storage_controls, &ButtonControls::modelSelected);

    /* triggers for other tab */

    // features
    connect(feature_controls->buttonAdd(), &QPushButton::released, [=] {
        createItem(ui->tV_features);
    });
    connect(feature_controls->buttonRemove(), &QPushButton::released, [=] {
        removeItem(ui->tV_features);
    });
    connect(feature_controls->buttonUp(), &QPushButton::released, this, [=] {
        moveUp(ui->tV_features);
    });
    connect(feature_controls->buttonDown(), &QPushButton::released, this, [=] {
        moveDown(ui->tV_features);
    });

    connect(ui->tV_features, &QListView::clicked, feature_controls, &ButtonControls::modelSelected);
}

ParamDialog::~ParamDialog()
{
    delete ui;
    delete m_corpus_model;
    delete m_storage_model;
    delete m_feature_model;

    delete storage_controls;
    delete feature_controls;
}

void ParamDialog::setupControls()
{
    storage_controls = new ButtonControls;
    feature_controls = new ButtonControls;

    ui->gB_storages->layout()->addWidget(storage_controls);
    ui->gB_features->layout()->addWidget(feature_controls);
}

void ParamDialog::setupModels()
{
    m_corpus_model = new CorpusModel(this);
    m_storage_model = new StorageModel(this);
    m_feature_model = new FeatureModel(this);
}

void ParamDialog::loadCorpuses() {
    m_corpus_model->select();

    ui->lV_corpuses->setModel(m_corpus_model);
    ui->lV_corpuses->setModelColumn(1);

    /* Load the first entry */
    if (m_corpus_model->rowCount() > 0) {
        QModelIndex index = m_corpus_model->index(0, 0);
        loadStorages(index.data());
    }
}

void ParamDialog::selectCorpus(const QModelIndex &index)
{
    if (index.isValid()){
        storage_controls->disable();

        QVariant id = index.sibling(index.row(), 0).data();
        loadStorages(id);
    }
}

void ParamDialog::loadStorages(QVariant id)
{
    m_storage_model->setFilter("corpus_id=" + id.toString());
    m_storage_model->setParentId(id.toInt());
    m_storage_model->select();

    ui->tV_storages->setModel(m_storage_model);
    ui->tV_storages->setItemDelegateForColumn(4, new IntDelegate());
    ui->tV_storages->setItemDelegateForColumn(5, new IntListDelegate());

    ui->tV_storages->hideColumn(0);
    ui->tV_storages->hideColumn(1);
    ui->tV_storages->hideColumn(2);
    ui->tV_storages->hideColumn(6);

    ui->tV_storages->setColumnWidth(3, 200);
    ui->tV_storages->setColumnWidth(4, 90);
    ui->tV_storages->setColumnWidth(5, 90);
}

void ParamDialog::loadFeatures() {
    m_feature_model->select();

    m_feature_model->setHeaderData(2, Qt::Horizontal, tr("Name"));

    ui->tV_features->setModel(m_feature_model);

    ui->tV_features->hideColumn(0);
    ui->tV_features->hideColumn(1);
}

void ParamDialog::createItem(QAbstractItemView *view)
{
    QAbstractItemView *v = qobject_cast<QAbstractItemView*> (view);
    BaseModel *model = qobject_cast<BaseModel*> (v->model());

    if (model && model->insert()) {
        QModelIndex index = model->index(model->rowCount() - 1, 1);
        v->setCurrentIndex(index);
    } else {
        QMessageBox::warning(this, tr("Storage structure"), tr("Could not create item"), QMessageBox::Ok);
    }
}

void ParamDialog::removeItem(QAbstractItemView *view)
{
    QAbstractItemView *v = qobject_cast<QAbstractItemView*> (view);
    BaseModel *model = qobject_cast<BaseModel*> (v->model());

    if(model) {
        QModelIndexList indexes = v->selectionModel()->selectedIndexes();
        if (!model->remove(indexes)) {
            QMessageBox::warning(this, tr("Storage structure"), tr("Could not remove item"), QMessageBox::Ok);
        }
    }
}

void ParamDialog::moveUp(QAbstractItemView *view)
{
    QAbstractItemView *v = qobject_cast<QAbstractItemView*> (view);
    BaseModel *model = qobject_cast<BaseModel*> (v->model());

    QModelIndex index = v->currentIndex();
    QModelIndex up_index = model->moveUp(index.row());

    if (up_index.isValid()) {
        v->setCurrentIndex(up_index);
    }
}

void ParamDialog::moveDown(QAbstractItemView *view)
{
    QAbstractItemView *v = qobject_cast<QAbstractItemView*> (view);
    BaseModel *model = qobject_cast<BaseModel*> (v->model());

    QModelIndex index = v->currentIndex();
    QModelIndex down_index = model->moveDown(index.row());

    if (down_index.isValid()) {
        v->setCurrentIndex(down_index);
    }
}
