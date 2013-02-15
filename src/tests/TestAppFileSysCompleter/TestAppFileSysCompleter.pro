######################################################################
# src/tests/TestAppFileSysCompleter/TestAppFileSysCompleter.pro
# hand hacked by SvOlli
# released as public domain
######################################################################

TARGET = TestAppFileSysCompleter
TEMPLATE = app

include( ../../buildconfig.pri )

INCLUDEPATH += $${TOPSRC}/libs/Common $${TOPSRC}/libs/CommandLine
LIBS += -lCommon -lCommandLine
contains( TARGETARCH, debug ) {
QT += sql network
}

# Input
#RESOURCES = Resources.qrc

SOURCES += Application.cpp
