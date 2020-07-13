#include "commalistdelegate.h"

#include <QItemDelegate>
#include <QLineEdit>
#include <QObject>
#include <QRegExp>
#include <QRegExpValidator>
#include <QValidator>

CommaListDelegate::CommaListDelegate(QObject *parent) : QItemDelegate(parent)
{

}

QWidget *CommaListDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &) const
{
    QLineEdit *editor = new QLineEdit(parent);

    //Comma separated floors from 1-19 or blank line
    QRegExp rx("^(([1-9]|([1-9][0-9]))(,([1-9]|([1-9][0-9])))*)?$");
    QValidator *validator = new QRegExpValidator(rx, parent);

    editor->setValidator(validator);

    return editor;
}

CommaListDelegate::~CommaListDelegate(){}
