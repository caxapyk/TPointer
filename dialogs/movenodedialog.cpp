#include "movenodedialog.h"
#include "ui_movenodedialog.h"

MoveNodeDialog::MoveNodeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MoveNodeDialog)
{
    ui->setupUi(this);
}

MoveNodeDialog::~MoveNodeDialog()
{
    delete ui;
}
