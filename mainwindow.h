#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "dialogs/searchdialog.h"
#include "models/searchmodel.h"
#include "views/dataview.h"
#include "views/navigationview.h"
#include "widgets/itemfilter.h"

#include <QCloseEvent>
#include <QHBoxLayout>
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

    void clearMTableFilter();
    DataView* dataView() { return m_dataView; };
    QAction* getMenuAction(const QString &name);
    ItemFilter* itemFilter() { return m_itemFilter; };
    NavigationView* navView() { return m_navView; };

    void setExportCsvEnabled(bool e);
    void setPrintF15Enabled(bool e);
    void setPrintF16Enabled(bool e);
    void setDisplayRows(int count);
    void updateTotal();

private:
    Ui::MainWindow *ui;

    NavigationView *m_navView;
    DataView *m_dataView;

    QWidget *tb_panel;
    QHBoxLayout *tb_layout;
    ItemFilter *m_itemFilter; // main table filter

    QLabel *lb_server;
    QLabel *lb_total;

    SearchDialog *search_dialog = nullptr;

    void initialize();
    void restoreAppState();
    void setupStatusBar();
    void setupToolBar();
    void setupItemFilter();

private slots:
    void insertNode();
    void openConnection();
    void openFeatures();
    void openFundList();
    void openMainForm();
    void openParam();
    void openSearch();
    void openSync();
    void search(const FilterStruct &filter);

protected:
    void closeEvent(QCloseEvent *event) override;
};
#endif // MAINWINDOW_H
