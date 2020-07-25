#ifndef FUNDLISTDIALOG_H
#define FUNDLISTDIALOG_H

#include "controllers/modelcontroller.h"
#include "models/fundmodel.h"
#include "models/fundproxymodel.h"
#include "widgets/itemcontroller.h"
#include "widgets/itemfilter.h"

#include <QDialog>

namespace Ui {
class FundListDialog;
}

class FundListDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FundListDialog(QWidget *parent = nullptr);
    ~FundListDialog();

private:
    Ui::FundListDialog *ui;
    FundModel *f_model;
    FundProxyModel *f_proxymodel;
    ItemFilter *filter;

    ItemController *controls;
    ModelController *controller;

private slots:
    void filterFunds(const QString &text);
    void createItem(QAbstractItemView *view);
    void removeItem(QAbstractItemView *view);
};

#endif // FUNDLISTDIALOG_H
