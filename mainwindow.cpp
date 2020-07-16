#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "application.h"
#include "dialogs/structuredialog.h"
#include "models/hierarchymodel.h"

#include <QAction>
#include <QDialog>
#include <QItemSelectionModel>
#include <QObject>
#include <QCoreApplication>
#include <QSettings>
#include <QString>
#include <QTreeView>

#include <QStandardItemModel>
#include <QStandardItem>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /* MainTableModel */
    m_mainTableModel = new MainTableModel;
    ui->tV_MainTable->setModel(m_mainTableModel);

    /* HierarchyModel */
    m_hierarchy_model = new HierarchyModel;
    m_hierarchy_model->select();

    m_hierarchy_model->setHeaderData(0, Qt::Horizontal, tr("Place path"));
    m_hierarchy_model->setHeaderData(1, Qt::Horizontal, tr("floors"));

    ui->tV_hierarchy->setModel(m_hierarchy_model);
    ui->tV_hierarchy->setColumnWidth(0, 200);
    ui->tV_hierarchy->resizeColumnToContents(1);

    ui->tV_hierarchy->expandToDepth(0);

    connect(ui->tV_hierarchy, &QTreeView::clicked, this, &MainWindow::loadData);

     /* FundModel */
    m_fund_model = new FundModel(this);
    m_fund_model->select();

    m_fund_model->setHeaderData(0, Qt::Horizontal, tr("Funds"));

    ui->tV_funds->setModel(m_fund_model);
    ui->tV_funds->setColumnWidth(0, 200);
    ui->tV_funds->resizeColumnToContents(1);

    connect(ui->tV_funds, &QTreeView::clicked, this, &MainWindow::loadData);

    /* Initialize */

    restoreAppState();
    setupStatusBar();

    ui->statusbar->showMessage(tr("Ready"), 2000);

    connect(ui->action_storageStruct, &QAction::triggered, this, &MainWindow::openStructureDialog);
    connect(ui->action_about, &QAction::triggered, application, &Application::about);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_mainTableModel;
    delete m_hierarchy_model;
    delete m_fund_model;
    delete lb_server;
}

void MainWindow::restoreAppState()
{
    QSettings* settings = application->applicationSettings();

    restoreGeometry(settings->value("MainWindow/geometry").toByteArray());
    restoreState(settings->value("MainWindow/windowState").toByteArray());
}

void MainWindow::setupStatusBar()
{
    lb_server = new QLabel(ui->statusbar);
    lb_server->setText("Server: " + application->server_address);
    ui->statusbar->addPermanentWidget(lb_server);
}

void MainWindow::loadData(const QModelIndex &index)
{

    if (dynamic_cast<const HierarchyModel*>(index.model())) {
        const HierarchyNode *node = static_cast<const HierarchyNode*>(index.internalPointer());
        if (node->level == HierarchyModel::ShelvingLevel) {
            m_mainTableModel->select();
            //fix storage=1
            m_mainTableModel->setFilter("storage=1 AND compartment='"
                                        + node->parent->name.toString()
                                        + "' AND shelving='"
                                        + node->name.toString()
                                        + "'");
        }

    } else if (dynamic_cast<const FundModel*>(index.model())) {
        m_mainTableModel->setFilter("fund='" + index.data().toString() + "'");
        m_mainTableModel->select();
    }

    connect(ui->tV_MainTable->selectionModel(), &QItemSelectionModel::currentRowChanged, this, &MainWindow::rowSelected, Qt::UniqueConnection);

    ui->statusbar->showMessage(tr("Showing rows: ") + QString::number(m_mainTableModel->rowCount()));

    /* Restore table state */
    QSettings* settings = application->applicationSettings();
    ui->tV_MainTable->horizontalHeader()->restoreState(settings->value("DataTable/tableState").toByteArray());
}

/*
 * Overrides QMainWindow::closeEvent(QCloseEvent *event)
 */
void MainWindow::closeEvent(QCloseEvent *event)
{
    QSettings* settings = application->applicationSettings();

    settings->beginGroup("MainWindow");
    settings->setValue("geometry", saveGeometry());
    settings->setValue("windowState", saveState());
    settings->endGroup();

    settings->beginGroup("DataTable");
    settings->setValue("tableState", ui->tV_MainTable->horizontalHeader()->saveState());
    settings->endGroup();

    QMainWindow::closeEvent(event);
}

/*
 * SLOT selectRow(const QModelIndex &index)
 */
void MainWindow::rowSelected(const QModelIndex &current, const QModelIndex&)
{
    QString t = current.sibling(current.row(), m_descColumn).data().toString();
    ui->pTE_Desc->setPlainText(t);
}

/*
 * SLOT openStructureDialog()
 */
void MainWindow::openStructureDialog()
{
    StructureDialog dialog;
    int res = dialog.exec();

    if (res == QDialog::Accepted) {
        m_hierarchy_model->select();
    }
}

