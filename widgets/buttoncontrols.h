#ifndef BUTTONCONTROLS_H
#define BUTTONCONTROLS_H

#include <QWidget>
#include <QButtonGroup>
#include <QPushButton>
#include <QModelIndex>

namespace Ui {
class ButtonControls;
}

class ButtonControls : public QWidget
{
    Q_OBJECT

public:
    explicit ButtonControls(int orientation = Qt::Vertical, QWidget *parent = nullptr);
    ~ButtonControls();

    QPushButton* buttonAdd();
    QPushButton* buttonRemove();
    QPushButton* buttonUp();
    QPushButton* buttonDown();

    void disable();

public slots:
    void modelSelected(const QModelIndex &index);

private:
    Ui::ButtonControls *ui;
    QButtonGroup *group;

};

#endif // BUTTONCONTROLS_H
