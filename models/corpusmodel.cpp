#include "corpusmodel.h"

#include <QSqlRecord>

CorpusModel::CorpusModel() : TableModel()
{
    setTable("corpus");
    setEditStrategy(QSqlTableModel::OnRowChange);
}

void CorpusModel::setDefaultRecord(int, QSqlRecord &record) {
    record.setValue(1, tr("Corpus %1").arg(itemMaxNum(1)));
    record.setGenerated(1, true);
};
