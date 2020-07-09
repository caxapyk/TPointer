#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "application.h"
#include "dialogs/structuredialog.h"

#include <QObject>
#include <QCoreApplication>
#include <QSettings>
#include <QString>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    restoreAppState();
    loadData();

    ui->statusbar->showMessage(tr("Ready"), 2000);

    connect(ui->action_storageStruct, SIGNAL(triggered()),
            this, SLOT(openStructureDialog()));

    connect(ui->action_about, SIGNAL(triggered()),
            application, SLOT(about()));
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
}

void MainWindow::loadData() {
    m_mainTableModel = new MainTableModel;
    m_mainTableModel->select();

    ui->tV_MainTable->setModel(m_mainTableModel);
    connect(ui->tV_MainTable, SIGNAL(clicked(const QModelIndex&)),
            this, SLOT(selectRow(const QModelIndex&)));
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
    QMainWindow::closeEvent(event);
}

/*
 * SLOT selectRow(const QModelIndex &index)
 */
void MainWindow::selectRow(const QModelIndex &index)
{
    QString t = index.sibling(index.row(), m_descColumn).data().toString();
    ui->pTE_Desc-> setPlainText(t);
}

/*
 * SLOT openStructureDialog()
 */
void MainWindow::openStructureDialog()
{
    StructureDialog dialog;
    dialog.setWindowTitle(tr("Storage structure"));
    dialog.exec();
}

