#include "movenodedialog.h"
#include "ui_movenodedialog.h"
#include "utils/floorsparser.h"

MoveNodeDialog::MoveNodeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MoveNodeDialog)
{
    ui->setupUi(this);

    setupModels();

    connect(ui->cB_storage, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MoveNodeDialog::fillFloor);
}

MoveNodeDialog::~MoveNodeDialog()
{
    delete ui;
}

void MoveNodeDialog::setupModels()
{
    m_storageModel = new StorageExtendedModel;
    m_storageModel->select();
    m_nILstorageModel = new NoItemListModel;
    m_nILstorageModel->setModel(m_storageModel);
    m_nILstorageModel->setColumn(6);
    ui->cB_storage->setModel(m_nILstorageModel);

    m_floorModel = new QStringListModel;
    ui->cB_floor->setModel(m_floorModel);
}

void MoveNodeDialog::fillFloor(int index)
{
    QModelIndex storageModelIndex = m_storageModel->index(index - 1, 5);

    FloorsParser parser;
    QStringList floors = parser.process(storageModelIndex.data().toString());

    m_floorModel->setStringList(floors);
}
