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

    connect(ui->cB_storage, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &NodeDialog::fillFloor);
    connect(ui->pB_openFundList, &QPushButton::pressed, this, &NodeDialog::selectFund);
}

NodeDialog::~NodeDialog()
{
    delete ui;
    delete m_storageModel;
    delete m_floorModel;
    delete m_nILstorageModel;
    delete m_fundModel;
    delete m_nILfundModel;
    delete m_featureModel;
    delete m_nILfeatureModel;
}

void NodeDialog::restoreDialogState()
{
    QSettings* settings = application->applicationSettings();
    restoreGeometry(settings->value("NodeDialog/geometry").toByteArray());
}


void NodeDialog::setupModels()
{
    m_storageModel = new StorageExtendedModel;
    m_storageModel->select();
    m_nILstorageModel = new NoItemListModel;
    m_nILstorageModel->setModel(m_storageModel);
    m_nILstorageModel->setColumn(6);
    ui->cB_storage->setModel(m_nILstorageModel);

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

void NodeDialog::fillFloor(int index)
{
    QModelIndex storageModelIndex = m_storageModel->index(index - 1, 5);

    FloorsParser parser;
    QStringList floors = parser.process(storageModelIndex.data().toString());

    m_floorModel->setStringList(floors);
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
