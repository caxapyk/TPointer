#include "movenodedialog.h"
#include "ui_movenodedialog.h"
#include "utils/floorsparser.h"

#include <QPushButton>
#include <QSqlRecord>
#include <QDebug>
#include <QMessageBox>

MoveNodeDialog::MoveNodeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MoveNodeDialog)
{
    ui->setupUi(this);

    setupModels();

    connect(ui->cB_corpus, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MoveNodeDialog::fillStorage);
    connect(ui->cB_storage, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MoveNodeDialog::fillFloor);
    connect(ui->cB_storage, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MoveNodeDialog::fillCompartment);

    connect(ui->buttonBox->button(QDialogButtonBox::Ok), &QPushButton::pressed, this, &MoveNodeDialog::save);
}

MoveNodeDialog::~MoveNodeDialog()
{
    delete ui;
    delete m_corpusModel;
    delete m_storageModel;
    delete m_floorModel;
}

void MoveNodeDialog::setupModels()
{
    m_corpusModel = new CorpusModel;
    m_corpusModel->select();
    ui->cB_corpus->setModel(m_corpusModel);
    ui->cB_corpus->setModelColumn(1);

    m_storageModel = new StorageModel;
    ui->cB_storage->setModel(m_storageModel);
    ui->cB_storage->setModelColumn(3);
    fillStorage(0);

    m_floorModel = new QStringListModel;
    ui->cB_floor->setModel(m_floorModel);
    fillFloor(0);
}

void MoveNodeDialog::fillStorage(int index)
{
    qDebug() << "here!";
    QModelIndex corpusModelIndex = m_corpusModel->index(index, 0);
    m_storageModel->setFilter("corpus=" + corpusModelIndex.data().toString());
    m_storageModel->select();

    ui->cB_storage->setEnabled((m_storageModel->rowCount() > 1));

    // clear floors if empty
    if(m_storageModel->rowCount() == 0) {
        fillFloor(-1);
    }
}

void MoveNodeDialog::fillCompartment(int index)
{
    QModelIndex storageModelIndex = m_storageModel->index(index, 4);

    if(storageModelIndex.isValid()) {
        // enable field once
        ui->sB_compartment->setEnabled(true);

        // setup max value
        ui->sB_compartment->setMaximum(storageModelIndex.data().toInt());
    }
}

void MoveNodeDialog::fillFloor(int index)
{
    QModelIndex storageModelIndex = m_storageModel->index(index, 5);
    QStringList floors;

    if(storageModelIndex.isValid()) {
        FloorsParser parser;
        floors = parser.process(storageModelIndex.data().toString());

        // floor - enable floor if count > 1
        ui->cB_floor->setEnabled((floors.length() > 1));
    }

    m_floorModel->setStringList(floors);
}


void MoveNodeDialog::save()
{
    QSqlRecord record = dataModel()->record();

    for(int i = 0; i < indexes().length(); ++i) {
        record.setValue("id",  indexes().at(i).siblingAtColumn(0).data());

        record.setValue("floor",  QVariant(ui->cB_floor->currentText().remove(QRegExp("\\D+"))));
        record.setGenerated("floor", true);

        record.setValue("storage", m_storageModel->index(ui->cB_storage->currentIndex(), 0).data());
        record.setGenerated("storage", true);

        record.setValue("compartment", QVariant(ui->sB_compartment->value()));
        record.setGenerated("compartment", true);

        record.setValue("shelving",  QVariant(ui->sB_shelving->value()));
        record.setGenerated("shelving", true);

        bool res = dataModel()->primaryUpdate(record);

        if(!res) {
            QMessageBox()
                    .critical(this,
                              tr("Move record"),
                              tr("Can not move record!"),
                              QMessageBox::Ok);
            break;
        }
    }
}
