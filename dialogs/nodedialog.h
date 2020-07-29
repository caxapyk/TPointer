#ifndef MAINFORMDIALOG_H
#define MAINFORMDIALOG_H

#include <QDialog>

namespace Ui {
class MainFormDialog;
}

class MainFormDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MainFormDialog(QWidget *parent = nullptr);
    ~MainFormDialog();

private:
    Ui::MainFormDialog *ui;
};

#endif // MAINFORMDIALOG_H
