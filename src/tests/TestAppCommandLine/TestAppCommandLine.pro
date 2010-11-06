######################################################################
# src/tests/TestAppCommandLine/TestAppCommandLine.pro
# hand hacked by SvOlli
# released as public domain
######################################################################

TARGET = TestAppCommandLine
TEMPLATE = app
QT -= gui

include( ../../buildconfig.pri )

INCLUDEPATH += $${TOPSRC}/libs/CommandLine
LIBS += -lCommandLine

# Input
SOURCES += Application.cpp

