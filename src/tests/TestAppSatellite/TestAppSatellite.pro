######################################################################
# src/tests/TestAppSatellite/TestAppSatellite.pro
# hand hacked by SvOlli
# released as public domain
######################################################################

TARGET = TestAppSatellite
TEMPLATE = app
QT += network

include( ../../buildconfig.pri )

INCLUDEPATH += $${TOPSRC}/libs/Satellite
LIBS += -lSatellite
contains( TARGETARCH, debug ) {
QT += sql network
}

# Input
#RESOURCES = Resources.qrc

SOURCES += Application.cpp

HEADERS += MainWindow.hpp
SOURCES += MainWindow.cpp

HEADERS += MainWidget.hpp
SOURCES += MainWidget.cpp

