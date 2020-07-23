#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include "models/storageextendedmodel.h"
#include "models/featuremodel.h"
#include "models/noitemlistmodel.h"
#include "models/filterstruct.h"

#include <QDialog>

namespace Ui {
class SearchDialog;
}

class SearchDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SearchDialog(QWidget *parent = nullptr);
    ~SearchDialog();

signals:
    void searched(const FilterStruct &fs);

protected:
    void closeEvent(QCloseEvent *e) override;
    void showEvent(QShowEvent *event) override;

private:
    Ui::SearchDialog *ui;
    StorageExtendedModel *storage_model;
    NoItemListModel *_storage_model;
    FeatureModel *feature_model;
    NoItemListModel *_feature_model;

    FilterStruct fs;

private slots:
    void search();
};

#endif // SEARCHDIALOG_H
