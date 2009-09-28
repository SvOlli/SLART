######################################################################
# hand hacked by SvOlli
######################################################################

include ( ../Global.pri )

TEMPLATE = app
TARGET = TestAppSocket
DESTDIR = ../test

# Input
#RESOURCES = Resources.qrc

SOURCES += Application.cpp 

HEADERS += Database.hpp

HEADERS += MySettings.hpp 
SOURCES += MySettings.cpp 

HEADERS += MainWindow.hpp 
SOURCES += MainWindow.cpp 

HEADERS += MainWidget.hpp 
SOURCES += MainWidget.cpp 

HEADERS += SLARTSock.hpp 
SOURCES += SLARTSock.cpp 

HEADERS += SLARTSockServer.hpp 
SOURCES += SLARTSockServer.cpp 

