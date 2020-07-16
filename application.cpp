#include "application.h"
#include "connection.h"
#include "mainwindow.h"

#include <QDebug>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QIcon>
#include <QMessageBox>
#include <QTranslator>
#include <QStringList>

Application* application = nullptr;

Application::Application(int &argc, char **argv) : QApplication(argc, argv)
{
    application = this; //global pointer to current instance

    initializeSettings();
    initializeLanguage();

    QCommandLineParser parser;
    parser.setApplicationDescription(tr("Archival topographic pointer"));
    parser.addVersionOption();

    QCommandLineOption hostnameOption(QStringList() << "h" << "hostname", tr("Address of database server"), "localhost");
    parser.addOption(hostnameOption);
    QCommandLineOption databaseOption(QStringList() << "d" << "database", tr("Database name"), "tpointer");
    parser.addOption(databaseOption);
    QCommandLineOption usernameOption(QStringList() << "u" << "username", tr("Database user"), "root");
    parser.addOption(usernameOption);
    QCommandLineOption passwordOption(QStringList() << "p" << "password", tr("Password to database user"), "pass");
    parser.addOption(passwordOption);

    parser.process(*application);

    server_address = parser.value(hostnameOption);

    try {
        Connection conn;
        conn.connect(parser.value(hostnameOption),
                     parser.value(databaseOption),
                     parser.value(usernameOption),
                     parser.value(passwordOption));

        m_mainWindow = new MainWindow;
        m_mainWindow->show();

    } catch (ConnectionExeption e) {
        qCritical() << QString(e.p);
        exit(EXIT_FAILURE);
    }
}

Application::~Application()
{
    delete m_mainWindow;
    delete m_settings;
}

void Application::initializeSettings()
{
    setOrganizationName(m_orgName);
    setApplicationName(m_appName);
    setApplicationVersion(m_version);

    m_settings = new QSettings(QSettings::NativeFormat,
                               QSettings::UserScope, m_orgName, m_appName);
}

void Application::initializeLanguage()
{
    QTranslator* translator = new QTranslator(this);

    if(translator->load("tpointer_ru_RU.qml", "/home/caxapyk/Projects/TPointer/translations")) {
        installTranslator(translator);
    }
}

void Application::about() const {
    QString message;
    message += "<h3>" + tr("Archival topographic pointer") + tr(" v %1").arg(version()) + "</h3>";
    message += "<p>" + tr("This program is free software: you can redistribute it and/or modify"
        " it under the terms of the GNU General Public License as published by"
        " the Free Software Foundation, either version 3 of the License, or"
    " (at your option) any later version.") + "</p>";

    message += "<p><span>" + tr("Application icons by ") + "<a href=https://icons8.com>" + "Icons8" + "</a>" + ".</span></p>";

    message += "<p>" + tr("Copyright &copy; 2020 Alexander Sakharuk") + "</p>";

    QMessageBox aboutBox;
    aboutBox.setWindowTitle(tr("Archival topographic pointer"));
    aboutBox.setText(message);
    aboutBox.setWindowIcon(icon());

    aboutBox.exec();
}
