#ifndef FILTERSTRUCT_H
#define FILTERSTRUCT_H

#include <QVariant>

struct FilterStruct {
    QVariant corpus;
    QVariant storage;
    QVariant compartment;
    QVariant shelving;
    QVariant fund;
    QVariant fund_name;
    bool fund_strict;
    QVariant feature;
    bool isSearch = false;
};

#endif // FILTERSTRUCT_H
