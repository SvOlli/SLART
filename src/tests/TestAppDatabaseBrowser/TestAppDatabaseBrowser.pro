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
INCLUDEPATH += $${TOPSRC}/libs/Common
LIBS += -lCommon
contains( TARGETARCH, debug ) {
QT += sql network
}

# Input
# RESOURCES = Resources/TestAppDatabaseBrowser.qrc
SOURCES += Application.cpp

HEADERS += MainWidget.hpp
SOURCES += MainWidget.cpp
