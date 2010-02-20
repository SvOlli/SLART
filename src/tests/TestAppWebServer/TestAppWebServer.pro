######################################################################
# src/tests/TestAppWebServer/TestAppWebServer.pro
# hand hacked by SvOlli
# released as public domain
######################################################################

TARGET = TestAppWebServer
TEMPLATE = app
QT += sql network

include( ../../config.pri )

INCLUDEPATH += ../../libs/Common
LIBS += -lCommon

# Input
RESOURCES = Resources/TestAppWebServer.qrc

SOURCES += Application.cpp 

HEADERS += MainWidget.hpp 
SOURCES += MainWidget.cpp 

