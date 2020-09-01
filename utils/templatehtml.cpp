#include "templatehtml.h"

#include <QTextDocument>
#include <QPrinter>
#include <QPrintDialog>
#include <QTextStream>
#include <QDebug>
#include <QFile>

TemplateHtml::TemplateHtml(const QString &path, QObject *parent) : QObject(parent)
{
    QFile tmpl(path + ".html");
    QFile stylesheet(path + ".css");

    if (tmpl.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream tmpl_stream(&tmpl);
        tmpl_stream.setCodec("UTF-8");
        m_html = tmpl_stream.readAll();

        tmpl.close();
    }

    if (stylesheet.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stylesheet_stream(&stylesheet);
        stylesheet_stream.setCodec("UTF-8");
        m_stylesheet = stylesheet_stream.readAll();

        stylesheet.close();
    }
}

QString TemplateHtml::makeTableRows(QAbstractItemModel *model, const QVector<int> &columns)
{
    QString t;
    for(int i = 0; i < model->rowCount(); ++i) {
        t.append("<tr>");
        for(int j = 0; j < columns.length(); ++j) {
            t.append(
                        QString("<td>%1</td>")
                        .arg(model->index(i, columns.at(j)).data().toString()));
        }
        t.append("</tr>");
    }

    return t;
}

void TemplateHtml::setVars(const QMap<QString, QVariant> &vars)
{
    for(int i = 0; i < vars.size(); ++i) {
        m_html.replace(vars.keys().at(i), vars.values().at(i).toString());
    }
}

void TemplateHtml::print()
{
    QPrinter printer(QPrinter::ScreenResolution);
    printer.setPageMargins(5, 5, 5, 5, QPrinter::Millimeter);

    QPrintDialog printDialog(&printer);
    if (printDialog.exec() == QDialog::Accepted) {
        QTextDocument td;
        td.setPageSize(QSizeF(printer.pageRect().size()));
        td.setDefaultStyleSheet(m_stylesheet);
        td.setHtml(m_html);

        td.print(&printer);
    }
}
