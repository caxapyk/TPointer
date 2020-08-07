#ifndef ADDNODEDIALOG_H
#define ADDNODEDIALOG_H

#include "dialogs/insertnodedialog.h"
#include "models/filterstruct.h"

#include <QObject>

class AddNodeDialog : public InsertNodeDialog
{
    Q_OBJECT

public:
    AddNodeDialog();
    void applyFilter(const FilterStruct &fs);

protected:
    void revert() override;
    void save() override;
};

#endif // ADDNODEDIALOG_H
