######################################################################
# hand hacked by SvOlli
# global settings included by subdirs
######################################################################

QT += network
CONFIG += qt warn_all

DEPENDPATH += ../Common
INCLUDEPATH += ../Common
DESTDIR = ../bin

CONFIG += $$system( sh ./configure --config )

contains( QMAKE_CXX, g++ )
{
   QMAKE_CXXFLAGS_DEBUG += -pedantic -Wno-long-long
   QMAKE_CXXFLAGS += -ffunction-sections -fdata-sections
   QMAKE_LFLAGS += -Wl,--gc-sections
}

CONFIG(debug, debug|release)
{
   HEADERS += Trace.hpp
   SOURCES += Trace.cpp
}

