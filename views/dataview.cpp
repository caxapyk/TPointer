#include "dataview.h"
#include "ui_dataview.h"

#include "application.h"
#include "dialogs/addnodedialog.h"
#include "dialogs/editnodedialog.h"
#include "dialogs/movenodedialog.h"
#include "models/fundmodel.h"

#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QPainter>
#include <QPrinter>
#include <QPrintDialog>
#include <QSettings>

DataView::DataView(QWidget *parent) :
    View(parent),
    ui(new Ui::DataView)
{
    ui->setupUi(this);

    setMainWidget(ui->splitter_mt);

    initialize();
    restoreViewState();
}

DataView::~DataView()
{
    saveViewState();
    delete ui;
    delete m_model;
    delete m_proxyModel;
    delete m_tableController;
    delete m_itemFilter;
    delete m_buttonsControl;
}

void DataView::restoreViewState()
{
    QSettings* settings = application->applicationSettings();
    ui->splitter_mt->restoreState(settings->value("Views/splitter_mt").toByteArray());
    ui->tV_dataTable->horizontalHeader()->restoreState(settings->value("Views/tV_dataTable").toByteArray());
}

void DataView::saveViewState()
{
    QSettings* settings = application->applicationSettings();

    settings->beginGroup("Views");
    settings->setValue("tV_dataTable", ui->tV_dataTable->horizontalHeader()->saveState());
    settings->setValue("splitter_mt", ui->splitter_mt->saveState());
    settings->endGroup();
}

void DataView::initialize()
{
    m_model = new DataModel;
    m_proxyModel = new DataProxyModel;
    m_proxyModel->setSourceModel(m_model);

    m_tableController = new TableModelController;

    m_buttonsControl = new ButtonsControl(ButtonsControl::Add | ButtonsControl::Edit | ButtonsControl::Remove | ButtonsControl::Refresh, Qt::Horizontal);

    connect(m_buttonsControl, &ButtonsControl::addRequested, this, [=] {
        addItem();
    });

    m_buttonsControl->setMaximumSize(QSize(300, 50));

    //ui->hL_mtPanel->addWidget(m_buttonsControl, 0, Qt::AlignLeft);

    // main table filter
    m_itemFilter = new ItemFilter;
    m_itemFilter->setPlaceholderText(tr("Filter by notes..."));
    m_itemFilter->setMinimumSize(QSize(300, 0));
    m_itemFilter->setEnabled(false);

    ui->hL_mtPanel->addWidget(m_itemFilter, 0, Qt::AlignRight);

    ui->tV_dataTable->horizontalHeader()->setHidden(true);

    connect(m_itemFilter, &ItemFilter::filtered, this, &DataView::filterMainTable);
}

void DataView::loadData(const FilterStruct &filter)
{
    m_model->setFilter(filter);
    m_model->select();

    ui->tV_dataTable->setModel(m_proxyModel);
    ui->tV_dataTable->horizontalHeader()->setHidden(false);
    ui->tV_dataTable->hideColumn(0); // hide id

    ui->tV_dataTable->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->tV_dataTable->selectionModel(), &QItemSelectionModel::selectionChanged, this, &DataView::showMetaData, Qt::UniqueConnection);
    connect(ui->tV_dataTable, &QMenu::customContextMenuRequested, this, &DataView::showContextMenu, Qt::UniqueConnection);

    m_buttonsControl->assetView(ui->tV_dataTable);
    m_buttonsControl->connectToMenu(ButtonsControl::Add, application->mainWindow()->getMenuAction("action_mAdd"));
    m_buttonsControl->connectToMenu(ButtonsControl::Edit, application->mainWindow()->getMenuAction("action_mEdit"));
    m_buttonsControl->connectToMenu(ButtonsControl::Remove, application->mainWindow()->getMenuAction("action_mRemove"));
    m_buttonsControl->connectToMenu(ButtonsControl::Refresh, application->mainWindow()->getMenuAction("action_mRefresh"));

    connect(application->mainWindow()->getMenuAction("action_mAdd"), &QAction::triggered, this, &DataView::addItem, Qt::UniqueConnection);
    connect(application->mainWindow()->getMenuAction("action_mEdit"), &QAction::triggered, this, &DataView::editItem, Qt::UniqueConnection);
    connect(application->mainWindow()->getMenuAction("action_mRemove"), &QAction::triggered, this, &DataView::removeItems, Qt::UniqueConnection);
    connect(application->mainWindow()->getMenuAction("action_mRefresh"), &QAction::triggered, this, &DataView::refresh, Qt::UniqueConnection);

    // disable add record on fund selected or searched

    m_buttonsControl->setEnabled((!filter.isSearch) ? filter.fund.isNull() : false, ButtonsControl::Add);
    m_buttonsControl->setEnabled(true, ButtonsControl::Refresh);

    // enable filter widget
    m_itemFilter->setEnabled(true);
    m_itemFilter->clear(); // clear every time model has changed

    application->mainWindow()->setDisplayRows(m_model->rowCount());
    application->mainWindow()->setExportCsvEnabled(true);
}

