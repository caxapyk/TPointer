#ifndef FUNDMODEL_H
#define FUNDMODEL_H

#include <QObject>
#include <QStandardItemModel>

class FundModel : public QStandardItemModel
{
    Q_OBJECT
public:
    FundModel();
    void select();
};

#endif // FUNDMODEL_H
