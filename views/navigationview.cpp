#include "navigationview.h"
#include "ui_navigationview.h"

#include "application.h"
#include "widgets/customcontextmenu.h"

#include <QDebug>
#include <QMenu>
#include <QMessageBox>
#include <QSettings>

NavigationView::NavigationView(QWidget *parent) :
    View(parent),
    ui(new Ui::NavigationView)
{
    ui->setupUi(this);

    setMainWidget(ui->splitter_nav);

    initialize();
    restoreViewState();
}

NavigationView::~NavigationView()
{
    saveViewState();
    delete ui;
    delete m_hierarchy_model;
    delete m_fund_model;
    delete m_fundc_model;
    delete m_fund_proxymodel;
    delete m_fundc_proxymodel;
    delete fund_filter;
}

void NavigationView::restoreViewState()
{

    QSettings* settings = application->applicationSettings();

    ui->splitter_nav->restoreState(settings->value("Views/splitter_nav").toByteArray());
    ui->tV_hierarchy->header()->restoreState(settings->value("Views/tV_hierarchy").toByteArray());
}

void NavigationView::saveViewState()
{

    QSettings* settings = application->applicationSettings();

    settings->beginGroup("Views");
    settings->setValue("splitter_nav", ui->splitter_nav->saveState());
    settings->setValue("tV_hierarchy", ui->tV_hierarchy->header()->saveState());
    settings->endGroup();
}

void NavigationView::initialize()
{
    /* HierarchyModel */
    m_hierarchy_model = new HierarchyModel;
    m_hierarchy_model->select();

    ui->tV_hierarchy->setModel(m_hierarchy_model);
    ui->tV_hierarchy->setColumnWidth(0, 180);
    ui->tV_hierarchy->resizeColumnToContents(1);

    ui->tV_hierarchy->expandToDepth(0);

    ui->tV_hierarchy->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->tV_hierarchy, &QTreeView::clicked, this, &NavigationView::hierarchyActivated);
    connect(ui->tV_hierarchy, &QMenu::customContextMenuRequested, this, &NavigationView::showHContextMenu);

     /* FundsModel */
    m_fund_model = new FundTreeModel;
    m_fundc_model = new FundTreeModel;

    m_fund_model->select();
    m_fundc_model->select();

    m_fund_proxymodel = new FundProxyModel;
    m_fund_proxymodel->setSourceModel(m_fund_model);
    m_fundc_proxymodel = new FundProxyModel;
    m_fundc_proxymodel->setSourceModel(m_fundc_model);

    ui->tV_funds->setModel(m_fund_proxymodel);
    ui->tV_funds->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->tV_fundsCurr->setModel(m_fundc_proxymodel);
    ui->tV_fundsCurr->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->tV_funds, &QTreeView::clicked, this, &NavigationView::fundActivated);
    connect(ui->tV_funds, &QMenu::customContextMenuRequested, this, &NavigationView::showFContextMenu);
    connect(ui->tV_fundsCurr, &QTreeView::clicked, this, &NavigationView::fundActivated);
    connect(ui->tV_fundsCurr, &QMenu::customContextMenuRequested, this, &NavigationView::showFContextMenu);

    // fund filter
    fund_filter = new ItemFilter;
    fund_filter->setPlaceholderText(tr("Fund filter..."));

    ui->vL_funds->addWidget(fund_filter);

    connect(fund_filter, &ItemFilter::filtered, this, &NavigationView::filterFunds);

    // fund tabs
    connect(ui->tW_funds, &QTabWidget::currentChanged, this, &NavigationView::fundTabChanged);

}

void NavigationView::resetCurrentIndexes()
{
    ui->tV_hierarchy->setCurrentIndex(QModelIndex());
    ui->tV_funds->setCurrentIndex(QModelIndex());
}

void NavigationView::hierarchyActivated(const QModelIndex &index)
{
    const HierarchyModel::HierarchyNode *node = static_cast<const HierarchyModel::HierarchyNode*>(index.internalPointer());

    if (node->level == HierarchyModel::ShelvingLevel) {

        FilterStruct filter;
        filter.corpus = node->parent->parent->parent->id;
        filter.storage = node->parent->parent->id;
        filter.compartment = node->parent->name;
        filter.shelving = node->name;

        application->mainWindow()->dataView()->loadData(filter);
        application->mainWindow()->setWindowTitle(application->basename()
                       + tr(" [%1, Str. %2, Comp. %3, Sh. %4]")
                       .arg(node->parent->parent->parent->name.toString())
                       .arg(node->parent->parent->name.toString())
                       .arg(node->parent->name.toString())
                       .arg(node->name.toString()));
    }

    ui->tV_funds->setCurrentIndex(QModelIndex());
}

void NavigationView::fundTabChanged(int index)
{
    switch (index) {
    case 0:
        break;
    case 1:
        break;
    }
}

void NavigationView::fundActivated(const QModelIndex &index)
{
    if (index.parent().isValid()) {
        FilterStruct filter;
        filter.fund = index.data(Qt::UserRole + 1);
        filter.fund_strict = true;

        application->mainWindow()->dataView()->loadData(filter);
        application->mainWindow()->setWindowTitle(application->basename()
                       + tr(" [Fund %1]")
                       .arg(index.data().toString()));

        ui->tV_hierarchy->setCurrentIndex(QModelIndex());
    }
}

void NavigationView::filterFunds(const QString &text)
{
    if (text.length() > 0) {
        QSortFilterProxyModel *m = (ui->tW_funds->currentIndex() > 0) ? m_fundc_proxymodel: m_fund_proxymodel;
        m->setRecursiveFilteringEnabled(true);
        m->setFilterKeyColumn(0);

        m->setFilterFixedString(text);
    } else {
        m_fund_proxymodel->setFilterFixedString(text);
        m_fundc_proxymodel->setFilterFixedString(text);
    }
}

void NavigationView::showHContextMenu(const QPoint&)
{
    CustomContextMenu menu(CustomContextMenu::Refresh);
    connect(&menu, &CustomContextMenu::refreshRequested, this, [=] {
        m_hierarchy_model->select();
        ui->tV_hierarchy->expandToDepth(0);
    });

    menu.exec(QCursor().pos());
}

void NavigationView::showFContextMenu(const QPoint &point)
{
    CustomContextMenu menu(CustomContextMenu::Refresh);
    connect(&menu, &CustomContextMenu::refreshRequested, this, [=] {
        if(ui->tV_funds->indexAt(point).isValid()) {
            m_fund_model->select();
        } else if(ui->tV_fundsCurr->indexAt(point).isValid()) {
            m_fundc_model->select();
        }
    });

    menu.exec(QCursor().pos());
}
