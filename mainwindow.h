#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "dialogs/searchdialog.h"
#include "models/searchmodel.h"
#include "views/dataview.h"
#include "views/navigationview.h"

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

    DataView* dataView() { return m_dataView; };
    NavigationView* navView() { return m_navView; };
    void setDisplayRows(int count);
    void updateTotal();

    void dataLoaded();

private:
    Ui::MainWindow *ui;

    NavigationView *m_navView;
    DataView *m_dataView;

    QLabel *lb_server;
    QLabel *lb_total;

    SearchDialog *search_dialog = nullptr;

    void restoreAppState();
    void initialize();
    void setupStatusBar();

private slots:
    void insertNode();
    void openExportCsv();
    void openExportPdf();
    void openFundList();
    void openMainForm();
    void openParam();
    void openPrint();
    void openSearch();
    void search(const FilterStruct &filter);

protected:
    void closeEvent(QCloseEvent *event) override;
};
#endif // MAINWINDOW_H
