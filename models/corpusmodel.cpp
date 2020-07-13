#include "corpusmodel.h"

#include <QSqlRecord>

CorpusModel::CorpusModel(QObject *parent) : BaseModel(parent)
{
    setTable("corpus");
    setItemName(tr("New corpus"));

    setEditStrategy(QSqlTableModel::OnFieldChange);
}

void CorpusModel::setDefaultRecord(int, QSqlRecord &record) {
    record.setValue(1, itemName());
    record.setGenerated(1, true);
};
