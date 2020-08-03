#ifndef INTDELEGATE_H
#define INTDELEGATE_H

#include <QItemDelegate>
#include <QObject>
#include <QWidget>

class IntDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    IntDelegate(int min = 1, int max = 99);
    int min() const { return m_min; };
    int max() const { return m_max; };

private:
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    int m_min;
    int m_max;
};

#endif // INTDELEGATE_H
