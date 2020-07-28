#include "itemcontroller.h"

#include <QDebug>

ItemController::ItemController(ItemController::StandardControls buttons, int orientation, QWidget *parent) : QWidget(parent)
{
    if (orientation == Qt::Horizontal) {
        layout_box = new QBoxLayout(QBoxLayout::LeftToRight);
    } else {
        layout_box = new QBoxLayout(QBoxLayout::TopToBottom);
    }

    layout_box->setSpacing(6);
    layout_box->setContentsMargins(0,0,0,0);
    layout_box->setAlignment(Qt::AlignTop);

    // add button
    if(buttons & ItemController::Add) {
        pB_add = new QPushButton();
        pB_add->setEnabled(false);
        pB_add->setText(tr("Add"));
        pB_add->setIcon(QIcon(":/icons/plus-16.png"));

        bmap->insert(ItemController::Add, pB_add);
        layout_box->addWidget(pB_add);

        connect(pB_add, &QPushButton::pressed, this, [=] { emit addRequested(m_view); });
    }

    // edit button
    if(buttons & ItemController::Edit) {
        pB_edit = new QPushButton();
        pB_edit->setEnabled(false);
        pB_edit->setText(tr("Edit"));
        pB_edit->setIcon(QIcon(":/icons/edit-16.png"));

        bmap->insert(ItemController::Edit, pB_edit);
        layout_box->addWidget(pB_edit);

        connect(pB_edit, &QPushButton::pressed, this, [=] { emit editRequested(m_view); });
    }

    // remove button
    if(buttons & ItemController::Remove) {
        pB_remove = new QPushButton();
        pB_remove->setEnabled(false);
        pB_remove->setText(tr("Remove"));
        pB_remove->setIcon(QIcon(":/icons/delete-16.png"));

        bmap->insert(ItemController::Remove, pB_remove);
        layout_box->addWidget(pB_remove);

        connect(pB_remove, &QPushButton::pressed, this, [=] { emit removeRequested(m_view); });
    }

    // up button
    if(buttons & ItemController::Up) {
        pB_up = new QPushButton();
        pB_up->setEnabled(false);
        pB_up->setText(tr("Up"));
        pB_up->setIcon(QIcon(":/icons/arrow_down-16.png"));

        bmap->insert(ItemController::Up, pB_up);
        layout_box->addWidget(pB_up);

        connect(pB_up, &QPushButton::pressed, this, [=] { emit upRequested(m_view); });
    }

    // down button
    if(buttons & ItemController::Down) {
        pB_down = new QPushButton();
        pB_down->setEnabled(false);
        pB_down->setText(tr("Down"));
        pB_down->setIcon(QIcon(":/icons/arrow-top-16.png"));

        bmap->insert(ItemController::Down, pB_down);
        layout_box->addWidget(pB_down);

        connect(pB_down, &QPushButton::pressed, this, [=] { emit downRequested(m_view); });
    }

    // refresh button
    if(buttons & ItemController::Refresh) {
        pB_refresh = new QPushButton();
        pB_refresh->setEnabled(false);
        pB_refresh->setText(tr("Refresh"));
        pB_refresh->setIcon(QIcon(":/icons/types-16.png"));

        bmap->insert(ItemController::Refresh, pB_refresh);
        layout_box->addWidget(pB_refresh);

        connect(pB_refresh, &QPushButton::pressed, this, [=] { emit refreshRequested(m_view); });
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

    delete bmap;

    delete m_view;
}

QPushButton *ItemController::button(ItemController::StandardControl which) const
{
    return bmap->value(which);
}

void ItemController::assetView(QAbstractItemView *view)
{
    m_view = view;
    connect(view->selectionModel(), &QItemSelectionModel::selectionChanged, this, &ItemController::selectionChanged);
}

void ItemController::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    /* clear if selected is empty
     * needed to fix disabled buttons on deselection
     */
    if(selected.isEmpty()) {
        m_view->setCurrentIndex(QModelIndex());
    }

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

    connect(button(control), &QPushButton::pressed, this, [=] { emit action->triggered(); });

    // disable current action
    action->setDisabled(true);
}
