#include "itemcontroller.h"

#include <QDebug>

ItemController::ItemController(ItemController::StandardControls buttons, int orientation, QWidget *parent) : QWidget(parent)
{
    if (orientation == Qt::Horizontal) {
        layout_box = new QHBoxLayout(this);
    } else {
        layout_box = new QVBoxLayout(this);
    }

    layout_box->setSpacing(6);
    layout_box->setContentsMargins(0,0,0,0);
    layout_box->setSizeConstraint(QLayout::SetDefaultConstraint);

    // setup button group
    group = new QButtonGroup;

    // add button
    if(buttons & ItemController::Add) {
        pB_add = new QPushButton();
        pB_add->setEnabled(false);
        pB_add->setText(tr("Add"));
        pB_add->setIcon(QIcon(":/icons/plus-16.png"));

        group->addButton(pB_add);
        layout_box->addWidget(pB_add);
    }

    // edit button
    if(buttons & ItemController::Edit) {
        pB_edit = new QPushButton();
        pB_edit->setEnabled(false);
        pB_edit->setText(tr("Edit"));
        pB_edit->setIcon(QIcon(":/icons/edit-16.png"));

        group->addButton(pB_edit);
        layout_box->addWidget(pB_edit);
    }

    // remove button
    if(buttons & ItemController::Remove) {
        pB_remove = new QPushButton();
        pB_remove->setEnabled(false);
        pB_remove->setText(tr("Remove"));
        pB_remove->setIcon(QIcon(":/icons/delete-16.png"));

        group->addButton(pB_remove);
        layout_box->addWidget(pB_remove);
    }

    // up button
    if(buttons & ItemController::Up) {
        pB_up = new QPushButton();
        pB_up->setEnabled(false);
        pB_up->setText(tr("Up"));
        pB_up->setIcon(QIcon(":/icons/arrow_down-16.png"));

        group->addButton(pB_up);
        layout_box->addWidget(pB_up);
    }

    // down button
    if(buttons & ItemController::Down) {
        pB_down = new QPushButton();
        pB_down->setEnabled(false);
        pB_down->setText(tr("Down"));
        pB_down->setIcon(QIcon(":/icons/arrow-top-16.png"));

        group->addButton(pB_down);
        layout_box->addWidget(pB_down, 0, Qt::AlignTop);
    }

    // refresh button
    if(buttons & ItemController::Refresh) {
        pB_refresh = new QPushButton();
        pB_refresh->setEnabled(false);
        pB_refresh->setText(tr("Refresh"));
        pB_refresh->setIcon(QIcon(":/icons/types-16.png"));

        group->addButton(pB_refresh);
        layout_box->addWidget(pB_refresh, 0, Qt::AlignTop);
    }

    setLayout(layout_box);
}

ItemController::~ItemController()
{
    delete layout_box;

    delete pB_add;
    delete pB_edit;
    delete pB_remove;
    delete pB_up;
    delete pB_down;
    delete pB_refresh;

    delete group;
}

QPushButton *ItemController::button(ItemController::StandardControl which) const
{
    switch (which) {
    case ItemController::Add:
        return pB_add;
        break;
    case ItemController::Edit:
        return pB_edit;
        break;
    case ItemController::Remove:
        return  pB_remove;
        break;
    case ItemController::Up:
        return pB_up;
        break;
    case ItemController::Down:
        return pB_down;
        break;
    case ItemController::Refresh:
        return pB_refresh;
        break;
    default:
        return nullptr;
    }
}

void ItemController::assetView(QAbstractItemView *view)
{
    connect(view->selectionModel(), &QItemSelectionModel::selectionChanged, this, &ItemController::selectionChanged);
}

void ItemController::selectionChanged(const QItemSelection &selected, const QItemSelection&)
{
    setEnabled(!selected.isEmpty(), ItemController::Edit);
    setEnabled(!selected.isEmpty(), ItemController::Remove);
    setEnabled(!selected.isEmpty(), ItemController::Up);
    setEnabled(!selected.isEmpty(), ItemController::Down);
}

void ItemController::setEnabled(bool b, ItemController::StandardControl which) {
    if(button(which)) {
        button(which)->setEnabled(b);
    }

    if(actions->contains(which)){
        QAction *action = actions->value(which);
        action->setEnabled(b);
    }
}

void ItemController::connectToMenu(ItemController::StandardControl control, QAction *action)
{
    actions->insert(control, action);

    // disable current action
    action->setDisabled(true);
}
