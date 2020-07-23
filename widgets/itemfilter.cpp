#include "itemfilter.h"

ItemFilter::ItemFilter(QWidget* parent) : QWidget(parent)
{
    layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    lE_filter = new QLineEdit(this);
    lE_filter->setPlaceholderText(tr("Search..."));

    layout->addWidget(lE_filter);

    pB_clearFilter = new QPushButton(this);
    pB_clearFilter->setIcon(QIcon(":/icons/clear-filter-16.png"));
    pB_clearFilter->setToolTip(tr("Clear filter"));
    pB_clearFilter->setDisabled(true);
    pB_clearFilter->setFlat(true);

    layout->addWidget(pB_clearFilter);

    setLayout(layout);

    connect(lE_filter, &QLineEdit::textChanged, this, &ItemFilter::filter);
    connect(pB_clearFilter, &QPushButton::released, this, &ItemFilter::clear);
}

ItemFilter::~ItemFilter()
{
    delete layout;
    delete lE_filter;
    delete pB_clearFilter;
}

void ItemFilter::filter(const QString &text)
{
    pB_clearFilter->setEnabled(text.length() > 0);
    emit filtered(text);
}

void ItemFilter::clear()
{
    lE_filter->clear();
    pB_clearFilter->setEnabled(false);
}

void ItemFilter::setPlaceholderText(const QString &text)
{
    lE_filter->setPlaceholderText(text);
}
