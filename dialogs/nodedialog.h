#ifndef MAINFORMDIALOG_H
#define MAINFORMDIALOG_H

#include "dialogs/fundlistdialog.h"
#include "models/datamodel.h"
#include "models/storageextendedmodel.h"
#include "models/fundmodel.h"
#include "models/featuremodel.h"
#include "models/noitemlistmodel.h"

#include <QComboBox>
#include <QDialog>
#include <QStringListModel>

namespace Ui {
class NodeDialog;
}

class NodeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NodeDialog(QWidget *parent = nullptr);
    ~NodeDialog();

    void setDataModel(DataModel *model) { m_dataModel = model; };
    //void setModelIndex(const QModelIndex &index) { m_modelIndex = index; };

private:
    void restoreDialogState();
    void setupModels();
    void fillFloor(int index);
    void selectFund();

    //QModelIndex m_modelIndex;

protected:
    Ui::NodeDialog *ui;
    DataModel *m_dataModel;

    StorageExtendedModel *m_storageModel;
    NoItemListModel *m_nILstorageModel;
    QStringListModel *m_floorModel;
    FundModel *m_fundModel;
    NoItemListModel *m_nILfundModel;
    FeatureModel *m_featureModel;
    NoItemListModel *m_nILfeatureModel;

    void closeEvent(QCloseEvent *event) override;

};

#endif // MAINFORMDIALOG_H
