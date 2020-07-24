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
    void select();

private:
    //QStandardItem *parentItem;
    QStandardItem *f_pre_soviet;
    QStandardItem *f_soviet;
    QStandardItem *f_consignment;
    QStandardItem *f_others;
};

#endif // FundReadonlyModel_H
