#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "application.h"
#include "dialogs/paramdialog.h"
#include "models/hierarchymodel.h"
#include "models/searchmodel.h"
#include "widgets/customcontextmenu.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    restoreAppState();
    initialize();
    setupStatusBar();

    // MainWindow actions
    connect(ui->action_param, &QAction::triggered, this, &MainWindow::openParamDialog);
    connect(ui->action_about, &QAction::triggered, application, &Application::about);
    connect(ui->action_search, &QAction::triggered, this, &MainWindow::openSearchDialog);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_table_model;
    delete m_table_proxymodel;
    delete m_hierarchy_model;
    delete m_fund_model;
    delete m_fund_proxymodel;
    delete lb_server;
    delete lb_total;
    delete fund_filter;
    delete mt_filter;
    delete mt_controls;
    delete search_dialog;
}

void MainWindow::restoreAppState()
{
    QSettings* settings = application->applicationSettings();

    //restore main window geometry and state
    restoreGeometry(settings->value("MainWindow/geometry").toByteArray());
    restoreState(settings->value("MainWindow/windowState").toByteArray());

    //restore splitters state
    ui->splitter_base->restoreState(settings->value("Splitters/splitter_base").toByteArray());
    ui->splitter_pos->restoreState(settings->value("Splitters/splitter_pos").toByteArray());
    ui->splitter_mt->restoreState(settings->value("Splitters/splitter_mt").toByteArray());
}

void MainWindow::initialize()
{
    /* MainTableModel */
    m_table_model = new MainTableModel;
    m_table_proxymodel = new MainTableProxyModel;
    m_table_proxymodel->setSourceModel(m_table_model);

    // main table controls
    mt_controls = new ItemController(
                ItemController::Add | ItemController::Edit | ItemController::Remove,
                Qt::Horizontal);

    mt_controls->connectToMenu(ItemController::Add, ui->action_newRecord);
    mt_controls->connectToMenu(ItemController::Edit, ui->action_editRecord);
    mt_controls->connectToMenu(ItemController::Remove, ui->action_removeRecord);
    mt_controls->connectToMenu(ItemController::Refresh, ui->action_refresh);

    mt_controls->setMaximumSize(QSize(300, 50));

    ui->hL_mtPanel->addWidget(mt_controls, 0, Qt::AlignLeft);

    // main table filter
    mt_filter = new ItemFilter;
    mt_filter->setPlaceholderText(tr("Filter by notes..."));
    mt_filter->setMinimumSize(QSize(300, 0));
    mt_filter->setEnabled(false);

    ui->hL_mtPanel->addWidget(mt_filter, 0, Qt::AlignRight);

    connect(mt_filter, &ItemFilter::filtered, this, &MainWindow::filterMainTable);

    // get total rows
    total = m_table_model->count();

    /* HierarchyModel */
    m_hierarchy_model = new HierarchyModel;
    m_hierarchy_model->select();

    ui->tV_hierarchy->setModel(m_hierarchy_model);
    ui->tV_hierarchy->setColumnWidth(0, 250);
    ui->tV_hierarchy->resizeColumnToContents(1);

    ui->tV_hierarchy->expandToDepth(0);

    ui->tV_hierarchy->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->tV_hierarchy, &QTreeView::clicked, this, &MainWindow::loadByShelving);
    connect(ui->tV_hierarchy, &QMenu::customContextMenuRequested, this, &MainWindow::showHContextMenu);

     /* FundModel */
    m_fund_model = new FundReadonlyModel;
    m_fund_model->select();

    m_fund_proxymodel = new FundProxyModel;
    m_fund_proxymodel->setSourceModel(m_fund_model);

    ui->tV_funds->setModel(m_fund_proxymodel);
    ui->tV_funds->setContextMenuPolicy(Qt::CustomContextMenu);

    ui->tV_funds->hideColumn(1);
    ui->tV_funds->hideColumn(2);

    connect(ui->tV_funds, &QTreeView::clicked, this, &MainWindow::loadByFund);
    connect(ui->tV_funds, &QMenu::customContextMenuRequested, this, &MainWindow::showFContextMenu);

    // fund filter
    fund_filter = new ItemFilter;
    fund_filter->setPlaceholderText(tr("Fund filter..."));

    ui->vL_funds->addWidget(fund_filter);

    connect(fund_filter, &ItemFilter::filtered, this, &MainWindow::filterFunds);
}

void MainWindow::initializeMainTable()
{
    ui->tV_MainTable->setModel(m_table_proxymodel);
    ui->tV_MainTable->resizeColumnsToContents();
    ui->tV_MainTable->setColumnWidth(10, 180);

    ui->tV_MainTable->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->tV_MainTable->selectionModel(), &QItemSelectionModel::selectionChanged, this, &MainWindow::rowSelected, Qt::UniqueConnection);
    connect(ui->tV_MainTable, &QMenu::customContextMenuRequested, this, &MainWindow::showMTContextMenu, Qt::UniqueConnection);

    mt_controls->assetView(ui->tV_MainTable);
    mt_controls->setEnabled(true, ItemController::Add);
    mt_controls->setEnabled(true, ItemController::Refresh);

    // enable filter widget
    mt_filter->setEnabled(true);
    mt_filter->clear(); // clear every time model has changed
}

void MainWindow::setupStatusBar()
{
    lb_server = new QLabel(ui->statusbar);
    lb_server->setText(tr("Server: ") + application->server_address);
    ui->statusbar->addPermanentWidget(lb_server);

    lb_total = new QLabel(ui->statusbar);
    lb_total->setText(tr("| Total rows: ") + QString::number(total));
    ui->statusbar->addPermanentWidget(lb_total);

     ui->statusbar->showMessage(tr("Ready"), 2000);
}

