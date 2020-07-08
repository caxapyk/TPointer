#include "application.h"
#include "mainwindow.h"

Application::Application(int &argc, char **argv) : QApplication(argc, argv)
{
    QCoreApplication::setOrganizationName(m_orgName);
    QCoreApplication::setApplicationName(m_appName);

    m_mainWindow = new MainWindow();
    m_mainWindow->show();
}

Application::~Application()
{
    delete m_mainWindow;
}
