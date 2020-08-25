#include "buttonscontrol.h"

#include <QDebug>

ButtonsControl::ButtonsControl(ButtonsControl::StandardControls buttons, int orientation, QWidget *parent) : QWidget(parent)
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
    if(buttons & ButtonsControl::Add) {
        pB_add = new QPushButton();
        pB_add->setEnabled(false);
        pB_add->setText(tr("Add"));
        pB_add->setIcon(QIcon(":/icons/add-16.png"));

        bmap->insert(ButtonsControl::Add, pB_add);
        layout_box->addWidget(pB_add);

        connect(pB_add, &QPushButton::pressed, this, [=] { emit addRequested(m_view); });
    }

    // edit button
    if(buttons & ButtonsControl::Edit) {
        pB_edit = new QPushButton();
        pB_edit->setEnabled(false);
        pB_edit->setText(tr("Edit"));
        pB_edit->setIcon(QIcon(":/icons/edit-16.png"));

        bmap->insert(ButtonsControl::Edit, pB_edit);
        layout_box->addWidget(pB_edit);

        connect(pB_edit, &QPushButton::pressed, this, [=] { emit editRequested(m_view); });
    }

    // remove button
    if(buttons & ButtonsControl::Remove) {
        pB_remove = new QPushButton();
        pB_remove->setEnabled(false);
        pB_remove->setText(tr("Remove"));
        pB_remove->setIcon(QIcon(":/icons/delete-16.png"));

        bmap->insert(ButtonsControl::Remove, pB_remove);
        layout_box->addWidget(pB_remove);

        connect(pB_remove, &QPushButton::pressed, this, [=] { emit removeRequested(m_view); });
    }

    // up button
    if(buttons & ButtonsControl::Up) {
        pB_up = new QPushButton();
        pB_up->setEnabled(false);
        pB_up->setText(tr("Up"));
        pB_up->setIcon(QIcon(":/icons/arrow_down-16.png"));

        bmap->insert(ButtonsControl::Up, pB_up);
        layout_box->addWidget(pB_up);

        connect(pB_up, &QPushButton::pressed, this, [=] { emit upRequested(m_view); });
    }

    // down button
    if(buttons & ButtonsControl::Down) {
        pB_down = new QPushButton();
        pB_down->setEnabled(false);
        pB_down->setText(tr("Down"));
        pB_down->setIcon(QIcon(":/icons/arrow-top-16.png"));

        bmap->insert(ButtonsControl::Down, pB_down);
        layout_box->addWidget(pB_down);

        connect(pB_down, &QPushButton::pressed, this, [=] { emit downRequested(m_view); });
    }

    // refresh button
    if(buttons & ButtonsControl::Refresh) {
        pB_refresh = new QPushButton();
        pB_refresh->setEnabled(false);
        pB_refresh->setText(tr("Refresh"));
        pB_refresh->setIcon(QIcon(":/icons/types-16.png"));

        bmap->insert(ButtonsControl::Refresh, pB_refresh);
        layout_box->addWidget(pB_refresh);

        connect(pB_refresh, &QPushButton::pressed, this, [=] { emit refreshRequested(m_view); });
    }

    setLayout(layout_box);
}

ButtonsControl::~ButtonsControl()
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

QPushButton *ButtonsControl::button(ButtonsControl::StandardControl which) const
{
    return bmap->value(which);
}

void ButtonsControl::assetView(QAbstractItemView *view)
{
    m_view = view;
    connect(view->selectionModel(), &QItemSelectionModel::selectionChanged, this, &ButtonsControl::selectionChanged);
}

void ButtonsControl::selectionChanged(const QItemSelection &selected, const QItemSelection&)
{
    /* clear if selected is empty
     * needed to fix disabled buttons on deselection
     */
    if(selected.isEmpty()) {
        m_view->setCurrentIndex(QModelIndex());
    }

    setEnabled(!selected.isEmpty(), ButtonsControl::Edit);
    setEnabled(!selected.isEmpty(), ButtonsControl::Remove);
    setEnabled(!selected.isEmpty(), ButtonsControl::Up);
    setEnabled(!selected.isEmpty(), ButtonsControl::Down);
}

void ButtonsControl::setEnabled(bool b, ButtonsControl::StandardControl which) {
    if(button(which)) {
        button(which)->setEnabled(b);
    }

    if(actions->contains(which)){
        QAction *action = actions->value(which);
        action->setEnabled(b);
    }
}

void ButtonsControl::connectToMenu(ButtonsControl::StandardControl control, QAction *action)
{
    actions->insert(control, action);

    connect(button(control), &QPushButton::pressed, this, [=] { emit action->triggered(); });

    // disable current action
    action->setDisabled(true);
}
