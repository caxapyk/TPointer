#ifndef NOITEMLISTMODEL_H
#define NOITEMLISTMODEL_H

#include <QObject>
#include <QAbstractListModel>

class NoItemListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    NoItemListModel();
    ~NoItemListModel();
    int baseColumn() const { return m_col; };
    void clear();
    QAbstractItemModel* baseModel() const { return m_model; };
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    void setColumn(int column){ m_col=column; };
    void setModel(QAbstractItemModel *model){ m_model=model; };

private:
    QAbstractItemModel *m_model;
    int m_col;
};

#endif // NOITEMLISTMODEL_H
