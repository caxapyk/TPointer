#include "view.h"
#include "application.h"

#include <QSettings>

View::View(QWidget *parent) : QWidget(parent)
{

}

View::~View()
{
    delete m_mainWidget;
}
