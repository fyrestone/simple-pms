#-------------------------------------------------
#
# Project created by QtCreator 2011-02-14T11:10:47
#
#-------------------------------------------------

QT       += core gui sql

TARGET = MainWinTest
TEMPLATE = app


SOURCES += main.cpp \
    ../../src/gui/mainwin.cpp \
    ../../src/gui/aboutdlg.cpp \
    ../../src/engine/dataengine.cpp \
    ../../src/engine/abstracttask.cpp \
    ../../src/context/context.cpp \
    ../../src/gui/custom/navigationitem.cpp

HEADERS  += \
    ../../src/gui/mainwin.h \
    ../../src/gui/aboutdlg.h \
    ../../src/pch.h \
    ../../src/gui/mainwin_p.h \
    ../../src/engine/dataengine.h \
    ../../src/engine/abstracttask.h \
    ../../src/context/permission.h \
    ../../src/context/context.h \
    ../../src/gui/custom/navigationitem.h

FORMS    += \
    ../../src/gui/mainwin.ui \
    ../../src/gui/aboutdlg.ui

RESOURCES += \
    ../../Resource.qrc
