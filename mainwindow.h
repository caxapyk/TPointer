#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "models/maintablemodel.h"
#include "models/hierarchymodel.h"
#include "models/fundmodel.h"
#include "models/searchmodel.h"
#include "dialogs/searchdialog.h"

#include <QCloseEvent>
#include <QLabel>
#include <QMainWindow>
#include <QModelIndex>
#include <QSortFilterProxyModel>
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
    HierarchyModel *m_hierarchy_model;
    FundModel *m_fund_model;
    QSortFilterProxyModel *m_fund_proxymodel;

    QLabel *lb_server;
    QLabel *lb_total;

    SearchDialog *search_dialog = nullptr;

    unsigned int total = 0;

    void restoreAppState();
    void setupModels();
    void setupStatusBar();
    void setDisplayRows(int rows);

private slots:
    void loadByShelving(const QModelIndex &index);
    void loadByFund(const QModelIndex &index);
    void filterFunds(const QString &text);
    void clearFundFilter();
    void openParamDialog();
    void openSearchDialog();
    void rowSelected(const QModelIndex &current, const QModelIndex&);
    void search(const FilterStruct &sf);

protected:
    void closeEvent(QCloseEvent *event) override;
};
#endif // MAINWINDOW_H
