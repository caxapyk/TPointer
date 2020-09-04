#ifndef SYNCDIALOG_H
#define SYNCDIALOG_H

#include <models/corpusmodel.h>

#include <QDialog>

namespace Ui {
class SyncDialog;
}

class SyncDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SyncDialog(QWidget *parent = nullptr);
    ~SyncDialog();

private:
    Ui::SyncDialog *ui;
    CorpusModel *m_corpusModel;

private slots:
    void sync();
};

#endif // SYNCDIALOG_H
