#include "navigationview.h"
#include "ui_navigationview.h"

NavigationView::NavigationView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NavigationView)
{
    ui->setupUi(this);
}

NavigationView::~NavigationView()
{
    delete ui;
}
