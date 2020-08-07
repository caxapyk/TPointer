#ifndef MAINFORMDIALOG_H
#define MAINFORMDIALOG_H

#include "dialogs/fundlistdialog.h"
#include "models/corpusmodel.h"
#include "models/fundmodel.h"
#include "models/featuremodel.h"
#include "models/noitemlistmodel.h"
#include "models/storagemodel.h"

#include <QComboBox>
#include <QDialog>
#include <QStringListModel>
#include <QValidator>

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
    void fillFloor(int index);
    void fillStorage(int index);
    void restoreDialogState();
    void selectFund();
    void setupModels();
    void setupValidators();
protected:
    Ui::NodeDialog *ui;

    CorpusModel *m_corpusModel;
    NoItemListModel *m_nILcorpusModel;
    StorageModel *m_storageModel;
    QStringListModel *m_floorModel;
    FundModel *m_fundModel;
    NoItemListModel *m_nILfundModel;
    FeatureModel *m_featureModel;
    NoItemListModel *m_nILfeatureModel;

    QValidator *validatorCompartment;
    QValidator *validatorCupboard;
    QValidator *validatorShelving;

    void closeEvent(QCloseEvent *event) override;
    void virtual cancel() {};
    void virtual revert() {};
    void virtual save() {};
};

#endif // MAINFORMDIALOG_H
