#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "application.h"
#include "dialogs/insertnodedialog.h"
#include "dialogs/paramdialog.h"
#include "dialogs/fundlistdialog.h"
#include "models/hierarchymodel.h"
#include "models/searchmodel.h"
#include "widgets/customcontextmenu.h"
#include "utils/tablemodeltohtml.h"

#include <QDebug>
#include <QMessageBox>
#include <QSplitter>
#include <QFileDialog>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initialize();
    restoreAppState();
    setupStatusBar();

    // MainWindow actions
    connect(ui->action_new, &QAction::triggered, this, &MainWindow::insertNode);
    connect(ui->action_csv, &QAction::triggered, this, &MainWindow::openExportCsv);
    connect(ui->action_form15, &QAction::triggered, this, &MainWindow::openExportPdf);
    connect(ui->action_param, &QAction::triggered, this, &MainWindow::openParam);
    connect(ui->action_about, &QAction::triggered, application, &Application::about);
    connect(ui->action_search, &QAction::triggered, this, &MainWindow::openSearch);
    connect(ui->action_fundList, &QAction::triggered, this, &MainWindow::openFundList);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_navView;
    delete m_dataView;
    delete lb_server;
    delete lb_total;
    delete search_dialog;
}

void MainWindow::restoreAppState()
{
    QSettings* settings = application->applicationSettings();

    //restore main window geometry and states
    restoreGeometry(settings->value("MainWindow/geometry").toByteArray());
    restoreState(settings->value("MainWindow/windowState").toByteArray());

    ui->splitter->restoreState(settings->value("MainWindow/splitter").toByteArray());
}

void MainWindow::initialize()
{
    m_navView = new NavigationView(ui->splitter);
    m_dataView = new DataView(ui->splitter);
}


void MainWindow::setupStatusBar()
{
    lb_server = new QLabel(ui->statusbar);
    lb_server->setText(tr("Server: ") + application->server_address);
    ui->statusbar->addPermanentWidget(lb_server);

    lb_total = new QLabel(ui->statusbar);
    ui->statusbar->addPermanentWidget(lb_total);
    updateTotal();

    ui->statusbar->showMessage(tr("Ready"), 2000);
}

void MainWindow::updateTotal()
{
    DataModel model;
    model.select();

    lb_total->setText(tr("| Total rows: %1").arg(model.count()));
}

void MainWindow::search(const FilterStruct &filter)
{
    dataView()->loadData(filter);
    setWindowTitle(application->basename()
                   + tr(" [Search]"));

    navView()->resetCurrentIndexes();
}


void MainWindow::dataLoaded()
{
    ui->menuPrint->setEnabled(true);
    ui->action_csv->setEnabled(true);
    ui->action_pdf->setEnabled(true);
}

void MainWindow::setDisplayRows(int count)
{
     ui->statusbar->showMessage(tr("Showing rows: %1").arg(count), 3600000);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QSettings* settings = application->applicationSettings();

    settings->beginGroup("MainWindow");
    settings->setValue("geometry", saveGeometry());
    settings->setValue("windowState", saveState());
    settings->setValue("splitter", ui->splitter->saveState());
    settings->endGroup();

    QMainWindow::closeEvent(event);
}

/* *
 * Dialogs
 * */

void MainWindow::openExportCsv()
{
    QFileDialog dialog;
    QString output_file_location = dialog.getSaveFileName(this, tr("Save File"),
                                                          tr("untitled.csv"),
                                                          tr("Spreadsheets (*.csv)"));
}

void MainWindow::openExportPdf()
{
    QFileDialog dialog;
    QString file_location = dialog.getSaveFileName(this, tr("Save File"),
                                                          tr("untitled.pdf"),
                                                          tr("PDF (*.pdf)"));

    TableModelToHtml *h = new TableModelToHtml(file_location, "tmp/table.html", m_dataView->model());
    ui->verticalLayout->addWidget(h->printHtmlView);

    h->save();
}

void MainWindow::insertNode()
{
    InsertNodeDialog dialog;
    DataModel model;

    dialog.setDataModel(&model);

    int res = dialog.exec();

    if (res == QDialog::Accepted) {
        // do something
    }
}

void MainWindow::openMainForm()
{
    NodeDialog dialog;
    int res = dialog.exec();

    if (res == QDialog::Accepted) {
        // do something
    }
}

void MainWindow::openFundList()
{
    FundListDialog dialog;
    int res = dialog.exec();

    if (res == QDialog::Accepted) {
         // do something
    }
}

void MainWindow::openParam()
{
    ParamDialog dialog;
    int res = dialog.exec();

    if (res == QDialog::Accepted) {
         // do something
    }
}

void MainWindow::openSearch()
{
    if (!search_dialog) {
        search_dialog = new SearchDialog(this);
    }

    search_dialog->show();
    search_dialog->raise();
    search_dialog->activateWindow();

    connect(search_dialog, &SearchDialog::searched, this, &MainWindow::search);
}

void MainWindow::openPrint()
{
    m_dataView->print();
}


