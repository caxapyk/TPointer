#include "customcontextmenu.h"

#include <QItemSelectionModel>
#include <QtDebug>

CustomContextMenu::CustomContextMenu(CustomContextMenu::StandardActions actions, QWidget *parent) : QMenu(parent)
{
    // add action
    if(actions & CustomContextMenu::Add) {
        action_add = addAction(tr("Add"));
        action_add->setIcon(QIcon(":/icons/add-16.png"));
        connect(action_add, &QAction::triggered, this, [=] { emit addRequested(); });
    }

    // edit action
    if(actions & CustomContextMenu::Edit) {
        action_edit = addAction(tr("Edit"));
        action_edit->setIcon(QIcon(":/icons/edit-16.png"));
        connect(action_edit, &QAction::triggered, this, [=] { emit editRequested(); });
    }

    // remove action
    if(actions & CustomContextMenu::Remove) {
        action_remove = addAction(tr("Remove"));
        action_remove->setIcon(QIcon(":/icons/delete-16.png"));
        connect(action_remove, &QAction::triggered, this, [=] { emit removeRequested(); });
    }

    // refresh action
    if(actions & CustomContextMenu::Refresh) {
        action_refresh = addAction(tr("Refresh"));
        action_refresh->setIcon(QIcon(":/icons/refresh-16.png"));
        connect(action_refresh, &QAction::triggered, this, [=] { emit refreshRequested(); });
    }
}

CustomContextMenu::~CustomContextMenu()
{
    delete action_add;
    delete action_edit;
    delete action_remove;
    delete action_refresh;
}

QAction *CustomContextMenu::action(CustomContextMenu::StandardAction which) const
{
    switch (which) {
    case CustomContextMenu::Add:
        return action_add;
        break;
    case CustomContextMenu::Edit:
        return action_edit;
        break;
    case CustomContextMenu::Remove:
        return action_remove;
        break;
    case CustomContextMenu::Refresh:
        return action_refresh;
        break;
    default:
        return nullptr;
    }
}

void CustomContextMenu::setSelection(const QModelIndexList &selected)
{
    //disable edit action for more then one row selected
    setEnabled(selected.length() == 1, CustomContextMenu::Edit);

    setEnabled(!selected.isEmpty(), CustomContextMenu::Remove);
}

void CustomContextMenu::setEnabled(bool b, CustomContextMenu::StandardAction which) {
    if(action(which)) {
        action(which)->setEnabled(b);
    }
}
