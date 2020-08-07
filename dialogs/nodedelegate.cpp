#include "nodedelegate.h"
#include "models/datamodel.h"

#include <QDebug>
#include <QComboBox>
#include <QLineEdit>
#include <QPlainTextEdit>

#include <QtCore/qmetaobject.h>

NodeDelegate::NodeDelegate() : QItemDelegate()
{

}

QWidget *NodeDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &index) const
{
    QWidget *editor = nullptr;

    switch (index.column()) {
    case DataModel::Corpus:
    case DataModel::Storage:
    case DataModel::Fund:
    case DataModel::Features:
        editor = new QComboBox(parent);
        break;
    case DataModel::Note:
        editor = new QPlainTextEdit(parent);
        break;
   default:
        editor = new QLineEdit(parent);
        break;
    }

    return editor;
}

void NodeDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if (!index.isValid())
        return;

    if (qobject_cast<QComboBox *>(editor)
            || qobject_cast<QLineEdit*>(editor)
            || qobject_cast<QPlainTextEdit*>(editor)) {
        // Taken from QItemDelegate::setEditorData() as we need
        // to present the DisplayRole and not the EditRole which
        // is the id reference to the related model

        QVariant v = index.data(Qt::DisplayRole);

        const QByteArray n = editor->metaObject()->userProperty().name();
        if (!n.isEmpty()) {
            if (!v.isValid())
                v = QVariant(editor->property(n.data()).userType(), nullptr);
            editor->setProperty(n.data(), v);
            return;
        }
    }

    QItemDelegate::setEditorData(editor, index);
}
