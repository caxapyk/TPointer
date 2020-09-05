#include "connectiondialog.h"
#include "ui_connectiondialog.h"
#include "application.h"
#include "connection.h"

#include <QSettings>

ConnectionDialog::ConnectionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectionDialog)
{
    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Save)->setDisabled(true);

    QSettings *settings = application->applicationSettings();

    settings->beginGroup("Database");
    ui->lE_hostname->setText(settings->value("hostname").toString());
    ui->lE_dbname->setText(settings->value("db").toString());
    ui->lE_dbuser->setText(settings->value("user").toString());
    ui->lE_dbpass->setText(settings->value("password").toString());
    settings->endGroup();

    connect(ui->pB_test, &QPushButton::clicked, this, &ConnectionDialog::testConnection);
}

ConnectionDialog::~ConnectionDialog()
{
    delete ui;
}

void ConnectionDialog::testConnection()
{
    QSqlDatabase db;
    test_db = db.contains("db_test") ? db.database("db_test") : db.addDatabase("QMYSQL", "db_test");

    test_db.setHostName(ui->lE_hostname->text());
    test_db.setDatabaseName(ui->lE_dbname->text());
    test_db.setUserName(ui->lE_dbuser->text());
    test_db.setPassword(ui->lE_dbpass->text());

    test_db.setConnectOptions("MYSQL_OPT_CONNECT_TIMEOUT=5");

    if (test_db.open()) {
          ui->label_status->setStyleSheet("color: green");
          ui->label_status->setText(tr("Connection is successfull!"));

          ui->buttonBox->button(QDialogButtonBox::Save)->setEnabled(true);
          connect(ui->buttonBox->button(QDialogButtonBox::Save), &QPushButton::clicked, this, &ConnectionDialog::save);
      } else {
          ui->label_status->setStyleSheet("color: red");
          ui->label_status->setText(tr("Could not connect to database!"));

          ui->buttonBox->button(QDialogButtonBox::Save)->setDisabled(true);
    }
}

void ConnectionDialog::save()
{
    QSettings *settings = application->applicationSettings();

    settings->beginGroup("Database");
    settings->setValue("hostname", ui->lE_hostname->text());
    settings->setValue("db", ui->lE_dbname->text());
    settings->setValue("user", ui->lE_dbuser->text());
    settings->setValue("password", ui->lE_dbpass->text());
    settings->endGroup();

    accept();
}
