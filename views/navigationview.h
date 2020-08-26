#ifndef NAVIGATIONVIEW_H
#define NAVIGATIONVIEW_H

#include "view.h"
#include "models/hierarchymodel.h"
#include "models/fundtreemodel.h"
#include "models/fundproxymodel.h"
#include "widgets/itemfilter.h"

#include <QWidget>

namespace Ui {
class NavigationView;
}

class NavigationView : public View
{
    Q_OBJECT

public:
    explicit NavigationView(QWidget *parent = nullptr);
    ~NavigationView();

    void restoreViewState() override;
    void saveViewState() override;

    void resetCurrentIndexes();

private:
    Ui::NavigationView *ui;

    HierarchyModel *m_hierarchy_model;

    FundTreeModel *m_fund_model;
    FundTreeModel *m_fundc_model; // current funds model
    FundProxyModel *m_fund_proxymodel;
    FundProxyModel *m_fundc_proxymodel;

    ItemFilter *fund_filter;

    void initialize() override;

private slots:
    void filterFunds(const QString &text);
    void fundActivated(const QModelIndex &index);
    void fundTabChanged(int index);
    void hierarchyActivated(const QModelIndex &index);
    void showHContextMenu(const QPoint &pos); // hierarchy context menu
    void showFContextMenu(const QPoint &pos); // funds context menu
};

#endif // NAVIGATIONVIEW_H
