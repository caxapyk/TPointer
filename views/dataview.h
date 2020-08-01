#ifndef DATAVIEW_H
#define DATAVIEW_H

#include "view.h"
#include "tablemodelcontroller.h"
#include "models/filterstruct.h"
#include "models/datamodel.h"
#include "models/dataproxymodel.h"
#include "widgets/itemfilter.h"
#include "widgets/buttonscontrol.h"

#include <QWidget>
#include <QAbstractItemView>

namespace Ui {
class DataView;
}

class DataView : public View
{
    Q_OBJECT

public:
    explicit DataView(QWidget *parent = nullptr);
    ~DataView();


    void restoreViewState() override;
    void saveViewState() override;

    void loadData(const FilterStruct &filter);

private:
    Ui::DataView *ui;

    DataModel *m_model;
    DataProxyModel *m_proxyModel;

    ItemFilter *m_itemFilter; // main table filter
    ButtonsControl *m_buttonsControl;
    TableModelController *m_tableController;

    void initialize() override;

private slots:
    void addItem();
    void filterMainTable(const QString &text);
    void moveItems();
    void removeItems();
    void showContextMenu(const QPoint &pos); // data table context menu
    void showMetaData(const QItemSelection &selected, const QItemSelection &deselected);
};

#endif // DATAVIEW_H
