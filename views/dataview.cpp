#include "dataview.h"
#include "ui_dataview.h"

#include "application.h"
#include "dialogs/addnodedialog.h"
#include "dialogs/editnodedialog.h"
#include "dialogs/movenodedialog.h"
#include "widgets/customcontextmenu.h"

#include <QMessageBox>
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

    /*
    connect(ui->action_remove, &QAction::triggered, this, [=] {
        removeItem(ui->tV_dataTable);
    });
    connect(ui->action_refresh, &QAction::triggered, m_tableController, [=] {
        m_model->select();
    });*/

    m_buttonsControl = new ButtonsControl(
                ButtonsControl::Add | ButtonsControl::Edit | ButtonsControl::Remove,
                Qt::Horizontal);

    connect(m_buttonsControl, &ButtonsControl::addRequested, this, [=] {
        addItem();
    });

    //m_buttonsControl->connectToMenu(ButtonsControl::Add, ui->action_new);
    //m_buttonsControl->connectToMenu(ButtonsControl::Edit, ui->action_edit);
    //m_buttonsControl->connectToMenu(ButtonsControl::Remove, ui->action_remove);

    m_buttonsControl->setMaximumSize(QSize(300, 50));

    ui->hL_mtPanel->addWidget(m_buttonsControl, 0, Qt::AlignLeft);

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
    ui->tV_dataTable->hideColumn(0);

    ui->tV_dataTable->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->tV_dataTable->selectionModel(), &QItemSelectionModel::selectionChanged, this, &DataView::showMetaData, Qt::UniqueConnection);
    connect(ui->tV_dataTable, &QMenu::customContextMenuRequested, this, &DataView::showContextMenu, Qt::UniqueConnection);

    m_buttonsControl->assetView(ui->tV_dataTable);
    m_buttonsControl->setEnabled(true, ButtonsControl::Add);
    m_buttonsControl->setEnabled(true, ButtonsControl::Refresh);

    // enable filter widget
    m_itemFilter->setEnabled(true);
    m_itemFilter->clear(); // clear every time model has changed

    application->mainWindow()->setDisplayRows(m_model->rowCount());
}

void DataView::addItem()
{
    AddNodeDialog dialog;
    dialog.applyFilter(m_model->filterStruct());

    int res = dialog.exec();

    if (res == QDialog::Accepted) {
        // do something
    }
}

void DataView::editItem()
{
    QModelIndex index = ui->tV_dataTable->currentIndex();
    m_model->setMetaField(index);

    EditNodeDialog dialog(m_model, index.row());

    int res = dialog.exec();

    if (res == QDialog::Accepted) {
        // do something
    }
}

void DataView::moveItems()
{
    MoveNodeDialog dialog;

    int res = dialog.exec();

    if (res == QDialog::Accepted) {
        // do something
    }
}

void DataView::removeItems()
{
    int res = QMessageBox()
            .critical(this,
                      tr("Delete record"),
                      tr("Are you shure that you want to delete %1 items?").arg(1/*view->selectionModel()->selectedRows().length()*/),
                      QMessageBox::No | QMessageBox::Yes);

    if (res == QMessageBox::Yes) {
        if(true) {
            application->mainWindow()->updateTotal();
        } else {
            QMessageBox::warning(this, tr("Fund list"), tr("Could not remove item"), QMessageBox::Ok);
        }
    }
}

void DataView::showMetaData(const QItemSelection &selected, const QItemSelection&)
{
    if (!selected.indexes().isEmpty()) {
        QModelIndex current = selected.indexes().at(0);

        QString descr = current.sibling(current.row(), 10).data().toString();
        QString feature = current.sibling(current.row(), 11).data().toString();

        QString t = descr + (descr.isNull() ? "" : "\n") + feature;

        ui->pTE_Desc->setPlainText(t);
    }
}

void DataView::filterMainTable(const QString &text)
{
    m_proxyModel->setFilterKeyColumn(10);
    m_proxyModel->setFilterFixedString(text);
}

void DataView::showContextMenu(const QPoint&)
{
    CustomContextMenu menu(
                CustomContextMenu::Add | CustomContextMenu::Edit | CustomContextMenu::Remove | CustomContextMenu::Refresh);
    menu.setSelection(ui->tV_dataTable->selectionModel()->selectedRows());

    connect(&menu, &CustomContextMenu::addRequested, this, &DataView::addItem);
    connect(&menu, &CustomContextMenu::editRequested, this, &DataView::editItem);
    connect(&menu, &CustomContextMenu::removeRequested, this, &DataView::removeItems);


    connect(&menu, &CustomContextMenu::refreshRequested, this, [=] {
        m_model->select();
    });

    /* default sort action */
    QAction *defaultSortAction = new QAction(tr("Sort by default"));
    menu.insertAction(menu.action(CustomContextMenu::Add), defaultSortAction);
    defaultSortAction->setEnabled(ui->tV_dataTable->horizontalHeader()->sortIndicatorSection() >= 0);
    connect(defaultSortAction, &QAction::triggered, this, [=] {
        ui->tV_dataTable->sortByColumn(-1, Qt::AscendingOrder);
    });
    menu.insertSeparator(menu.action(CustomContextMenu::Add));

    /* move action */
    QAction *moveAction = new QAction(tr("Move"));
    menu.insertAction(menu.action(CustomContextMenu::Refresh), moveAction);
    moveAction->setEnabled(!ui->tV_dataTable->selectionModel()->selectedRows().isEmpty());
    connect(moveAction, &QAction::triggered, this, &DataView::moveItems);
    menu.insertSeparator(moveAction);
    menu.insertSeparator(menu.action(CustomContextMenu::Refresh));

    menu.exec(QCursor().pos());

    // ??
    delete defaultSortAction;
    delete moveAction;
}

