#include "corpusmodel.h"

#include <QSqlRecord>

CorpusModel::CorpusModel() : TableModel()
{
    setTable("corpus");
    setItemName(tr("New corpus"));

    setEditStrategy(QSqlTableModel::OnRowChange);
}

void CorpusModel::setDefaultRecord(int, QSqlRecord &record) {
    record.setValue(1, itemName());
    record.setGenerated(1, true);
};
