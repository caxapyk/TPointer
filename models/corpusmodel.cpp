#include "corpusmodel.h"

#include <QSqlRecord>

CorpusModel::CorpusModel() : BaseModel()
{
    setTable("corpus");
    setItemName(tr("New corpus"));

    setEditStrategy(QSqlTableModel::OnFieldChange);
}

void CorpusModel::setDefaultRecord(int, QSqlRecord &record) {
    record.setValue(1, itemName());
    record.setGenerated(1, true);
};
