#ifndef APPLICATION_H
#define APPLICATION_H

#include "mainwindow.h"

#include <QApplication>
#include <QIcon>
#include <QString>
#include <QSettings>

class Application : public QApplication
{

    Q_OBJECT

public:
    Application(int &argc, char **argv);
    ~Application();
    QSettings* applicationSettings() { return m_settings; }
    QString version() const { return m_version; };
    QIcon icon() const{ return m_icon; };

    QString server_address;
    const QString m_orgName = "caxapyk";
    const QString m_appName = "TPointer";
    const QString m_version = "0.0";
    const QIcon m_icon = QIcon(":/icons/icon.png");

public slots:
      void about() const;

private:
    /*
     * Create new Global QSettings object on heap.
     */
    void initializeSettings();
    void initializeLanguage();

    QSettings* m_settings;
    MainWindow* m_mainWindow;
};

/**
* Global pointer used to access the Application.
*/
extern Application* application;

#endif // APPLICATION_H
