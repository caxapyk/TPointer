#ifndef FundReadonlyModel_H
#define FundReadonlyModel_H

#include <QObject>
#include <QStandardItemModel>

class FundTreeModel : public QStandardItemModel
{
    Q_OBJECT
public:
    FundTreeModel();
    ~FundTreeModel();
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    void select();

private:
    QStandardItem *f_pre_soviet;
    QStandardItem *f_soviet;
    QStandardItem *f_consignment;
    QStandardItem *f_others;
};

#endif // FundReadonlyModel_H
