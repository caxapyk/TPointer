#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "models/maintablemodel.h"
#include "models/hierarchymodel.h"
#include "models/fundmodel.h"

#include <QCloseEvent>
#include <QLabel>
#include <QMainWindow>
#include <QModelIndex>

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
    MainTableModel *m_mainTableModel;
    HierarchyModel *m_hierarchy_model;
    FundModel *m_fund_model;

    QLabel *lb_server;

    const unsigned int m_descColumn = 10;

    void restoreAppState();
    void setupStatusBar();
    void loadData(const QModelIndex &index);

private slots:
    void openStructureDialog();
    void rowSelected(const QModelIndex &current, const QModelIndex&);

protected: // overrides
    void closeEvent( QCloseEvent* e );
};
#endif // MAINWINDOW_H
