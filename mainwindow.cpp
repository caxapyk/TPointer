#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "application.h"
#include "dialogs/structuredialog.h"
#include "models/hierarchymodel.h"

#include <QAction>
#include <QItemSelectionModel>
#include <QObject>
#include <QCoreApplication>
#include <QSettings>
#include <QString>

#include <QStandardItemModel>
#include <QStandardItem>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);



    HierarchyModel *model = new HierarchyModel();
    model->setupModelData();
    qDebug() << model->rowCount();
    ui->tV_hierarchy->setModel(model);

    restoreAppState();
    loadData();

    ui->statusbar->showMessage(tr("Ready"), 2000);

    connect(ui->action_storageStruct, &QAction::triggered, this, &MainWindow::openStructureDialog);
    connect(ui->action_about, &QAction::triggered, application, &Application::about);

    connect(ui->tV_MainTable->selectionModel(), &QItemSelectionModel::currentRowChanged, this, &MainWindow::rowSelected);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_mainTableModel;
}

void MainWindow::restoreAppState()
{
    QSettings* settings = application->applicationSettings();

    restoreGeometry(settings->value("MainWindow/geometry").toByteArray());
    restoreState(settings->value("MainWindow/windowState").toByteArray());

    ui->tV_MainTable->horizontalHeader()->restoreState(settings->value("DataTable/tableState").toByteArray());
}

void MainWindow::loadData() {
    m_mainTableModel = new MainTableModel;
    m_mainTableModel->select();

    ui->tV_MainTable->setModel(m_mainTableModel);
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
    ui->pTE_Desc-> setPlainText(t);
}

/*
 * SLOT openStructureDialog()
 */
void MainWindow::openStructureDialog()
{
    StructureDialog dialog;
    dialog.exec();
}

