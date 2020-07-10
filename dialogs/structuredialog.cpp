#include "structuredialog.h"
#include "ui_structuredialog.h"
#include "commalistdelegate.h"

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

StructureDialog::StructureDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StructureDialog)
{
    ui->setupUi(this);
    loadCorpuses();

    connect(ui->lV_corpuses, SIGNAL(clicked(const QModelIndex &)),
            this, SLOT(selectCorpus(const QModelIndex&)));

    connect(ui->pB_corpusAdd, SIGNAL(clicked()),
            this, SLOT(addCorpus()));
    connect(ui->pB_corpusRemove, SIGNAL(clicked()),
            this, SLOT(removeCorpus()));

    connect(ui->tV_storages, SIGNAL(clicked(const QModelIndex &)),
            this, SLOT(setStorageControlsState(const QModelIndex&)));

    connect(ui->pB_storageAdd, SIGNAL(clicked()),
            this, SLOT(addStorage()));

    connect(ui->pB_storageUp, SIGNAL(clicked()),
            this, SLOT(saveStorage()));
}

void StructureDialog::loadCorpuses() {
    m_corpus_model = new QSqlTableModel;
    m_corpus_model->setTable("corpus");
    m_corpus_model->setEditStrategy(QSqlTableModel::OnFieldChange);
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

void StructureDialog::addCorpus()
{
    if (m_corpus_model->insertRows(m_corpus_model->rowCount(), 1)) {
        QModelIndex index = m_corpus_model->index(m_corpus_model->rowCount() - 1, 1);
        ui->lV_corpuses->setCurrentIndex(index);

        m_corpus_model->setData(index, tr("New corpus"));
        m_corpus_model->submit();
    } else {
        m_corpus_model->select();
        QMessageBox::warning(this, tr("Corpuses"), tr("Could not create corpus"), QMessageBox::Ok);
    }
}

void StructureDialog::removeCorpus()
{
    QModelIndex index =  ui->lV_corpuses->currentIndex();

    if (index.isValid()) {
        if (m_corpus_model->removeRows(index.row(), 1)) {
            m_corpus_model->select();
        } else {
            m_corpus_model->select();
            QMessageBox::warning(this, tr("Corpuses"), tr("Could not remove corpus"), QMessageBox::Ok);
        }
    }
}

void StructureDialog::loadStorages(QVariant id)
{
    m_parent = id; //set current parent id

    m_storage_model = new QSqlTableModel;
    m_storage_model->setTable("storage");
    m_storage_model->setFilter("corpus_id=" + id.toString());
    m_storage_model->setEditStrategy(QSqlTableModel::OnFieldChange);

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

    m_storage_controls = new QButtonGroup();
    m_storage_controls->addButton(ui->pB_storageAdd);
    m_storage_controls->setId(ui->pB_storageAdd, 0);
    m_storage_controls->addButton(ui->pB_storageRemove);
    m_storage_controls->setId(ui->pB_storageRemove, 1);
    m_storage_controls->addButton(ui->pB_storageUp);
    m_storage_controls->setId(ui->pB_storageUp, 2);
    m_storage_controls->addButton(ui->pB_storageDown);
    m_storage_controls->setId(ui->pB_storageDown, 3);

    m_storage_controls->button(0)->setEnabled(true);

    setStorageControlsState(ui->tV_storages->currentIndex());

    //test it многократное присваивание!
    connect(m_storage_model, SIGNAL(primeInsert(int, QSqlRecord&)),
            this, SLOT(initDefaultStorageRow(int, QSqlRecord&)));
}

void StructureDialog::addStorage()
{
    m_storage_model->insertRows(m_storage_model->rowCount(), 1);
    m_storage_model->submit();
    qDebug() << m_storage_model->lastError().text();

    //QModelIndex index = m_storage_model->index(m_corpus_model->rowCount() - 1, 1);
    //ui->tV_storages->setCurrentIndex(index);
}

void StructureDialog::saveStorage()
{
    //qDebug() << m_storage_model->lastError().text();
}

void StructureDialog::setStorageControlsState(const QModelIndex &index)
{
    m_storage_model->submitAll();
    for (int i = 1; i < 4; ++i) {
        m_storage_controls->button(i)->setEnabled(index.isValid());
    }
}

void StructureDialog::initDefaultStorageRow(int, QSqlRecord &record)
{
    //qDebug() << m_parent;
    //record.remove(0); //remove id
    record.setValue(1, m_parent);
    record.setGenerated(1, true);
    //record.setValue(2, QVariant(tr("New storage")));
    //record.setGenerated(2, true);

    m_storage_model->submit();
}

StructureDialog::~StructureDialog()
{
    delete ui;
    delete m_corpus_model;
    delete m_storage_model;
    delete m_storage_controls;
}
