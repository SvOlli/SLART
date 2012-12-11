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

#TRANSLATIONS += $${TOPSRC}/translations/$${TARGET}.ts
#TRANSLATIONS += $${TOPSRC}/translations/$${TARGET}_de.ts

HEADERS += Satellite.hpp
SOURCES += Satellite.cpp

HEADERS += SatelliteServer.hpp
SOURCES += SatelliteServer.cpp

HEADERS += SatelliteServerRunner.hpp
SOURCES += SatelliteServerRunner.cpp