void DataView::clearView()
{
    QSortFilterProxyModel model;
    ui->tV_dataTable->setModel(&model);

    m_buttonsControl->setEnabled(false, ButtonsControl::Add);
    m_buttonsControl->setEnabled(false, ButtonsControl::Refresh);

    disconnect(application->mainWindow()->getMenuAction("action_mAdd"), &QAction::triggered, this, &DataView::addItem);
    disconnect(application->mainWindow()->getMenuAction("action_mEdit"), &QAction::triggered, this, &DataView::editItem);
    disconnect(application->mainWindow()->getMenuAction("action_mRemove"), &QAction::triggered, this, &DataView::removeItems);
    disconnect(application->mainWindow()->getMenuAction("action_mRefresh"), &QAction::triggered, this, &DataView::refresh);
}

void DataView::addItem()
{
    AddNodeDialog dialog(m_model);
    dialog.applyFilter(m_model->filterStruct());

    int res = dialog.exec();

    if (res == QDialog::Accepted) {
        application->mainWindow()->updateTotal();
    }
}

void DataView::editItem()
{
    QModelIndex index = m_proxyModel->mapToSource(ui->tV_dataTable->currentIndex());
    EditNodeDialog dialog(m_model, index.row());

    int res = dialog.exec();

    if (res == QDialog::Accepted) {
        // do something
    }
}

void DataView::moveItems()
{
    MoveNodeDialog dialog;
    QModelIndexList indexes = ui->tV_dataTable->selectionModel()->selectedRows();

    dialog.setDataModel(m_model);
    dialog.setIndexes(indexes);

    int res = dialog.exec();

    if (res == QDialog::Accepted) {
        m_model->select();
    }
}

void DataView::removeItems()
{
    int res = QMessageBox()
            .critical(this,
                      tr("Delete record"),
                      tr("Are you shure that you want to delete %1 items?").arg(ui->tV_dataTable->selectionModel()->selectedRows().length()),
                      QMessageBox::No | QMessageBox::Yes);

    if (res == QMessageBox::Yes) {
        QModelIndexList indexes = ui->tV_dataTable->selectionModel()->selectedRows();
        if (m_proxyModel->removeRows(indexes.at(0).row(), indexes.length())) {
            application->mainWindow()->updateTotal();
        } else {
            QMessageBox::warning(this, tr("Record list"), tr("Could not remove item"), QMessageBox::Ok);
        }
    }
}

void DataView::showMetaData(const QItemSelection &selected, const QItemSelection&)
{
    if (!selected.indexes().isEmpty()) {
        QModelIndex current = selected.indexes().at(0);

        QString descr = current.sibling(current.row(), DataModel::Note).data().toString();
        QString feature = current.sibling(current.row(), DataModel::Features).data().toString();

        QString t = descr + (descr.isNull() ? "" : "\n") + feature;

        ui->pTE_Desc->setPlainText(t);
    }
}

void DataView::filterMainTable(const QString &text)
{
    m_proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    m_proxyModel->setFilterKeyColumn(DataModel::Note);
    m_proxyModel->setFilterFixedString(text);
}

