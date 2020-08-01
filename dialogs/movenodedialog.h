#ifndef MOVENODEDIALOG_H
#define MOVENODEDIALOG_H

#include "models/storageextendedmodel.h"
#include "models/noitemlistmodel.h"

#include <QDialog>
#include <QStringListModel>

namespace Ui {
class MoveNodeDialog;
}

class MoveNodeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MoveNodeDialog(QWidget *parent = nullptr);
    ~MoveNodeDialog();

private:
    Ui::MoveNodeDialog *ui;

    StorageExtendedModel *m_storageModel;
    NoItemListModel *m_nILstorageModel;
    QStringListModel *m_floorModel;

    void setupModels();
    void fillFloor(int index);
};

#endif // MOVENODEDIALOG_H
