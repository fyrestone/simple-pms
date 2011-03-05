#-------------------------------------------------
#
# Project created by QtCreator 2011-02-11T11:25:09
#
#-------------------------------------------------

QT       += core gui sql

TARGET = PMS
TEMPLATE = app

QTPLUGIN += qcncodecs qsqlite

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
    gui/custom/navigationitem.cpp \
    gui/addclasswizard.cpp \
    gui/addgradewizard.cpp \
    gui/selectgradewizardpage.cpp \
    gui/inputclassinfowizardpage.cpp \
    gui/inputgradeinfowizardpage.cpp

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
    gui/custom/navigationitem.h \
    gui/addclasswizard.h \
    gui/addgradewizard.h \
    gui/selectgradewizardpage.h \
    gui/inputclassinfowizardpage.h \
    gui/inputgradeinfowizardpage.h \
    gui/inputgradeinfowizardpage_p.h

FORMS    += gui/logindlg.ui \
    gui/mainwin.ui \
    gui/tabpage.ui \
    gui/aboutdlg.ui \
    gui/classmgmtdlg.ui \
    gui/addclasswizard.ui \
    gui/addgradewizard.ui \
    gui/selectgradewizardpage.ui \
    gui/inputclassinfowizardpage.ui \
    gui/inputgradeinfowizardpage.ui

RC_FILE = ../info/info.rc

RESOURCES += ../Resource.qrc
