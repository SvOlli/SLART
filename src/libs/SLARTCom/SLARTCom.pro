######################################################################
# src/libs/SLARTCom/SLARTCom.pro
# hand hacked by SvOlli
# distributed as public domain
######################################################################

TARGET = SLARTCom
TEMPLATE = lib
CONFIG += qt staticlib
QT += network

include( ../../config.pri )

HEADERS += SLARTCom.hpp
SOURCES += SLARTCom.cpp

HEADERS += SLARTComServer.hpp
SOURCES += SLARTComServer.cpp

HEADERS += SLARTComServerRunner.hpp
SOURCES += SLARTComServerRunner.cpp
