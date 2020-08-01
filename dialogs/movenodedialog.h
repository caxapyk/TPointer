#ifndef MOVENODEDIALOG_H
#define MOVENODEDIALOG_H

#include <QDialog>

namespace Ui {
class MoveNodeDialog;
}

class MoveNodeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MoveNodeDialog(QWidget *parent = nullptr);
    ~MoveNodeDialog();

private:
    Ui::MoveNodeDialog *ui;
};

#endif // MOVENODEDIALOG_H
