######################################################################
# src/tests/TestAppDragAndDrop/TestAppDragAndDrop.pro
# hand hacked by SvOlli
# released as public domain
######################################################################

TARGET = TestAppDragAndDrop
TEMPLATE = app
QT += widgets sql network

include( ../../buildconfig.pri )

INCLUDEPATH += $${TOPSRC}/libs/Common
LIBS += -lCommon
INCLUDEPATH += $${TOPSRC}/libs/Satellite
LIBS += -lSatellite
contains( TARGETARCH, debug ) {
QT += widgets sql network
}

# Input
SOURCES += Application.cpp

HEADERS += MainWidget.hpp
SOURCES += MainWidget.cpp

