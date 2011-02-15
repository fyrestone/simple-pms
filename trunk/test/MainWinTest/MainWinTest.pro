#-------------------------------------------------
#
# Project created by QtCreator 2011-02-14T11:10:47
#
#-------------------------------------------------

QT       += core gui

TARGET = MainWinTest
TEMPLATE = app


SOURCES += main.cpp \
    ../../src/gui/mainwin.cpp \
    ../../src/gui/aboutdlg.cpp

HEADERS  += \
    ../../src/gui/mainwin.h \
    ../../src/gui/aboutdlg.h \
    ../../src/gui/logic/mainwin_p.h \
    ../../src/pch.h

FORMS    += \
    ../../src/gui/mainwin.ui \
    ../../src/gui/aboutdlg.ui

RESOURCES += \
    ../../Resource.qrc
