#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "models/maintablemodel.h"

#include <QCloseEvent>
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
    const unsigned int m_descColumn = 10;

    void restoreAppState();
    void loadData();

private slots:
    void openStructureDialog();
    void rowSelected(const QModelIndex &current, const QModelIndex&);

protected: // overrides
    void closeEvent( QCloseEvent* e );
};
#endif // MAINWINDOW_H
