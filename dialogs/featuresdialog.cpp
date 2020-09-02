#include "featuresdialog.h"
#include "ui_featuresdialog.h"
#include "models/featuremodel.h"

#include <QMessageBox>

FeaturesDialog::FeaturesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FeaturesDialog)
{
    ui->setupUi(this);

    ft_controls = new ButtonsControl(
                ButtonsControl::Add | ButtonsControl::Remove | ButtonsControl::Up | ButtonsControl::Down);
    ui->gB_features->layout()->addWidget(ft_controls);

    m_feature_model = new FeatureModel;
    loadFeatures();

    // features
    connect(ft_controls, &ButtonsControl::addRequested, this,  &FeaturesDialog::createItem);
    connect(ft_controls, &ButtonsControl::removeRequested, this,  &FeaturesDialog::removeItem);
    connect(ft_controls, &ButtonsControl::upRequested, this,  &FeaturesDialog::moveUp);
    connect(ft_controls, &ButtonsControl::downRequested, this,  &FeaturesDialog::moveDown);
}

FeaturesDialog::~FeaturesDialog()
{
    delete ui;
    delete m_feature_model;
    delete ft_controls;
}

void FeaturesDialog::loadFeatures() {
    m_feature_model->select();

    m_feature_model->setHeaderData(2, Qt::Horizontal, tr("Name"));

    ui->tV_features->setModel(m_feature_model);

    ui->tV_features->hideColumn(0);
    ui->tV_features->hideColumn(1);

    ft_controls->assetView(ui->tV_features);
    ft_controls->setEnabled(true, ButtonsControl::Add);
}

void FeaturesDialog::createItem(QAbstractItemView *view)
{
    if (!controller->createItem(view)) {
        QMessageBox::warning(this, tr("Features"), tr("Could not create item"), QMessageBox::Ok);
    }
}

void FeaturesDialog::removeItem(QAbstractItemView *view)
{
    int res = QMessageBox()
            .critical(this,
                      tr("Delete items"),
                      tr("Are you shure that you want to delete %1 item?").arg(view->selectionModel()->selectedRows().length()),
                      QMessageBox::No | QMessageBox::Yes);

    if (res == QMessageBox::Yes && !controller->removeItem(view)) {
        QMessageBox::warning(this, tr("Features"), tr("Could not remove item"), QMessageBox::Ok);
    }
}

void FeaturesDialog::moveUp(QAbstractItemView *view)
{
   controller->moveUp(view);
}

void FeaturesDialog::moveDown(QAbstractItemView *view)
{
    controller->moveDown(view);
}
