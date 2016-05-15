# #####################################################################
# src/tests/TestAppDatabaseBrowser/TestAppDatabaseBrowser.pro
# hand hacked by SvOlli
# released as public domain
# #####################################################################

TARGET = TestAppDatabaseBrowser
TEMPLATE = app
QT += widgets sql network

include( ../../buildconfig.pri )

INCLUDEPATH += $${TOPSRC}/libs/Satellite
LIBS += -lSatellite
INCLUDEPATH += $${TOPSRC}/libs/Common
LIBS += -lCommon -lSatellite

# Input
# RESOURCES = Resources/TestAppDatabaseBrowser.qrc
SOURCES += Application.cpp

HEADERS += MainWidget.hpp
SOURCES += MainWidget.cpp
