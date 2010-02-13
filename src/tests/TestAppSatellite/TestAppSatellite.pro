######################################################################
# src/tests/TestAppSLARTCom/TestAppSLARTCom.pro
# hand hacked by SvOlli
# released as public domain
######################################################################

TARGET = TestAppSLARTCom
TEMPLATE = app
QT += network

include( ../../config.pri )

INCLUDEPATH += ../../libs/SLARTCom
LIBS += -lSLARTCom

# Input
#RESOURCES = Resources.qrc

SOURCES += Application.cpp 

HEADERS += MainWindow.hpp 
SOURCES += MainWindow.cpp 

HEADERS += MainWidget.hpp 
SOURCES += MainWidget.cpp 

