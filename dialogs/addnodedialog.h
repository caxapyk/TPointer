#ifndef ADDNODEDIALOG_H
#define ADDNODEDIALOG_H

#include "dialogs/insertnodedialog.h"
#include "models/filterstruct.h"

#include <QDataWidgetMapper>
#include <QObject>

class AddNodeDialog : public InsertNodeDialog
{
    Q_OBJECT

public:
    AddNodeDialog(DataModel *model);
    void applyFilter(const FilterStruct &fs);

protected:
    void revert() override;
};

#endif // ADDNODEDIALOG_H
