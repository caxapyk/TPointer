#ifndef EDITNODEDIALOG_H
#define EDITNODEDIALOG_H

#include "nodedialog.h"
#include "models/datamodel.h"

#include <QDataWidgetMapper>
#include <QObject>

class EditNodeDialog : public NodeDialog
{
    Q_OBJECT
public:
    EditNodeDialog(DataModel *model, int index);
    ~EditNodeDialog();

    DataModel *dataModel() { return m_dataModel; };

protected:
    void save() override;

private:
    DataModel *m_dataModel;
    QDataWidgetMapper *mapper;

    void setDataModel(DataModel *model) { m_dataModel = model; };
};

#endif // EDITNODEDIALOG_H
