######################################################################
# src/tests/TestAppWebServer/TestAppWebServer.pro
# hand hacked by SvOlli
# released as public domain
######################################################################

TARGET = TestAppWebServer
TEMPLATE = app
QT += sql network

include( ../../buildconfig.pri )

INCLUDEPATH += $${TOPSRC}/libs/Common
LIBS += -lCommon
contains( TARGETARCH, debug ) {
QT += sql network
}

# Input
RESOURCES = Resources/TestAppWebServer.qrc

SOURCES += Application.cpp

HEADERS += MainWidget.hpp
SOURCES += MainWidget.cpp

