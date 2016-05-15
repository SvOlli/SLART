######################################################################
# src/tests/TestAppGSettings/TestAppGSettings.pro
# hand hacked by SvOlli
# released as public domain
######################################################################

TARGET = TestAppGSettings
TEMPLATE = app
QT -= gui

include( ../../buildconfig.pri )
include( $${TOPSRC}/gio-2.0.pri )
INCLUDEPATH += $${TOPSRC}/libs/Common
LIBS += -lCommon

contains( TARGETARCH, debug ) {
QT += widgets sql network
}

# Input
#RESOURCES = Resources.qrc

SOURCES += Application.cpp
