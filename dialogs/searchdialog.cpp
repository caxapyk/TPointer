#include "application.h"
#include "searchdialog.h"
#include "ui_searchdialog.h"

#include <QSettings>
#include <QPushButton>
#include <QDebug>

SearchDialog::SearchDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SearchDialog)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);

    storage_model = new StorageModel;
    storage_model->select();

    _storage_model = new NoItemListModel(storage_model);
    _storage_model->setColumn(6);

    ui->cB_storage->setModel(_storage_model);

    feature_model = new FeatureModel;
    feature_model->select();

    _feature_model = new NoItemListModel(feature_model);
    _feature_model->setColumn(2);

    ui->cB_feature->setModel(_feature_model);

    connect(ui->pB_search, &QPushButton::released, this, &SearchDialog::search);
}

SearchDialog::~SearchDialog()
{
    delete ui;
    delete storage_model;
    delete _storage_model;
    delete feature_model;
    delete _feature_model;
}

void SearchDialog::search()
{
    fs.storage = (ui->cB_storage->currentIndex() > 0) ?
                storage_model->data(
                    storage_model->index(
                        ui->cB_storage->currentIndex()-1, 0)) : QVariant();

    fs.compartment = (ui->lE_compartment->text().length() > 0) ? QVariant(ui->lE_compartment->text()) : QVariant();
    fs.shelving = (ui->lE_shelving->text().length() > 0) ? QVariant(ui->lE_shelving->text()) : QVariant();

    fs.fund = (ui->lE_fund->text().length() > 0) ? QVariant(ui->lE_fund->text()): QVariant();
    fs.fund_strict = ui->cB_strict->isChecked();

    fs.feature = (ui->cB_feature->currentIndex() > 0) ?
                feature_model->data(
                    feature_model->index(
                        ui->cB_feature->currentIndex()-1,0)) : QVariant();

    emit searched(fs);
}

/*
 * Overrides QMainWindow::closeEvent(QCloseEvent *e)
 */
void SearchDialog::closeEvent(QCloseEvent *e)
{
    QSettings* settings = application->applicationSettings();
    settings->beginGroup("SearchWindow");
    settings->setValue("geometry", saveGeometry());
    settings->endGroup();

    QDialog::closeEvent(e);
}

/*
 * Overrides QMainWindow::showEvent(QShowEvent *event)
 */
void SearchDialog::showEvent(QShowEvent *event)
{
    // update related models (may be changed outside)
    storage_model->select();
    feature_model->select();

    QSettings* settings = application->applicationSettings();
    restoreGeometry(settings->value("SearchWindow/geometry").toByteArray());

    QDialog::showEvent(event);
}
