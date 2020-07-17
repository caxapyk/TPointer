#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "models/maintablemodel.h"
#include "models/hierarchymodel.h"
#include "models/fundmodel.h"
#include "dialogs/searchdialog.h"

#include <QCloseEvent>
#include <QLabel>
#include <QMainWindow>
#include <QModelIndex>
#include <QSortFilterProxyModel>

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

    const unsigned int m_descColumn = 10;
    unsigned int total = 0;

    void restoreAppState();
    void setupModels();
    void setupStatusBar();

    void fillMainTable();

private slots:
    void treeObjectSelected(const QModelIndex &index);
    void filterFunds(const QString &text);
    void clearFundFilter();
    void openParamDialog();
    void openSearchDialog();
    void rowSelected(const QModelIndex &current, const QModelIndex&);

protected:
    void closeEvent(QCloseEvent *event) override;
};
#endif // MAINWINDOW_H
