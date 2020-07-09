#include "structuredialog.h"
#include "ui_structuredialog.h"

#include <QDebug>
#include <QModelIndex>
#include <QSqlRelationalDelegate>
#include <QSqlTableModel>
#include <QSqlRelationalTableModel>
#include <QSqlRelation>
#include <QTableView>
#include <QVariant>

StructureDialog::StructureDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StructureDialog)
{
    ui->setupUi(this);
    setupModels();

    connect(ui->lV_corpuses, SIGNAL(clicked(const QModelIndex &)),
            this, SLOT(selectCorpus(const QModelIndex&)));

    connect(ui->pB_corpusAdd, SIGNAL(clicked()),
            this, SLOT(addCorpus()));
    connect(ui->pB_corpusRemove, SIGNAL(clicked()),
            this, SLOT(removeCorpus()));
}

void StructureDialog::setupModels() {
    m_corpus_model = new QSqlTableModel;
    m_corpus_model->setTable("corpus");
    m_corpus_model->setEditStrategy(QSqlTableModel::OnFieldChange);
    m_corpus_model->select();

    ui->lV_corpuses->setModel(m_corpus_model);
    ui->lV_corpuses->setModelColumn(m_corpus_model->fieldIndex("name"));
}

void StructureDialog::selectCorpus(const QModelIndex &index)
{
    QVariant id = index.sibling(index.row(), 0).data();
    loadStorages(id);
}

void StructureDialog::addCorpus()
{
    m_corpus_model->insertRows(m_corpus_model->rowCount(), 1);

    QModelIndex index = m_corpus_model->index(m_corpus_model->rowCount() - 1, m_corpus_model->fieldIndex("name"));
    ui->lV_corpuses->setCurrentIndex(index);

    m_corpus_model->setData(index, tr("New corpus"));
    m_corpus_model->submit();
}

void StructureDialog::removeCorpus()
{
    QModelIndex index =  ui->lV_corpuses->currentIndex();

    if (index.isValid()) {
        m_corpus_model->removeRows(index.row(), 1);
        m_corpus_model->select();
    }
}

void StructureDialog::loadStorages(QVariant id)
{
    m_storage_model = new QSqlRelationalTableModel;
    m_storage_model->setTable("storage");
    m_storage_model->setFilter("corpus_id=" + id.toString());
    m_storage_model->setRelation(m_storage_model->fieldIndex("corpus_id"),
                                 QSqlRelation("corpus", "id", "name"));

    m_storage_model->setEditStrategy(QSqlRelationalTableModel::OnFieldChange);

    m_storage_model->select();

    ui->tV_storages->setModel(m_storage_model);
    ui->tV_storages->setItemDelegate(new QSqlRelationalDelegate(ui->tV_storages));
}

StructureDialog::~StructureDialog()
{
    delete ui;
    delete m_corpus_model;
    delete m_storage_model;
}
