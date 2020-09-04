#include "fundlistdialog.h"
#include "ui_fundlistdialog.h"

#include <QMessageBox>
#include <QDebug>

FundListDialog::FundListDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FundListDialog)
{
    ui->setupUi(this);

    // fund model
    f_model = new FundModel;
    f_model->select();

    // model controller
    controller = new TableModelController();

    f_proxymodel = new FundProxyModel;
    f_proxymodel->setSourceModel(f_model);

    ui->tV_funds->setModel(f_proxymodel);

    ui->tV_funds->setContextMenuPolicy(Qt::CustomContextMenu);

    ui->tV_funds->hideColumn(0);
    ui->tV_funds->setColumnWidth(1, 140);

    // fund controls
    controls = new ButtonsControl(
                ButtonsControl::Add | ButtonsControl::Remove,
                Qt::Horizontal);

    controls->assetView(ui->tV_funds);
    controls->setEnabled(true, ButtonsControl::Add);

    ui->hL_controls->addWidget(controls);

    // fund filter
    filter = new ItemFilter;
    filter->setPlaceholderText(tr("Fund filter..."));

    ui->hL_controls->addWidget(filter);

    // triggers
    connect(ui->tV_funds->selectionModel(), &QItemSelectionModel::selectionChanged, this, &FundListDialog::selectionChanged);

    connect(controls, &ButtonsControl::addRequested, this, &FundListDialog::createItem);
    connect(controls, &ButtonsControl::removeRequested, this, &FundListDialog::removeItem);

    connect(filter, &ItemFilter::filtered, this, &FundListDialog::filterFunds);
}

FundListDialog::~FundListDialog()
{
    delete ui;
    delete f_model;
    delete f_proxymodel;
    delete controls;
    delete controller;
    delete filter;
}

void FundListDialog::filterFunds(const QString &text)
{
    f_proxymodel->setFilterKeyColumn(1);
    f_proxymodel->setFilterFixedString(text);
}

void FundListDialog::createItem(QAbstractItemView *view)
{
    filter->clear();
    f_proxymodel->invalidate();
    if (!controller->createItem(view)) {
        QMessageBox::warning(this, tr("Fund list"), tr("Could not create item"), QMessageBox::Ok);
    }
}

void FundListDialog::removeItem(QAbstractItemView *view)
{
    int res = QMessageBox()
            .critical(this,
                      tr("Delete items"),
                      tr("Are you shure that you want to delete %1 fund?").arg(view->selectionModel()->selectedRows().length()),
                      QMessageBox::No | QMessageBox::Yes);

    if (res == QMessageBox::Yes && !controller->removeItem(view)) {
        QMessageBox::warning(this, tr("Fund list"), tr("Could not remove item"), QMessageBox::Ok);
    }
}

void FundListDialog::selectionChanged(const QItemSelection &selected, const QItemSelection&)
{
    if (!selected.isEmpty()) {
        int col_id = selected.indexes().at(0).data().toInt();

        if (col_id) {
            pK = col_id;
        }
    }
}

void FundListDialog::setSigleRowSelection()
{
    ui->tV_funds->setSelectionMode(QAbstractItemView::SingleSelection);
}
