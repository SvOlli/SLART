######################################################################
# src/tests/TestAppWebServer/TestAppWebServer.pro
# hand hacked by SvOlli
# released as public domain
######################################################################

TARGET = TestAppXxtea
TEMPLATE = app
QT -= gui

include( ../../buildconfig.pri )

INCLUDEPATH += $${TOPSRC}/libs/Common $${TOPSRC}/libs/CommandLine
LIBS += -lCommon -lCommandLine

# Input
#RESOURCES = Resources.qrc

SOURCES += Application.cpp
