#ifndef ITEMFILTER_H
#define ITEMFILTER_H

#include <QObject>

#include <QBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>

class ItemFilter : public QWidget
{
    Q_OBJECT
public:
    ItemFilter(QWidget* parent = nullptr);
    ~ItemFilter();
    void setPlaceholderText(const QString &text);
    void clear();

signals:
    void filtered(const QString &text);

private:
    void filter(const QString &text);

    QBoxLayout *layout;
    QLineEdit *lE_filter;
    QPushButton *pB_clearFilter;
};

#endif // ITEMFILTER_H
