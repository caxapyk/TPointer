#include "application.h"
#include "searchdialog.h"
#include "ui_searchdialog.h"

#include <QSettings>

SearchDialog::SearchDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SearchDialog)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
}

SearchDialog::~SearchDialog()
{
    delete ui;
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
