#ifndef FundReadonlyModel_H
#define FundReadonlyModel_H

#include "fundmodel.h"

#include <QObject>
#include <QStandardItemModel>

class FundTreeModel : public QStandardItemModel
{
    Q_OBJECT
public:
    FundTreeModel();
    ~FundTreeModel();
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant getStorage() { return storageId; };
    QVariant getCompartment() { return compart; };
    void select();
    void setFilterByStorageCompartment(const QVariant &storage, const QVariant &comp = QVariant());
    FundModel *sourceModel() { return m_model; };

private:
    void initialize();

    FundModel *m_model;

    QStandardItem *f_pre_soviet;
    QStandardItem *f_soviet;
    QStandardItem *f_consignment;
    QStandardItem *f_others;

    QVariant storageId;
    QVariant compart;
};

#endif // FundReadonlyModel_H
