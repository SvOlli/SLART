######################################################################
# src/libs/WebServer/WebServer.pro
# hand hacked by SvOlli
# distributed as public domain
######################################################################

TARGET = WebServer
TEMPLATE = lib
CONFIG += qt staticlib
QT += network

include( ../../config.pri )

HEADERS += WebServer.hpp
SOURCES += WebServer.cpp

