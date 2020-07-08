#include "application.h"
#include "connection.h"
#include "mainwindow.h"

#include <QDebug>
#include <QMessageBox>
#include <QTranslator>

Application* application = nullptr;

Application::Application(int &argc, char **argv) : QApplication(argc, argv)
{
    QCoreApplication::setOrganizationName(m_orgName);
    QCoreApplication::setApplicationName(m_appName);

    /*
     * Create new Global QSettings object on heap.
     */
    m_settings = new QSettings(QSettings::NativeFormat, QSettings::UserScope, m_orgName, m_appName);


    QTranslator* translator = new QTranslator(this);

    if(translator->load("TPointer_ru_RU"))
        qInfo() << "translator loaded";
    installTranslator(translator);

    try {
        Connection conn;
        conn.connect("services.lsk.gaorel.ru2",
                     "tpointer",
                     "root",
                     "antilopagnu");

        application = this;

        m_mainWindow = new MainWindow;
        m_mainWindow->show();

    } catch (ConnectionExeption e) {
        qCritical() << QString(e.p);

        QMessageBox msg;
        msg.setText(tr("Database connection error"));
        msg.setIcon(QMessageBox::Critical);
        msg.exec();


        exit(1);
    }
}

Application::~Application()
{
    delete m_mainWindow;
    delete m_settings;
}
