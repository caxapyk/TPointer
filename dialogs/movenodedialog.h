#ifndef MOVENODEDIALOG_H
#define MOVENODEDIALOG_H

#include "models/storagemodel.h"
#include "models/noitemlistmodel.h"
#include "models/corpusmodel.h"
#include "models/datamodel.h"

#include <QDialog>
#include <QStringListModel>
#include <QModelIndexList>

namespace Ui {
class MoveNodeDialog;
}

class MoveNodeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MoveNodeDialog(QWidget *parent = nullptr);
    ~MoveNodeDialog();

    DataModel *dataModel() { return m_dataModel; };
    void setDataModel(DataModel *model) { m_dataModel = model; };
    QModelIndexList indexes() { return m_indexes; };
    void setIndexes(QModelIndexList &indexes) { m_indexes = indexes; };

private:
    Ui::MoveNodeDialog *ui;

    DataModel *m_dataModel;

    CorpusModel *m_corpusModel;
    StorageModel *m_storageModel;
    QStringListModel *m_floorModel;

    QModelIndexList m_indexes;

    void setupModels();
    void fillFloor(int index);
    void fillCompartment(int index);
    void fillStorage(int index);
    void save();
};

#endif // MOVENODEDIALOG_H
