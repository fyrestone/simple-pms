#-------------------------------------------------
#
# Project created by QtCreator 2011-02-11T11:25:09
#
#-------------------------------------------------

QT       += core gui sql

#project directory
PROJECT_DIR = E:/Project/PMS

#plugin lib directory
PLUGIN_DIR = $$PROJECT_DIR/plugin

TARGET = PMS
TEMPLATE = app

QTPLUGIN += qcncodecs qsqlite

LIBS += -L$$PLUGIN_DIR -lqcncodecs -lqsqlite

CONFIG += precompiled_header

PRECOMPILED_HEADER = pch.h

SOURCES += main.cpp\
        gui/logindlg.cpp \
    gui/mainwin.cpp \
    gui/tabpage.cpp \
    gui/aboutdlg.cpp \
    engine/dataengine.cpp \
    engine/abstracttask.cpp \
    context/context.cpp \
    gui/custom/navigationitem.cpp

HEADERS  += pch.h \
    gui/logindlg.h \
    gui/mainwin.h \
    gui/tabpage.h \
    gui/aboutdlg.h \
    engine/dataengine.h \
    engine/abstracttask.h \
    context/permission.h \
    context/context.h \
    gui/tabpage_p.h \
    gui/mainwin_p.h \
    gui/logindlg_p.h \
    gui/custom/navigationitem.h

FORMS    += gui/logindlg.ui \
    gui/mainwin.ui \
    gui/tabpage.ui \
    gui/aboutdlg.ui

RC_FILE = ../info/info.rc

RESOURCES += ../Resource.qrc
