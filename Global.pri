######################################################################
# hand hacked by SvOlli
# global settings included by subdirs
######################################################################

QT += network
CONFIG += qt warn_all debug

DEPENDPATH += ../Common
INCLUDEPATH += ../Common
DESTDIR = ../bin

QMAKE_CXXFLAGS += -ffunction-sections -fdata-sections
QMAKE_LFLAGS += -Wl,--gc-sections

CONFIG(debug, debug|release)
{
   HEADERS += Trace.hpp
   SOURCES += Trace.cpp
   contains( QMAKE_CXX, g++ )
   {
      QMAKE_CXXFLAGS_DEBUG += -pedantic -Wno-long-long
   }
}

