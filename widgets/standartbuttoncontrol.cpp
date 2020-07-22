#include "buttoncontrols.h"
#include "ui_buttoncontrols.h"

ButtonControls::ButtonControls(int, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ButtonControls)
{
    ui->setupUi(this);
    group = new QButtonGroup;

    group->addButton(ui->pB_add);
    group->setId(ui->pB_add, 0);
    group->addButton(ui->pB_remove);
    group->setId(ui->pB_remove, 1);
    group->addButton(ui->pB_up);
    group->setId(ui->pB_up, 2);
    group->addButton(ui->pB_down);
    group->setId(ui->pB_down, 3);
}

ButtonControls::~ButtonControls()
{
    delete ui;
    delete group;
}

QPushButton* ButtonControls::buttonAdd()
{
    return ui->pB_add;
}

QPushButton* ButtonControls::buttonRemove()
{
    return ui->pB_remove;
}

QPushButton* ButtonControls::buttonUp()
{
    return ui->pB_up;
}

QPushButton* ButtonControls::buttonDown()
{
    return ui->pB_down;
}

void ButtonControls::modelSelected(const QModelIndex &index)
{
    for (int i = 1; i < 4; ++i) {
        group->button(i)->setEnabled(index.isValid());
    }
}

void ButtonControls::disable() {
    modelSelected(QModelIndex());
}
