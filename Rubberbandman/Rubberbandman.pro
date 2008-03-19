######################################################################
# hand hacked by SvOlli
######################################################################

include( ../Global.pri )

TEMPLATE = app
TARGET = 

INCLUDEPATH += ../taglib
LIBS += -L../taglib -ltag

# Input
#RESOURCES = Resources.qrc

SOURCES += Application.cpp 

HEADERS += BrowseWidget.hpp 
SOURCES += BrowseWidget.cpp 

HEADERS += SLARTComWidget.hpp 
SOURCES += SLARTComWidget.cpp 

HEADERS += SLARTCom.hpp 
SOURCES += SLARTCom.cpp 

HEADERS += FileSysBrowser.hpp 
SOURCES += FileSysBrowser.cpp 

HEADERS += InfoEdit.hpp 
SOURCES += InfoEdit.cpp 

HEADERS += MainWindow.hpp 
SOURCES += MainWindow.cpp 

HEADERS += MainWidget.hpp 
SOURCES += MainWidget.cpp 

HEADERS += MySettings.hpp 
SOURCES += MySettings.cpp 

HEADERS += TagList.hpp 
SOURCES += TagList.cpp 

