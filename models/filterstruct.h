#ifndef FILTERSTRUCT_H
#define FILTERSTRUCT_H

#include <QVariant>

struct FilterStruct {
    QVariant corpus;
    QVariant storage;
    QVariant compartment;
    QVariant shelving;
    QVariant fund;
    bool fund_strict;
    QVariant feature;
};

#endif // FILTERSTRUCT_H
