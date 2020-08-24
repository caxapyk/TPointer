#include "tablemodeltohtml.h"

#include <QUrl>

TableModelToHtml::TableModelToHtml(const QString &file, const QString &templ, DataModel *model, QObject *parent) : QObject(parent)
{
    m_file.setFileName(file);
    m_templ.setFileName(templ);
    m_model = model;

    printHtmlView = new QWebEngineView;
}

TableModelToHtml::~TableModelToHtml()
{
    printHtmlView->close();
    delete printHtmlView;
}

void TableModelToHtml::save()
{
    if (!m_templ.open(QIODevice::ReadWrite | QIODevice::Text))
             return;

    QTextStream in(&m_templ);
    QString html = in.readAll();

    printHtmlView->setHtml(html);
    //printHtmlView->load(QUrl("https://en.wikipedia.org/wiki/Main_Page"));
    connect(printHtmlView, &QWebEngineView::loadFinished, this, &TableModelToHtml::process);
}

void TableModelToHtml::process(bool) {
    qDebug() << "save";
    printHtmlView->page()->printToPdf(m_file.fileName());
}
