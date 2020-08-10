#include "application.h"
#include "nodedialog.h"
#include "ui_nodedialog.h"
#include "utils/floorsparser.h"

#include <QDebug>

NodeDialog::NodeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NodeDialog)
{
    ui->setupUi(this);
    restoreDialogState();

    setupModels();

    validatorCupboard = new QIntValidator(1, 99);
    ui->lE_cupboard->setValidator(validatorCupboard);

    connect(ui->cB_corpus, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &NodeDialog::fillStorage);
    connect(ui->cB_storage, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &NodeDialog::fillFloor);
    connect(ui->cB_storage, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &NodeDialog::fillCompartment);
    connect(ui->pB_openFundList, &QPushButton::pressed, this, &NodeDialog::selectFund);

    connect(ui->buttonBox->button(QDialogButtonBox::Reset), &QPushButton::pressed, this, &NodeDialog::revert);
    connect(ui->buttonBox->button(QDialogButtonBox::Save), &QPushButton::pressed, this, &NodeDialog::save);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &NodeDialog::cancel);
}

NodeDialog::~NodeDialog()
{
    delete ui;
    delete m_corpusModel;
    delete m_nILcorpusModel;
    delete m_storageModel;
    delete m_floorModel;
    delete m_fundModel;
    delete m_nILfundModel;
    delete m_featureModel;
    delete m_nILfeatureModel;
    delete validatorCupboard;
}

void NodeDialog::restoreDialogState()
{
    QSettings* settings = application->applicationSettings();
    restoreGeometry(settings->value("NodeDialog/geometry").toByteArray());
}

void NodeDialog::setupModels()
{
    m_corpusModel = new CorpusModel;
    m_corpusModel->select();
    m_nILcorpusModel = new NoItemListModel;
    m_nILcorpusModel->setModel(m_corpusModel);
    m_nILcorpusModel->setColumn(1);
    ui->cB_corpus->setModel(m_nILcorpusModel);

    m_storageModel = new StorageModel;
    ui->cB_storage->setModel(m_storageModel);
    ui->cB_storage->setModelColumn(3);

    m_floorModel = new QStringListModel;
    ui->cB_floor->setModel(m_floorModel);

    m_fundModel = new FundModel;
    m_fundModel->select();
    m_nILfundModel = new NoItemListModel;
    m_nILfundModel->setModel(m_fundModel);
    m_nILfundModel->setColumn(1);
    ui->cB_fund->setModel(m_nILfundModel);

    m_featureModel = new FeatureModel;
    m_featureModel->select();
    m_nILfeatureModel = new NoItemListModel;
    m_nILfeatureModel->setModel(m_featureModel);
    m_nILfeatureModel->setColumn(2);
    ui->cB_feature->setModel(m_nILfeatureModel);
}

void NodeDialog::fillStorage(int index)
{
    QModelIndex corpusModelIndex = m_corpusModel->index(index - 1, 0);
    m_storageModel->setFilter("corpus=" + corpusModelIndex.data().toString());
    m_storageModel->select();

    ui->cB_storage->setEnabled((m_storageModel->rowCount() > 1));

    // clear floors if empty
    if(m_storageModel->rowCount() == 0) {
        fillFloor(-1);
    }
}

void NodeDialog::fillFloor(int index)
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

void NodeDialog::fillCompartment(int index)
{
    QModelIndex storageModelIndex = m_storageModel->index(index, 4);

    if(storageModelIndex.isValid()) {
        // enable field once
        ui->sB_compartment->setEnabled(true);

        // setup max value
        ui->sB_compartment->setMaximum(storageModelIndex.data().toInt());
    }
}

void NodeDialog::selectFund()
{
    FundListDialog dialog;
    dialog.setSigleRowSelection();

    int res = dialog.exec();

    if (res == QDialog::Accepted && dialog.selectedPrimaryKey().isValid()) {
         m_fundModel->select();

         for (int i = 0; i < m_nILfundModel->rowCount(); ++i) {
                if(m_fundModel->index(i, 0).data() == dialog.selectedPrimaryKey()) {
                    ui->cB_fund->setCurrentIndex(i + 1);
                    break;
                } else {
                    ui->cB_fund->setCurrentIndex(0);
                }
         }
    }
}

void NodeDialog::closeEvent(QCloseEvent *event)
{
    QSettings* settings = application->applicationSettings();

    settings->beginGroup("NodeDialog");
    settings->setValue("geometry", saveGeometry());
    settings->endGroup();

    QDialog::closeEvent(event);
}
