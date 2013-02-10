######################################################################
# src/tests/TestAppXxtea/TestAppXxtea.pro
# hand hacked by SvOlli
# released as public domain
######################################################################

TARGET = TestAppXxtea
TEMPLATE = app
QT -= gui

include( ../../buildconfig.pri )

INCLUDEPATH += $${TOPSRC}/libs/Common $${TOPSRC}/libs/CommandLine
LIBS += -lCommon -lCommandLine
contains( TARGETARCH, debug ) {
QT += sql network
}

# Input
#RESOURCES = Resources.qrc

SOURCES += Application.cpp
