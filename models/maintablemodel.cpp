#include "maintablemodel.h"

MainTableModel::MainTableModel(QObject *parent, QSqlDatabase db) :  QSqlTableModel(parent, db)
{
    setTable("tpointer");
    setEditStrategy(QSqlTableModel::OnManualSubmit);
}
