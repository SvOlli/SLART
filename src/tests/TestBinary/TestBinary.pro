######################################################################
# src/tests/TestBinary/TestBinary.pro
# hand hacked by SvOlli
# released as public domain
######################################################################

TARGET = TestBinary
TEMPLATE = app
QT -= gui
QMAKE_CXXFLAGS += -std=c++11

include( ../../buildconfig.pri )

INCLUDEPATH += $${TOPSRC}/libs/Common $${TOPSRC}/libs/CommandLine
LIBS += -lCommon -lCommandLine
contains( TARGETARCH, debug ) {
QT += widgets sql network
}

# Input
#RESOURCES = Resources.qrc

SOURCES += TestBinary.cpp

