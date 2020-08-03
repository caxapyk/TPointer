#ifndef MAINFORMDIALOG_H
#define MAINFORMDIALOG_H

#include "dialogs/fundlistdialog.h"
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

private:
    void restoreDialogState();
    void setupModels();
    void fillFloor(int index);
    void selectFund();

protected:
    Ui::NodeDialog *ui;

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
