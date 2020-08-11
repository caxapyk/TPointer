QT       += core gui sql printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    application.cpp \
    connection.cpp \
    dialogs/addnodedialog.cpp \
    dialogs/editnodedialog.cpp \
    dialogs/fundlistdialog.cpp \
    dialogs/insertnodedialog.cpp \
    dialogs/intdelegate.cpp \
    dialogs/intlistdelegate.cpp \
    dialogs/movenodedialog.cpp \
    dialogs/nodedelegate.cpp \
    dialogs/nodedialog.cpp \
    dialogs/paramdialog.cpp \
    dialogs/searchdialog.cpp \
    main.cpp \
    mainwindow.cpp \
    models/corpusmodel.cpp \
    models/datamodel.cpp \
    models/dataproxymodel.cpp \
    models/featuremodel.cpp \
    models/fundmodel.cpp \
    models/fundproxymodel.cpp \
    models/fundtreemodel.cpp \
    models/hierarchymodel.cpp \
    models/noitemlistmodel.cpp \
    models/searchmodel.cpp \
    models/storageextendedmodel.cpp \
    models/storagemodel.cpp \
    models/tablemodel.cpp \
    utils/floorsparser.cpp \
    utils/fundsorter.cpp \
    utils/tableprinter.cpp \
    views/dataview.cpp \
    views/navigationview.cpp \
    views/tablemodelcontroller.cpp \
    views/view.cpp \
    widgets/buttonscontrol.cpp \
    widgets/customcontextmenu.cpp \
    widgets/itemfilter.cpp

HEADERS += \
    application.h \
    connection.h \
    dialogs/addnodedialog.h \
    dialogs/editnodedialog.h \
    dialogs/fundlistdialog.h \
    dialogs/insertnodedialog.h \
    dialogs/intdelegate.h \
    dialogs/intlistdelegate.h \
    dialogs/movenodedialog.h \
    dialogs/nodedelegate.h \
    dialogs/nodedialog.h \
    dialogs/paramdialog.h \
    dialogs/searchdialog.h \
    mainwindow.h \
    models/corpusmodel.h \
    models/datamodel.h \
    models/dataproxymodel.h \
    models/featuremodel.h \
    models/filterstruct.h \
    models/fundmodel.h \
    models/fundproxymodel.h \
    models/fundtreemodel.h \
    models/hierarchymodel.h \
    models/noitemlistmodel.h \
    models/searchmodel.h \
    models/storageextendedmodel.h \
    models/storagemodel.h \
    models/tablemodel.h \
    utils/floorsparser.h \
    utils/fundsorter.h \
    utils/tableprinter.h \
    views/dataview.h \
    views/navigationview.h \
    views/tablemodelcontroller.h \
    views/view.h \
    widgets/buttonscontrol.h \
    widgets/customcontextmenu.h \
    widgets/itemfilter.h

FORMS += \
    dialogs/fundlistdialog.ui \
    dialogs/movenodedialog.ui \
    dialogs/nodedialog.ui \
    dialogs/paramdialog.ui \
    dialogs/searchdialog.ui \
    mainwindow.ui \
    views/dataview.ui \
    views/navigationview.ui

TRANSLATIONS += \
    translations/tpointer_ru.ts \
    translations/tpointer_en.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

RESOURCES += \
    resources.qrc
