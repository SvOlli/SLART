######################################################################
# src/tests/TestAppSatellite/TestAppSatellite.pro
# hand hacked by SvOlli
# released as public domain
######################################################################

TARGET = TestAppSatellite
TEMPLATE = app
QT += widgets network
DEFINES += SATELLITE_DEBUG=1 SATELLITESERVER_DEBUG=1

include( ../../buildconfig.pri )

INCLUDEPATH += $${TOPSRC}/libs/Satellite
LIBS += -lSatellite
contains( TARGETARCH, debug ) {
QT += widgets sql network
}

# Input
#RESOURCES = Resources.qrc

SOURCES += Application.cpp

HEADERS += MainWindow.hpp
SOURCES += MainWindow.cpp

HEADERS += MainWidget.hpp
SOURCES += MainWidget.cpp

HEADERS += ../../libs/Satellite/Satellite.hpp
SOURCES += ../../libs/Satellite/Satellite.cpp

HEADERS += ../../libs/Satellite/SatelliteServer.hpp
SOURCES += ../../libs/Satellite/SatelliteServer.cpp
