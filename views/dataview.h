#ifndef DATAVIEW_H
#define DATAVIEW_H

#include "view.h"
#include "tablemodelcontroller.h"
#include "models/filterstruct.h"
#include "models/datamodel.h"
#include "models/dataproxymodel.h"
#include "widgets/buttonscontrol.h"
#include "utils/templatehtml.h"

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
    DataModel *model() { return m_model; };
    void clearView();

private:
    Ui::DataView *ui;

    DataModel *m_model;
    DataProxyModel *m_proxyModel;

    ButtonsControl *m_buttonsControl;
    TableModelController *m_tableController;

    void initialize() override;
    void printF(TemplateHtml &templ, QMap<QString, QVariant> vars);

public slots:
    void filterMainTable(const QString &text);
    void printF15();
    void printF16();
    void exportCsv();

private slots:
    void addItem();
    void editItem();

    void moveItems();
    void refresh();
    void removeItems();
    void showContextMenu(const QPoint &pos); // data table context menu
    void showMetaData(const QItemSelection &selected, const QItemSelection &deselected);
};

#endif // DATAVIEW_H
