######################################################################
# src/tests/TestAppWebServer/TestAppWebServer.pro
# hand hacked by SvOlli
# released as public domain
######################################################################

TARGET = TestAppWebServer
TEMPLATE = app
QT += sql network

include( ../../buildconfig.pri )

INCLUDEPATH += $${TOPSRC}/libs/Satellite
LIBS += -lSatellite
INCLUDEPATH += $${TOPSRC}/libs/Common
LIBS += -lCommon
contains( TARGETARCH, debug ) {
QT += sql network
}

# Input
RESOURCES = Resources/TestAppWebServer.qrc

HEADERS += ../../libs/Common/ScgiRequest.hpp
SOURCES += ../../libs/Common/ScgiRequest.cpp

HEADERS += ../../libs/Common/WebServer.hpp
SOURCES += ../../libs/Common/WebServer.cpp

SOURCES += Application.cpp

HEADERS += MainWidget.hpp
SOURCES += MainWidget.cpp

HEADERS += WebServerSanityTests.hpp
SOURCES += WebServerSanityTests.cpp

