#-------------------------------------------------
#
# Project created by QtCreator 2011-02-21T17:04:34
#
#-------------------------------------------------

QT       += core gui sql

#project directory
PROJECT_DIR = E:\Project\PMS

#plugin lib directory
PLUGIN_DIR = $$PROJECT_DIR\plugin

TARGET = TransDB
TEMPLATE = app

QTPLUGIN += qcncodecs qsqlite

LIBS += -L$$PLUGIN_DIR -lqcncodecs -lqsqlite

SOURCES += main.cpp\
        widget.cpp \
    ../../src/engine/dataengine.cpp \
    ../../src/engine/abstracttask.cpp

HEADERS  += widget.h \
    ../../src/engine/dataengine.h \
    ../../src/engine/abstracttask.h

FORMS    += widget.ui
