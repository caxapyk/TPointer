#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "controllers/modelcontroller.h"
#include "models/maintablemodel.h"
#include "models/maintableproxymodel.h"
#include "models/hierarchymodel.h"
#include "models/fundtreemodel.h"
#include "models/fundproxymodel.h"
#include "models/searchmodel.h"
#include "dialogs/searchdialog.h"
#include "widgets/itemfilter.h"
#include "widgets/itemcontroller.h"

#include <QCloseEvent>
#include <QLabel>
#include <QMainWindow>
#include <QModelIndex>
#include <QSqlQueryModel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    MainTableModel *m_table_model;
    MainTableProxyModel *m_table_proxymodel;
    ModelController *mt_controller;


    HierarchyModel *m_hierarchy_model;

    FundTreeModel *m_fund_model;
    FundProxyModel *m_fund_proxymodel;

    QLabel *lb_server;
    QLabel *lb_total;
    ItemFilter *fund_filter;
    ItemFilter *mt_filter; // main table filter
    ItemController *mt_controls;

    SearchDialog *search_dialog = nullptr;

    QVariant total;

    void restoreAppState();
    void initialize();
    void initializeMainTable();
    void setupStatusBar();
    void setDisplayRows(int rows);
    void setTotal(int total);
    void showHContextMenu(const QPoint &pos); // hierarchy context menu
    void showFContextMenu(const QPoint &pos); // funds context menu
    void showMTContextMenu(const QPoint &pos); // main table context menu

private slots:
    void loadByShelving(const QModelIndex &index);
    void loadByFund(const QModelIndex &index);
    void filterFunds(const QString &text);
    void filterMainTable(const QString &text);
    void openFundList();
    void openParam();
    void openSearch();
    void openMainForm();
    void rowSelected(const QItemSelection &selected, const QItemSelection &deselected);
    void search(const FilterStruct &sf);
    void createItem(QAbstractItemView *view);
    void removeItem(QAbstractItemView *view);

protected:
    void closeEvent(QCloseEvent *event) override;
};
#endif // MAINWINDOW_H
