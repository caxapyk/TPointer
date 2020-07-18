#ifndef INTLISTDELEGATE_H
#define INTLISTDELEGATE_H

#include <QItemDelegate>
#include <QObject>
#include <QWidget>

class IntListDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    IntListDelegate(QObject *parent = nullptr);

private:
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // INTLISTDELEGATE_H
