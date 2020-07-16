QT       += core gui sql

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
    dialogs/commalistdelegate.cpp \
    dialogs/structuredialog.cpp \
    main.cpp \
    mainwindow.cpp \
    models/basemodel.cpp \
    models/corpusmodel.cpp \
    models/fundmodel.cpp \
    models/hierarchymodel.cpp \
    models/maintablemodel.cpp \
    models/storagemodel.cpp

HEADERS += \
    application.h \
    connection.h \
    dialogs/commalistdelegate.h \
    dialogs/structuredialog.h \
    mainwindow.h \
    models/basemodel.h \
    models/corpusmodel.h \
    models/fundmodel.h \
    models/hierarchymodel.h \
    models/maintablemodel.h \
    models/storagemodel.h

FORMS += \
    dialogs/structuredialog.ui \
    mainwindow.ui

TRANSLATIONS += \
    translations/tpointer_en.ts \
    translations/tpointer_ru.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    translations/tpointer_en.ts \
    translations/tpointer_ru.ts

RESOURCES += \
    resources.qrc