void MainWindow::loadByShelving(const QModelIndex &index)
{
    const HierarchyNode *node = static_cast<const HierarchyNode*>(index.internalPointer());

    if (node->level == HierarchyModel::ShelvingLevel) {

        FilterStruct fs;
        fs.storage = node->parent->parent->id;
        fs.compartment = node->parent->name;
        fs.shelving = node->name;

        m_table_model->_setFilter(fs);
        m_table_model->select();

        ui->tV_MainTable->showColumn(7);

        ui->tV_MainTable->hideColumn(0);
        ui->tV_MainTable->hideColumn(2);
        ui->tV_MainTable->hideColumn(3);
        ui->tV_MainTable->hideColumn(4);

        initializeMainTable();

        ui->tV_funds->setCurrentIndex(QModelIndex());

        setWindowTitle(application->basename()
                       + tr(" [%1, Str. %2, Comp. %3, Sh. %4]")
                       .arg(node->parent->parent->parent->name.toString())
                       .arg(node->parent->parent->name.toString())
                       .arg(node->parent->name.toString())
                       .arg(node->name.toString()));

        setDisplayRows(m_table_model->rowCount());
    }
}

void MainWindow::loadByFund(const QModelIndex &index)
{
    FilterStruct fs;
    fs.fund = index.siblingAtColumn(1).data();
    fs.fund_strict = true;

    m_table_model->_setFilter(fs);
    m_table_model->select();

    ui->tV_MainTable->showColumn(2);
    ui->tV_MainTable->showColumn(3);
    ui->tV_MainTable->showColumn(4);

    ui->tV_MainTable->hideColumn(0);
    ui->tV_MainTable->hideColumn(7);


    initializeMainTable();

    ui->tV_hierarchy->setCurrentIndex(QModelIndex());

    setWindowTitle(application->basename()
                   + tr(" [Fund %1]")
                   .arg(index.data().toString()));

    setDisplayRows(m_table_model->rowCount());
}

void MainWindow::search(const FilterStruct &fs)
{
    m_table_model->_setFilter(fs);
    m_table_model->select();

    initializeMainTable();

    ui->tV_MainTable->showColumn(2);
    ui->tV_MainTable->showColumn(3);
    ui->tV_MainTable->showColumn(4);
    ui->tV_MainTable->showColumn(7);

    ui->tV_MainTable->hideColumn(0);

    ui->tV_hierarchy->setCurrentIndex(QModelIndex());
    ui->tV_funds->setCurrentIndex(QModelIndex());

    setWindowTitle(application->basename()
                   + tr(" [Search]"));

    setDisplayRows(m_table_model->rowCount());
}

void MainWindow::showHContextMenu(const QPoint&)
{
    CustomContextMenu menu(CustomContextMenu::Refresh);
    connect(&menu, &CustomContextMenu::refreshRequested, this, [=] {
        m_hierarchy_model->select();
        ui->tV_hierarchy->expandToDepth(0);
    });

    menu.exec(QCursor().pos());
}

void MainWindow::showFContextMenu(const QPoint&)
{
    CustomContextMenu menu(CustomContextMenu::Refresh);
    connect(&menu, &CustomContextMenu::refreshRequested, this, [=] {
        m_fund_model->select();
    });

    menu.exec(QCursor().pos());
}

void MainWindow::showMTContextMenu(const QPoint&)
{
    CustomContextMenu menu(CustomContextMenu::NoStandartAction);
    menu.addAction(ui->action_newRecord);
    menu.addSeparator();
    menu.addAction(ui->action_editRecord);
    menu.addAction(ui->action_removeRecord);
    menu.addSeparator();
    menu.addAction(ui->action_refresh);

    menu.exec(QCursor().pos());
}

void MainWindow::filterFunds(const QString &text)
{
    m_fund_proxymodel->setFilterKeyColumn(0);
    m_fund_proxymodel->setFilterFixedString(text);
}

void MainWindow::filterMainTable(const QString &text)
{
    m_table_proxymodel->setFilterKeyColumn(10);
    m_table_proxymodel->setFilterFixedString(text);
}

void MainWindow::setDisplayRows(int rows)
{
     ui->statusbar->showMessage(tr("Showing rows: ") + QString::number(rows));
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QSettings* settings = application->applicationSettings();

    settings->beginGroup("MainWindow");
    settings->setValue("geometry", saveGeometry());
    settings->setValue("windowState", saveState());
    settings->endGroup();

    settings->beginGroup("Splitters");
    settings->setValue("splitter_base", ui->splitter_base->saveState());
    settings->setValue("splitter_pos", ui->splitter_pos->saveState());
    settings->setValue("splitter_mt", ui->splitter_mt->saveState());
    settings->endGroup();

    QMainWindow::closeEvent(event);
}

void MainWindow::rowSelected(const QItemSelection &selected, const QItemSelection&)
{
    if (!selected.indexes().isEmpty()) {
        QModelIndex current = selected.indexes().at(0);

        QString descr =current.sibling(current.row(), 10).data().toString();
        QString feature = current.sibling(current.row(), 11).data().toString();

        QString t = descr + (descr.isNull() ? "" : "\n") + feature;

        ui->pTE_Desc->setPlainText(t);
    }
}

void MainWindow::openParamDialog()
{
    ParamDialog dialog;
    int res = dialog.exec();

    if (res == QDialog::Accepted) {
        m_hierarchy_model->select();
        ui->tV_hierarchy->expandToDepth(0);
    }
}

void MainWindow::openSearchDialog()
{
    if (!search_dialog) {
        search_dialog = new SearchDialog(this);
    }

    search_dialog->show();
    search_dialog->raise();
    search_dialog->activateWindow();

    connect(search_dialog, &SearchDialog::searched, this, &MainWindow::search);
}


