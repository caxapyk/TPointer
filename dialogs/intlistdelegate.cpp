#include "intlistdelegate.h"

#include <QLineEdit>
#include <QRegExp>
#include <QRegExpValidator>
#include <QValidator>

IntListDelegate::IntListDelegate(QObject *parent) : QItemDelegate(parent)
{

}

QWidget *IntListDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &) const
{
    QLineEdit *editor = new QLineEdit(parent);

    //Comma separated integer from -9-99 or blank line
    QRegExp rx("^((-[1-9]|[0]|[1-9]|([1-9][0-9]))(,(-[1-9]|[0]|[1-9]|([1-9][0-9])))*)?$");
    QValidator *validator = new QRegExpValidator(rx, parent);

    editor->setValidator(validator);

    return editor;
}
