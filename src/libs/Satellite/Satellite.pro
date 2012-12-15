######################################################################
# src/libs/Satellite/Satellite.pro
# hand hacked by SvOlli
# distributed as public domain
######################################################################

TARGET = Satellite
TEMPLATE = lib
CONFIG += qt staticlib
QT += network

include( ../../buildconfig.pri )

HEADERS += Satellite.hpp
SOURCES += Satellite.cpp

HEADERS += SatelliteServer.hpp
SOURCES += SatelliteServer.cpp

HEADERS += SatelliteSingleSend.hpp
SOURCES += SatelliteSingleSend.cpp
