#include "intdelegate.h"

#include <QLineEdit>
#include <QRegExp>
#include <QIntValidator>

IntDelegate::IntDelegate(int min, int max, QObject *parent) : QItemDelegate(parent)
{
    m_min = min;
    m_max = max;
}

QWidget *IntDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &) const
{
    QLineEdit *editor = new QLineEdit(parent);
    QValidator *validator = new QIntValidator(min(), max());

    editor->setValidator(validator);

    return editor;
}
