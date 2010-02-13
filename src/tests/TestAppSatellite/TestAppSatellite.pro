######################################################################
# src/tests/TestAppSatellite/TestAppSatellite.pro
# hand hacked by SvOlli
# released as public domain
######################################################################

TARGET = TestAppSatellite
TEMPLATE = app
QT += network

include( ../../config.pri )

INCLUDEPATH += ../../libs/Common
LIBS += -lCommon

# Input
#RESOURCES = Resources.qrc

SOURCES += Application.cpp 

HEADERS += MainWindow.hpp 
SOURCES += MainWindow.cpp 

HEADERS += MainWidget.hpp 
SOURCES += MainWidget.cpp 