void DataView::showContextMenu(const QPoint&)
{
    QMenu menu;

    /* default sort action */
    QAction *defaultSortAction = new QAction(tr("Sort by default"));
    menu.addAction(defaultSortAction);
    defaultSortAction->setEnabled(ui->tV_dataTable->horizontalHeader()->sortIndicatorSection() >= 0);
    connect(defaultSortAction, &QAction::triggered, this, [=] {
        ui->tV_dataTable->sortByColumn(-1, Qt::AscendingOrder);
    });
    menu.addSeparator();

    menu.addAction(application->mainWindow()->getMenuAction("action_mAdd"));
    menu.addAction(application->mainWindow()->getMenuAction("action_mEdit"));
    menu.addAction(application->mainWindow()->getMenuAction("action_mRemove"));
    menu.addSeparator();

    /* move action */
    QAction *moveAction = new QAction(tr("Move"));
    menu.addAction(moveAction);
    moveAction->setEnabled(!ui->tV_dataTable->selectionModel()->selectedRows().isEmpty());
    connect(moveAction, &QAction::triggered, this, &DataView::moveItems);
    menu.addSeparator();

    menu.addAction(application->mainWindow()->getMenuAction("action_mRefresh"));

    // disable add record on fund selected or searched
    if(m_model->filterStruct().isSearch || m_model->filterStruct().fund.isValid()) {
        application->mainWindow()->getMenuAction("action_mAdd")->setDisabled(true);
    }

    menu.exec(QCursor().pos());

    // ??
    delete defaultSortAction;
    delete moveAction;
}

void DataView::refresh()
{
    m_model->select();
    ui->pTE_Desc->clear();
}

void DataView::printF(TemplateHtml &templ, QMap<QString, QVariant> vars)
{
    if(m_proxyModel->rowCount() > 0) {
        // floors
        QStringList floors;
        for(int i = 0; i < m_proxyModel->rowCount(); ++i) {
            QString floor = m_proxyModel->index(i, 3).data().toString();
            if(!floors.contains(floor)) {
                floors.append(floor);
            }
        }

        vars.insert("[[floor]]",  floors.join(","));

        templ.setVars(vars);
        templ.print();
    } else {
        QMessageBox::warning(this, tr("Print"), tr("Could not print. List is empty!"), QMessageBox::Ok);
    }
}

void DataView::printF15()
{
    QMap<QString, QVariant> vars;
    TemplateHtml th("tmp/f15");

    vars.insert("[[shelving]]", m_proxyModel->index(0, 5).data());
    vars.insert("[[corpus]]", m_proxyModel->index(0, 1).data());
    vars.insert("[[storage]]",  m_proxyModel->index(0, 2).data());
    vars.insert("[[compartment]]",  m_proxyModel->index(0, 4).data());

    vars.insert("[[rows]]", th.makeTableRows(m_proxyModel, QVector<int>() << 6 << 7 << 8 << 9 << 10 << 11));

    printF(th, vars);
}

void DataView::printF16()
{
    QMap<QString, QVariant> vars;
    TemplateHtml th("tmp/f16");

    vars.insert("[[fundname]]", FundModel::getFundName(m_model->index(0, 8).data().toString()));
    vars.insert("[[fund]]", m_model->index(0, 8).data());
    vars.insert("[[corpus]]", m_model->index(0, 1).data());
    vars.insert("[[storage]]",  m_model->index(0, 2).data());

    // check if rows in the same compartment, otherwise make empty var
    QVariant comp = m_proxyModel->index(0, 4).data();
    int cc = 0;
    for(int i = 0; i < m_proxyModel->rowCount(); ++i) {
        if(comp != m_proxyModel->index(i, 4).data()) {
            ++cc;
            break;
        }
    }
    vars.insert("[[compartment]]",  cc == 0 ? m_proxyModel->index(0, 4).data() : "");

    vars.insert("[[rows]]", th.makeTableRows(m_model, QVector<int>() << 9 << 10 << 5 << 6 << 7 << 11));

    printF(th, vars);
}

void DataView::exportCsv()
{
    QFileDialog dialog;
    QString location = dialog.getSaveFileName(this, tr("Export CSV"), tr("untitled.csv"), tr("Spreadsheets (*.csv)"));

    QFile file(location);

    if(file.open(QIODevice::WriteOnly))
    {
        QTextStream stream(&file);

        /* header */
        QStringList header;
        for(int i = 0; i < m_proxyModel->columnCount(); ++i)
        {
            header<< m_proxyModel->headerData(i, Qt::Horizontal).toString();
        }
        stream << header.join(";") + "\n";

        /* rows */
        QStringList row;
        for(int i = 0; i < m_proxyModel->rowCount(); ++i)
        {
            row.clear();
            for(int column = 0; column < m_proxyModel->columnCount(); ++column )
            {
                QString value = m_proxyModel->index(i, column).data().toString();
                value.replace(";", ",");
                value.replace("\"", "'");

                row << value;
            }
            stream << row.join(";") + "\n";
        }

        file.close();
    }
}
