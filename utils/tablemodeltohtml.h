#ifndef TABLEMODELTOHTML_H
#define TABLEMODELTOHTML_H

#include "models/datamodel.h"

#include <QFile>
#include <QObject>

#include <QWebEngineView>
#include <QWebEnginePage>


class TableModelToHtml : public QObject
{
    Q_OBJECT
public:
    explicit TableModelToHtml(const QString &file, const QString &templ, DataModel *model, QObject *parent = nullptr);
    ~TableModelToHtml();
    void save();
    QWebEngineView *printHtmlView;

private:
    QAbstractItemModel *m_model;
    QFile m_file;
    QFile m_templ;

private slots:
    void process(bool ok);
};

#endif // TABLEMODELTOHTML_H
