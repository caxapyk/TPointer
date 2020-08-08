#ifndef ADDNODEDIALOG_H
#define ADDNODEDIALOG_H

#include "dialogs/insertnodedialog.h"
#include "models/datamodel.h"
#include "models/filterstruct.h"

#include <QObject>

class AddNodeDialog : public InsertNodeDialog
{
    Q_OBJECT

public:
    AddNodeDialog(DataModel *model);
    void applyFilter(const FilterStruct &fs);

protected:
    void revert() override;
    void save() override;

private:
    DataModel *m_dataModel;
};

#endif // ADDNODEDIALOG_H
