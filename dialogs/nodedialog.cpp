#include "mainformdialog.h"
#include "ui_mainformdialog.h"

MainFormDialog::MainFormDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainFormDialog)
{
    ui->setupUi(this);
}

MainFormDialog::~MainFormDialog()
{
    delete ui;
}
