#ifndef INSERTNODEDIALOG_H
#define INSERTNODEDIALOG_H

#include "nodedialog.h"

class InsertNodeDialog : public NodeDialog
{
    Q_OBJECT

public:
    InsertNodeDialog();

protected:
    void revert() override;
    void save() override;
};

#endif // INSERTNODEDIALOG_H
