######################################################################
# src/tests/TestAppWebServer/TestAppWebServer.pro
# hand hacked by SvOlli
# released as public domain
######################################################################

TARGET = TestAppCodeEditor
TEMPLATE = app
QT += sql network

include( ../../buildconfig.pri )
include( $${TOPSRC}/lua-5.1.pri )

INCLUDEPATH += $${TOPSRC}/libs/Common
LIBS += -lCommon
INCLUDEPATH += $${TOPSRC}/libs/Satellite
LIBS += -lSatellite
contains( TARGETARCH, debug ) {
QT += sql network
}

# Input
SOURCES += Application.cpp

HEADERS += ../../libs/Common/LuaEditorDialog.hpp
SOURCES += ../../libs/Common/LuaEditorDialog.cpp

HEADERS += ../../libs/Common/MyLua.hpp
SOURCES += ../../libs/Common/MyLua.cpp

HEADERS += MainWidget.hpp
SOURCES += MainWidget.cpp

