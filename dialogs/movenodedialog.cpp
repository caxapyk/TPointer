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
    ui->cB_storage->setModel(m_storageModel);
    ui->cB_storage->setModelColumn(6);

    m_floorModel = new QStringListModel;
    ui->cB_floor->setModel(m_floorModel);

    fillFloor(0);
}

void MoveNodeDialog::fillFloor(int index)
{
    QModelIndex storageModelIndex = m_storageModel->index(index, 5);

    FloorsParser parser;
    QStringList floors = parser.process(storageModelIndex.data().toString());

    m_floorModel->setStringList(floors);
}
