#include "application.h"
#include "searchdialog.h"
#include "ui_searchdialog.h"

#include <QSettings>
#include <QPushButton>

SearchDialog::SearchDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SearchDialog)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);

    storage_model = new StorageModel;
    storage_model->select();
    _storage_model = new NoItemListModel(storage_model);
    _storage_model->setColumn(storage_model->columnCount()-1);

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
    //delete _feature_model;
}

void SearchDialog::search()
{
    QString filter = "";
    emit searched(filter);
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
    QSettings* settings = application->applicationSettings();
    restoreGeometry(settings->value("SearchWindow/geometry").toByteArray());

    QDialog::showEvent(event);
}
