######################################################################
# src/tests/TestAppIconView/TestAppIconView.pro
# hand hacked by SvOlli
# released as public domain
######################################################################

TARGET = TestAppIconView
TEMPLATE = app
#QT += widgets sql network

include( ../../buildconfig.pri )

INCLUDEPATH += $${TOPSRC}/libs/Common
LIBS += -lCommon
INCLUDEPATH += $${TOPSRC}/libs/Satellite
LIBS += -lSatellite
contains( TARGETARCH, debug ) {
QT += sql network
}
QT += widgets

# Input
SOURCES += Application.cpp

HEADERS += MainWidget.hpp
SOURCES += MainWidget.cpp

HEADERS += MainWindow.hpp
SOURCES += MainWindow.cpp

