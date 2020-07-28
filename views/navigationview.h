#ifndef NAVIGATIONVIEW_H
#define NAVIGATIONVIEW_H

#include <QWidget>

namespace Ui {
class NavigationView;
}

class NavigationView : public QWidget
{
    Q_OBJECT

public:
    explicit NavigationView(QWidget *parent = nullptr);
    ~NavigationView();

private:
    Ui::NavigationView *ui;
};

#endif // NAVIGATIONVIEW_H
