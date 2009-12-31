######################################################################
# src/tests/TestAppCommandLine/TestAppCommandLine.pro
# hand hacked by SvOlli
# released as public domain
######################################################################

TARGET = TestAppCommandLine
TEMPLATE = app
QT -= gui

include( ../../config.pri )

INCLUDEPATH += ../../libs/CommandLine
LIBS += -lCommandLine

# Input
SOURCES += Application.cpp 

