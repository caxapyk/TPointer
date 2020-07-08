#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>
#include <QString>
#include "mainwindow.h"

class Application : public QApplication
{

public:
    Application(int &argc, char **argv);
    ~Application();

    MainWindow* m_mainWindow;

private:
    const QString m_orgName = "caxapyk";
    const QString m_appName = "TPointer";

};

#endif // APPLICATION_H
