#ifndef APPLICATION_H
#define APPLICATION_H

#include "mainwindow.h"

#include <QApplication>
#include <QString>
#include <QSettings>

class Application : public QApplication
{

public:
    Application(int &argc, char **argv);
    ~Application();
    QSettings* applicationSettings() { return m_settings; }

private:
    QSettings* m_settings;
    MainWindow* m_mainWindow;

    const QString m_orgName = "caxapyk";
    const QString m_appName = "TPointer";

};

/**
* Global pointer used to access the Application.
*/
extern Application* application;

#endif // APPLICATION_H
