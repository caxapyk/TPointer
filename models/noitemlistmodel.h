#ifndef NOITEMLISTMODEL_H
#define NOITEMLISTMODEL_H

#include <QObject>
#include <QAbstractListModel>

class NoItemListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    NoItemListModel(QAbstractItemModel *baseModel, QObject *parent = nullptr);
    ~NoItemListModel();
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QAbstractItemModel *baseModel() const { return m_model; };
    void setColumn(int column){ m_col=column; };
    int baseColumn() const { return m_col; };

private:
    QAbstractItemModel *m_model;
    int m_col;
};

#endif // NOITEMLISTMODEL_H
