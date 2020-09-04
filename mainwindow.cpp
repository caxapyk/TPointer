#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "application.h"
#include "dialogs/fundlistdialog.h"
#include "dialogs/featuresdialog.h"
#include "dialogs/insertnodedialog.h"
#include "dialogs/paramdialog.h"
#include "dialogs/syncdialog.h"
#include "models/hierarchymodel.h"
#include "models/searchmodel.h"
#include "widgets/customcontextmenu.h"

#include <QDebug>
#include <QMessageBox>
#include <QSplitter>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(windowTitle() + " " + application->version().toString());

    initialize();
    restoreAppState();
    setupStatusBar();
    setupToolBar();
    setupItemFilter();

    // MainWindow actions
    connect(ui->action_new, &QAction::triggered, this, &MainWindow::insertNode);
    connect(ui->action_csv, &QAction::triggered, m_dataView, &DataView::exportCsv);
    connect(ui->action_form15, &QAction::triggered, m_dataView, &DataView::printF15);
    connect(ui->action_form16, &QAction::triggered, m_dataView, &DataView::printF16);
    connect(ui->action_about, &QAction::triggered, application, &Application::about);
    connect(ui->action_search, &QAction::triggered, this, &MainWindow::openSearch);
    connect(ui->action_features, &QAction::triggered, this, &MainWindow::openFeatures);
    connect(ui->action_fundList, &QAction::triggered, this, &MainWindow::openFundList);
    connect(ui->action_param, &QAction::triggered, this, &MainWindow::openParam);
    connect(ui->action_sync, &QAction::triggered, this, &MainWindow::openSync);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_navView;
    delete m_dataView;
    delete m_itemFilter;
    delete tb_layout;
    delete tb_panel;
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

void MainWindow::setupToolBar()
{
    ui->toolBar->addAction(ui->action_new);
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(ui->action_mAdd);
    ui->toolBar->addAction(ui->action_mEdit);
    ui->toolBar->addAction(ui->action_mRemove);
    ui->toolBar->addAction(ui->action_mRefresh);
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(ui->action_search);

    ui->toolBar->setMaximumHeight(40);
}

void MainWindow::setupItemFilter()
{
    // main table filter widget
    m_itemFilter = new ItemFilter;
    m_itemFilter->setPlaceholderText(tr("Filter by notes..."));
    m_itemFilter->setMaximumWidth(400);
    m_itemFilter->setDisabled(true);

    tb_panel = new QWidget();
    tb_layout = new QHBoxLayout();
    tb_layout->setContentsMargins(0, 0, 0, 0);
    tb_layout->setAlignment(Qt::AlignRight);
    tb_panel->setLayout(tb_layout);

    tb_layout->addWidget(m_itemFilter);
    ui->toolBar->addWidget(tb_panel);

    connect(m_itemFilter, &ItemFilter::filtered, m_dataView, &DataView::filterMainTable);
}

void MainWindow::clearMTableFilter()
{
    m_itemFilter->clear();
}

void MainWindow::updateTotal()
{
    DataModel model;
    lb_total->setText(tr("| Total rows: %1").arg(model.count()));
}

void MainWindow::search(const FilterStruct &filter)
{
    dataView()->loadData(filter);
    setWindowTitle(application->basename()
                   + tr(" [Search]"));

    navView()->resetCurrentIndexes();
    setPrintF15Enabled(false);
    setPrintF16Enabled(false);
}

void MainWindow::setExportCsvEnabled(bool e)
{
    ui->action_csv->setEnabled(e);
}

void MainWindow::setPrintF15Enabled(bool e)
{
    ui->action_form15->setEnabled(e);
}

void MainWindow::setPrintF16Enabled(bool e)
{
    ui->action_form16->setEnabled(e);
}

void MainWindow::setDisplayRows(int count)
{
     ui->statusbar->showMessage(tr("Showing rows: %1").arg(count), 3600000);
}

QAction* MainWindow::getMenuAction(const QString &name)
{
    return findChild<QAction*>(name);
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

void MainWindow::insertNode()
{
    InsertNodeDialog dialog;
    DataModel model;

    dialog.setDataModel(&model);

    int res = dialog.exec();

    if (res == QDialog::Accepted) {
        updateTotal();
    }
}

void MainWindow::openMainForm()
{
    NodeDialog dialog;
    dialog.exec();
}

void MainWindow::openFeatures()
{
    FeaturesDialog dialog;
    dialog.exec();
}

void MainWindow::openFundList()
{
    FundListDialog dialog;
    dialog.exec();
}

void MainWindow::openParam()
{
    ParamDialog dialog;
    dialog.exec();
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

void MainWindow::openSync()
{
    SyncDialog dialog;
    dialog.exec();
}

