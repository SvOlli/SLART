######################################################################
# src/tests/TestAppWebServer/TestAppWebServer.pro
# hand hacked by SvOlli
# released as public domain
######################################################################

TARGET = TestAppWebServer
TEMPLATE = app
QT += network

include( ../../config.pri )

INCLUDEPATH += ../../libs/Common
LIBS += -lCommon
INCLUDEPATH += ../../libs/WebServer
LIBS += -lWebServer

# Input
RESOURCES = Resources/TestAppWebServer.qrc

SOURCES += Application.cpp 

//HEADERS += ../../libs/Common/MainWindow.hpp 
//SOURCES += ../../libs/Common/MainWindow.cpp 

HEADERS += MainWidget.hpp 
SOURCES += MainWidget.cpp 

