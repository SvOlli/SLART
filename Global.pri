######################################################################
# hand hacked by SvOlli
# global settings included by subdirs
######################################################################

QT += network
CONFIG += qt warn_all debug

DEPENDPATH += ../Common
INCLUDEPATH += ../Common
DESTDIR = ../bin

CONFIG(debug, debug|release)
{
   HEADERS += Trace.hpp
   SOURCES += Trace.cpp
}

