#ifndef TEMPLATEHTML_H
#define TEMPLATEHTML_H

#include <QAbstractItemModel>
#include <QMap>
#include <QObject>

class TemplateHtml : public QObject
{
    Q_OBJECT
public:
    explicit TemplateHtml(const QString &path, QObject *parent = nullptr);
    QString makeTableRows(QAbstractItemModel *model, const QVector<int> &columns);
    void setVars(const QMap<QString, QVariant> &vars);
    void print();

signals:

private:
    QString m_html;
    QString m_stylesheet;
};

#endif // TEMPLATEHTML_H
