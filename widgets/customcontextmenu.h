#ifndef CUSTOMCONTEXMENU_H
#define CUSTOMCONTEXMENU_H

#include <QObject>
#include <QItemSelection>
#include <QMenu>

class CustomContextMenu : public QMenu
{
    Q_OBJECT
public:
    enum StandardAction {
        Add                 = 0x01,
        Edit                = 0x02,
        Remove              = 0x04,
        Refresh             = 0x08,
        NoStandartAction    = 0x10
    };

    Q_DECLARE_FLAGS(StandardActions, StandardAction)
    Q_FLAG(StandardActions)

    CustomContextMenu(CustomContextMenu::StandardActions actions, QWidget *parent = nullptr);
    ~CustomContextMenu();
    QAction *action(CustomContextMenu::StandardAction which) const;
    void setEnabled(bool b, CustomContextMenu::StandardAction which);
    void setCurrentSelection(const QItemSelection &selected);

signals:
    void addRequested();
    void editRequested();
    void removeRequested();
    void refreshRequested();

private:
    QAction *action_add = nullptr;
    QAction *action_edit = nullptr;
    QAction *action_remove = nullptr;
    QAction *action_refresh = nullptr;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(CustomContextMenu::StandardActions)

#endif // CUSTOMCONTEXMENU_H
