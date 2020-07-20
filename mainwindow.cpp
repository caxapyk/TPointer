#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "application.h"
#include "dialogs/paramdialog.h"
#include "models/hierarchymodel.h"
#include "models/searchmodel.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    restoreAppState();
    setupModels();
    setupStatusBar();

    connect(ui->action_param, &QAction::triggered, this, &MainWindow::openParamDialog);
    connect(ui->action_about, &QAction::triggered, application, &Application::about);
    connect(ui->action_search, &QAction::triggered, this, &MainWindow::openSearchDialog);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_table_model;
    delete m_hierarchy_model;
    delete m_fund_model;
    delete m_fund_proxymodel;
    delete lb_server;
    delete search_dialog;
}

void MainWindow::restoreAppState()
{
    QSettings* settings = application->applicationSettings();

    restoreGeometry(settings->value("MainWindow/geometry").toByteArray());
    restoreState(settings->value("MainWindow/windowState").toByteArray());
}

void MainWindow::setupModels()
{
    /* MainTableModel */
    m_table_model = new MainTableModel;
    total = m_table_model->count();

    /* HierarchyModel */
    m_hierarchy_model = new HierarchyModel;
    m_hierarchy_model->select();

    ui->tV_hierarchy->setModel(m_hierarchy_model);
    ui->tV_hierarchy->setColumnWidth(0, 200);
    ui->tV_hierarchy->resizeColumnToContents(1);

    ui->tV_hierarchy->expandToDepth(0);

    connect(ui->tV_hierarchy, &QTreeView::clicked, this, &MainWindow::loadData);

     /* FundModel */
    m_fund_model = new FundModel(this);
    m_fund_model->select();

    m_fund_proxymodel = new QSortFilterProxyModel(this);
    m_fund_proxymodel->setSourceModel(m_fund_model);

    ui->tV_funds->setModel(m_fund_proxymodel);
    ui->tV_funds->setColumnWidth(0, 200);
    ui->tV_funds->resizeColumnToContents(1);

    connect(ui->tV_funds, &QTreeView::clicked, this, &MainWindow::loadData);
    connect(ui->lE_fundFilter, &QLineEdit::textChanged, this, &MainWindow::filterFunds);
    connect(ui->pB_clearFundFilter, &QPushButton::released, this, &MainWindow::clearFundFilter);
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

void MainWindow::loadData(const QModelIndex &index)
{
    if (dynamic_cast<const HierarchyModel*>(index.model())){
        const HierarchyNode *node = static_cast<const HierarchyNode*>(index.internalPointer());
        if (node->level == HierarchyModel::ShelvingLevel) {
            m_table_model->setFilter("storage=" + node->parent->parent->id.toString()
                                        + " AND compartment='"
                                        + node->parent->name.toString()
                                        + "' AND shelving='"
                                        + node->name.toString()
                                        + "'");
            ui->tV_funds->setCurrentIndex(QModelIndex());

            m_table_model->select();
            ui->tV_MainTable->setModel(m_table_model);

            ui->tV_MainTable->showColumn(7);

            ui->tV_MainTable->hideColumn(0);
            ui->tV_MainTable->hideColumn(2);
            ui->tV_MainTable->hideColumn(3);
            ui->tV_MainTable->hideColumn(4);

            setWindowTitle(tr("Archival topographic pointer")
                           + tr(" [%1, %2, Comp.%3, Sh.%4]")
                            .arg(node->parent->parent->parent->name.toString())
                            .arg(node->parent->parent->name.toString())
                            .arg(node->parent->name.toString())
                            .arg(node->name.toString()));

            connect(ui->tV_MainTable->selectionModel(), &QItemSelectionModel::currentRowChanged, this, &MainWindow::rowSelected, Qt::UniqueConnection);
        }
    } else if (dynamic_cast<const QSortFilterProxyModel*>(index.model())) {
        m_table_model->setFilter("fund='" + index.data().toString() + "'");
        ui->tV_hierarchy->setCurrentIndex(QModelIndex());

        m_table_model->select();
        ui->tV_MainTable->setModel(m_table_model);

        ui->tV_MainTable->showColumn(2);
        ui->tV_MainTable->showColumn(3);
        ui->tV_MainTable->showColumn(4);

        ui->tV_MainTable->hideColumn(0);
        ui->tV_MainTable->hideColumn(7);

        setWindowTitle(tr("Archival topographic pointer")
                       + tr(" [Fund %1]")
                        .arg(index.data().toString()));

        connect(ui->tV_MainTable->selectionModel(), &QItemSelectionModel::currentRowChanged, this, &MainWindow::rowSelected, Qt::UniqueConnection);
    }

    ui->tV_MainTable->resizeColumnsToContents();
    ui->tV_MainTable->setColumnWidth(10, 180);

    setDisplayRows(m_table_model->rowCount());

    /* Restore table state */
    //QSettings* settings = application->applicationSettings();
    //ui->tV_MainTable->horizontalHeader()->restoreState(settings->value("DataTable/tableState").toByteArray());
}

void MainWindow::filterFunds(const QString &text)
{
    m_fund_proxymodel->setFilterFixedString(text);
    ui->pB_clearFundFilter->setEnabled(text.length() > 0);
}

void MainWindow::clearFundFilter()
{
    ui->lE_fundFilter->clear();
    ui->pB_clearFundFilter->setEnabled(false);
}

void MainWindow::search(QString &filter)
{
    m_table_model->setFilter(filter);
    ui->tV_hierarchy->setCurrentIndex(QModelIndex());
    ui->tV_funds->setCurrentIndex(QModelIndex());

    m_table_model->select();
    ui->tV_MainTable->setModel(m_table_model);

    ui->tV_MainTable->showColumn(2);
    ui->tV_MainTable->showColumn(3);
    ui->tV_MainTable->showColumn(4);
    ui->tV_MainTable->showColumn(7);

    ui->tV_MainTable->hideColumn(0);

    setWindowTitle(tr("Archival topographic pointer")
                   + tr(" [Search]"));

    connect(ui->tV_MainTable->selectionModel(), &QItemSelectionModel::currentRowChanged, this, &MainWindow::rowSelected, Qt::UniqueConnection);

    ui->tV_MainTable->resizeColumnsToContents();
    ui->tV_MainTable->setColumnWidth(10, 180);

    setDisplayRows(m_table_model->rowCount());
}

void MainWindow::setDisplayRows(int rows)
{
     ui->statusbar->showMessage(tr("Showing rows: ") + QString::number(rows));
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

    /*settings->beginGroup("DataTable");
    settings->setValue("tableState", ui->tV_MainTable->horizontalHeader()->saveState());
    settings->endGroup();*/

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
 * SLOT openParamDialog()
 */
void MainWindow::openParamDialog()
{
    ParamDialog dialog;
    int res = dialog.exec();

    if (res == QDialog::Accepted) {
        m_hierarchy_model->select();
        ui->tV_hierarchy->expandToDepth(0);
    }
}

/*
 * SLOT openSearchDialog()
 */
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


