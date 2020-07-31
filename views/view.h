#ifndef VIEW_H
#define VIEW_H

#include <QWidget>

class View : public QWidget
{
    Q_OBJECT
public:
    explicit View(QWidget *parent = nullptr);
    ~View();
    QWidget *mainWidget() { return m_mainWidget; };

    void virtual restoreViewState(){};
    void virtual saveViewState(){};

protected:
    void virtual initialize(){};
    void setMainWidget(QWidget *widget) { m_mainWidget=widget; };

private:
    QWidget *m_mainWidget = nullptr;
};

#endif // VIEW_H
