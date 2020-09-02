#ifndef FEATURESDIALOG_H
#define FEATURESDIALOG_H

#include "models/tablemodel.h"
#include "views/tablemodelcontroller.h"
#include "widgets/buttonscontrol.h"

#include <QDialog>

namespace Ui {
class FeaturesDialog;
}

class FeaturesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FeaturesDialog(QWidget *parent = nullptr);
    ~FeaturesDialog();

private:
    void loadFeatures();

    Ui::FeaturesDialog *ui;
    TableModel *m_feature_model;
    ButtonsControl *ft_controls;
    TableModelController *controller;

private slots:
    void createItem(QAbstractItemView *view);
    void removeItem(QAbstractItemView *view);
    void moveUp(QAbstractItemView *view);
    void moveDown(QAbstractItemView *view);

};

#endif // FEATURESDIALOG_H
