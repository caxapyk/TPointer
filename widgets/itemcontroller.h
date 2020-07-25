#ifndef BUTTONCONTROLS_H
#define BUTTONCONTROLS_H

#include <QWidget>
#include <QAction>
#include <QAbstractItemView>
#include <QButtonGroup>
#include <QItemSelection>
#include <QPushButton>
#include <QModelIndex>
#include <QBoxLayout>
#include <QDialogButtonBox>
#include <QMenu>

class ItemController : public QWidget
{
    Q_OBJECT

public:
    enum StandardControl {
        Add     = 0x01,
        Edit    = 0x02,
        Remove  = 0x04,
        Up      = 0x08,
        Down    = 0x10,
        Refresh = 0x40
    };

    Q_DECLARE_FLAGS(StandardControls, StandardControl)
    Q_FLAG(StandardControls)

    explicit ItemController(ItemController::StandardControls buttons, int orientation = Qt::Vertical, QWidget *parent = nullptr);
    ~ItemController();

    QPushButton *button(ItemController::StandardControl which) const;
    void setEnabled(bool b, ItemController::StandardControl which);
    void assetView(QAbstractItemView *view);
    void connectToMenu(ItemController::StandardControl control, QAction *action);

signals:
    void addRequested(QAbstractItemView *view);
    void editRequested(QAbstractItemView *view);
    void removeRequested(QAbstractItemView *view);
    void upRequested(QAbstractItemView *view);
    void downRequested(QAbstractItemView *view);
    void refreshRequested(QAbstractItemView *view);

private:
    QBoxLayout *layout_box;
    QPushButton *pB_add = nullptr;
    QPushButton *pB_edit = nullptr;
    QPushButton *pB_remove = nullptr;
    QPushButton *pB_up = nullptr;
    QPushButton *pB_down = nullptr;
    QPushButton *pB_refresh = nullptr;

    QAbstractItemView *m_view = nullptr;

    QMap<ItemController::StandardControl, QPushButton*> *bmap = new QMap<ItemController::StandardControl, QPushButton*>;
    QMap<ItemController::StandardControl, QAction*> *actions =  new QMap<ItemController::StandardControl, QAction*>;

private slots:
    void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
};

Q_DECLARE_OPERATORS_FOR_FLAGS(ItemController::StandardControls)

#endif // BUTTONCONTROLS_H
