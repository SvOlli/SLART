# #####################################################################
# src/tests/TestAppDatabaseBrowser/TestAppDatabaseBrowser.pro
# hand hacked by SvOlli
# released as public domain
# #####################################################################
TARGET = TestAppDatabaseBrowser
TEMPLATE = app
QT += sql \
    network
include( ../../buildconfig.pri )
INCLUDEPATH += ../../libs/Common
LIBS += -lCommon

# Input
# RESOURCES = Resources/TestAppDatabaseBrowser.qrc
SOURCES += Application.cpp

HEADERS += MainWidget.hpp
SOURCES += MainWidget.cpp
