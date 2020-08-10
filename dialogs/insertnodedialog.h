#ifndef INSERTNODEDIALOG_H
#define INSERTNODEDIALOG_H

#include "nodedialog.h"
#include "models/datamodel.h"

class InsertNodeDialog : public NodeDialog
{
    Q_OBJECT

public:
    InsertNodeDialog();
    DataModel *dataModel() { return m_dataModel; };
    void setDataModel(DataModel *model) { m_dataModel = model; };

protected:
    void revert() override;
    void save() override;

private:
    DataModel *m_dataModel;
};

#endif // INSERTNODEDIALOG_H
