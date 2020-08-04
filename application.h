#ifndef APPLICATION_H
#define APPLICATION_H

#include "mainwindow.h"

#include <QApplication>
#include <QIcon>
#include <QTranslator>
#include <QString>
#include <QSettings>

class Application : public QApplication
{

    Q_OBJECT

public:
    Application(int &argc, char **argv);
    ~Application();
    QSettings* applicationSettings() { return m_settings; }
    QVariant version() const { return m_version; };
    QIcon icon() const{ return m_icon; };
    QString basename() { return QString(tr("Archival topographic pointer %1").arg(version().toString())); };
    MainWindow *mainWindow() { return m_mainWindow; };
    QString server_address;

    const QString m_orgName = "caxapyk";
    const QString m_appName = "TPointer";
    const QVariant m_version = "2.0";
    const QIcon m_icon = QIcon(":/icons/icon.png");

public slots:
      void about() const;

private:
    /*
     * Create new Global QSettings object on heap.
     */
    void initializeSettings();
    void initializeLanguage();

    QSettings *m_settings;
    MainWindow *m_mainWindow;
    QTranslator *qtTranslator;
    QTranslator *appTranslator;
};

/**
* Global pointer used to access the Application.
*/
extern Application* application;

#endif // APPLICATION_H
