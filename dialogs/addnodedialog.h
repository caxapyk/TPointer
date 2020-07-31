#ifndef ADDNODEDIALOG_H
#define ADDNODEDIALOG_H

#include "dialogs/insertnodedialog.h"

#include <QObject>

class AddNodeDialog : public InsertNodeDialog
{
    Q_OBJECT
public:
    AddNodeDialog();
    void applyFilter(const FilterStruct &fs);
};

#endif // ADDNODEDIALOG_H
