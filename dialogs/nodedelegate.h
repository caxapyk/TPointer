#ifndef NODEDELEGATE_H
#define NODEDELEGATE_H

#include <QItemDelegate>
#include <QObject>

class NodeDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    NodeDelegate();

private:
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
};

#endif // NODEDELEGATE_H
