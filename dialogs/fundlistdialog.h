#ifndef FUNDLISTDIALOG_H
#define FUNDLISTDIALOG_H

#include "views/tablemodelcontroller.h"
#include "models/fundmodel.h"
#include "models/fundproxymodel.h"
#include "widgets/buttonscontrol.h"
#include "widgets/itemfilter.h"

#include <QDialog>
#include <QItemSelectionModel>
#include <QItemSelection>

namespace Ui {
class FundListDialog;
}

class FundListDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FundListDialog(QWidget *parent = nullptr);
    ~FundListDialog();

    QVariant selectedPrimaryKey() const { return (pK) ? QVariant(pK): QVariant(); };
    void setSigleRowSelection();

private:
    Ui::FundListDialog *ui;
    FundModel *f_model;
    FundProxyModel *f_proxymodel;
    ItemFilter *filter;

    ButtonsControl *controls;
    TableModelController *controller;

    int pK = -1;

private slots:
    void filterFunds(const QString &text);
    void createItem(QAbstractItemView *view);
    void removeItem(QAbstractItemView *view);
    void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
};

#endif // FUNDLISTDIALOG_H
