#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "application.h"

#include <QObject>
#include <QCoreApplication>
#include <QSettings>
#include <QString>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    RestoreState();
    LoadData();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_mainTableModel;
}

void MainWindow::selectRow(const QModelIndex &index)
{
    //QString t = index.data().toString();
    QString t = index.sibling(index.row(), m_descColumn).data().toString();
    ui->pTE_Desc-> setPlainText(t);
}

void MainWindow::RestoreState()
{

    QSettings* settings = application->applicationSettings();

    restoreGeometry(settings->value("MainWindow/geometry").toByteArray());
    restoreState(settings->value("MainWindow/windowState").toByteArray());
}

void MainWindow::LoadData() {
    m_mainTableModel = new MainTableModel;
    m_mainTableModel->select();

    ui->tV_MainTable->setModel(m_mainTableModel);
    QObject::connect(ui->tV_MainTable, SIGNAL(clicked(const QModelIndex&)),
                      this,  SLOT(selectRow(const QModelIndex&)));
}

// overrides
void MainWindow::closeEvent(QCloseEvent *event)
{
    QSettings* settings = application->applicationSettings();

    settings->beginGroup("MainWindow");
    settings->setValue("geometry", saveGeometry());
    settings->setValue("windowState", saveState());
    settings->endGroup();
    QMainWindow::closeEvent(event);
}

