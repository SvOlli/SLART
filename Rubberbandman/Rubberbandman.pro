######################################################################
# hand hacked by SvOlli
######################################################################

include( ../Global.pri )

TEMPLATE = app
TARGET = Rubberbandman

INCLUDEPATH += ../taglib
LIBS += -L../taglib -ltag
QT += sql

# Input
#RESOURCES = Resources.qrc

SOURCES += Application.cpp 

HEADERS += BrowseWidget.hpp 
SOURCES += BrowseWidget.cpp 

HEADERS += SLARTComWidget.hpp 
SOURCES += SLARTComWidget.cpp 

HEADERS += DatabaseWidget.hpp 
SOURCES += DatabaseWidget.cpp 

HEADERS += DatabaseWorker.hpp 
SOURCES += DatabaseWorker.cpp 

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

HEADERS += ScrollLine.hpp 
SOURCES += ScrollLine.cpp 

HEADERS += AboutWidget.hpp
SOURCES += AboutWidget.cpp

HEADERS += TagList.hpp 
SOURCES += TagList.cpp 

HEADERS += ConfigDialog.hpp 
SOURCES += ConfigDialog.cpp 

HEADERS += GlobalConfigWidget.hpp 
SOURCES += GlobalConfigWidget.cpp 

HEADERS += TrackInfo.hpp 
SOURCES += TrackInfo.cpp 

HEADERS += Database.hpp 
SOURCES += Database.cpp 

HEADERS += DirWalker.hpp 
SOURCES += DirWalker.cpp 

