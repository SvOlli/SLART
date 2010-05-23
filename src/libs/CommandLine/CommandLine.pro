######################################################################
# src/libs/CommandLine/CommandLine.pro
# hand hacked by SvOlli
# distributed as public domain
######################################################################

TARGET = CommandLine
TEMPLATE = lib
CONFIG += qt staticlib

include( ../../buildconfig.pri )

HEADERS += CommandLine.hpp
SOURCES += CommandLine.cpp

HEADERS += CommandLineOptionBool.hpp
SOURCES += CommandLineOptionBool.cpp

HEADERS += CommandLineOption.hpp
SOURCES += CommandLineOption.cpp

HEADERS += CommandLineOptionString.hpp
SOURCES += CommandLineOptionString.cpp

