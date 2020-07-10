#ifndef COMMALISTDELEGATE_H
#define COMMALISTDELEGATE_H

#include <QItemDelegate>
#include <QObject>
#include <QValidator>
#include <QWidget>

class CommaListDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    CommaListDelegate(QObject *parent = nullptr);
    ~CommaListDelegate();

private:
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // COMMALISTDELEGATE_H
