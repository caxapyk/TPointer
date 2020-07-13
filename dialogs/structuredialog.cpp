#include "structuredialog.h"
#include "ui_structuredialog.h"
#include "commalistdelegate.h"
#include "models/corpusmodel.h"
#include "models/storagemodel.h"

#include <QAbstractItemModel>
#include <QDebug>
#include <QMessageBox>
#include <QModelIndex>
#include <QModelIndexList>
#include <QSqlError>
#include <QSqlRelationalDelegate>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QSqlRelation>
#include <QTableView>
#include <QVariant>

#include <QAbstractItemView>
#include <QPushButton>
#include <QListView>

StructureDialog::StructureDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StructureDialog)
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

    connect(ui->lV_corpuses, &QListView::clicked, this, &StructureDialog::selectCorpus);
    connect(ui->tV_storages, &QListView::clicked, this, &StructureDialog::setControlsState);
}

void StructureDialog::loadCorpuses() {
    m_corpus_model->select();

    ui->lV_corpuses->setModel(m_corpus_model);
    ui->lV_corpuses->setModelColumn(1);
}

void StructureDialog::selectCorpus(const QModelIndex &index)
{
    if (index.isValid()){
        QVariant id = index.sibling(index.row(), 0).data();
        loadStorages(id);
    }
}

void StructureDialog::loadStorages(QVariant id)
{
    m_storage_model->setParentId(1, id);
    m_storage_model->select();

    m_storage_model->setHeaderData(2, Qt::Horizontal, tr("Name"));
    m_storage_model->setHeaderData(3, Qt::Horizontal, tr("Rooms count"));
    m_storage_model->setHeaderData(4, Qt::Horizontal, tr("Floors"));

    ui->tV_storages->setModel(m_storage_model);
    ui->tV_storages->setItemDelegateForColumn(4, new CommaListDelegate());

    ui->tV_storages->hideColumn(0);
    ui->tV_storages->hideColumn(1);
    ui->tV_storages->setColumnWidth(2, 200);
    ui->tV_storages->setColumnWidth(3, 90);
    ui->tV_storages->setColumnWidth(4, 90);

    m_storage_controls->addButton(ui->pB_storageAdd);
    m_storage_controls->setId(ui->pB_storageAdd, 0);
    m_storage_controls->addButton(ui->pB_storageRemove);
    m_storage_controls->setId(ui->pB_storageRemove, 1);
    m_storage_controls->addButton(ui->pB_storageUp);
    m_storage_controls->setId(ui->pB_storageUp, 2);
    m_storage_controls->addButton(ui->pB_storageDown);
    m_storage_controls->setId(ui->pB_storageDown, 3);

    m_storage_controls->button(0)->setEnabled(true);

    setControlsState(ui->tV_storages->currentIndex());

}

void StructureDialog::createItem(QWidget *widget)
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

void StructureDialog::removeItem(QWidget *widget)
{
    QAbstractItemView *view = qobject_cast<QAbstractItemView*> (widget);
    BaseModel *model = qobject_cast<BaseModel*> (view->model());

    QModelIndexList indexes = view->selectionModel()->selectedIndexes();
    if (!model->remove(indexes)) {
        QMessageBox::warning(this, tr("Storage structure"), tr("Could not remove item"), QMessageBox::Ok);
    }
}

// сделать общие контролы
void StructureDialog::setControlsState(const QModelIndex &index)
{
    for (int i = 1; i < 4; ++i) {
        m_storage_controls->button(i)->setEnabled(index.isValid());
    }
}

StructureDialog::~StructureDialog()
{
    delete ui;
    delete m_corpus_model;
    delete m_storage_model;
    delete m_storage_controls;
}
