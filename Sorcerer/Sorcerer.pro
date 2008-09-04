######################################################################
# hand hacked by SvOlli
######################################################################

include ( ../Global.pri )

TEMPLATE = lib
CONFIG += plugin
QT += sql
TARGET = $$qtLibraryTarget(Sorcerer)
!windows {
  DESTDIR = ../lib
}

INCLUDEPATH += ../taglib
LIBS += -L../taglib -ltag

# Input
#RESOURCES = Resources.qrc

HEADERS += Interface.hpp

HEADERS += Plugin.hpp 
SOURCES += Plugin.cpp 

HEADERS += MainWindow.hpp 
SOURCES += MainWindow.cpp 

HEADERS += MainWidget.hpp 
SOURCES += MainWidget.cpp 

HEADERS += MySettings.hpp 
SOURCES += MySettings.cpp 

HEADERS += AboutWidget.hpp
SOURCES += AboutWidget.cpp

HEADERS += ../Innuendo/ConfigNotifyApplicationWidget.hpp
SOURCES += ../Innuendo/ConfigNotifyApplicationWidget.cpp

HEADERS += ../Innuendo/ConfigNotifyWidget.hpp
SOURCES += ../Innuendo/ConfigNotifyWidget.cpp

HEADERS += ProxyWidget.hpp
SOURCES += ProxyWidget.cpp

HEADERS += Database.hpp
SOURCES += Database.cpp

HEADERS += TrackInfo.hpp
SOURCES += TrackInfo.cpp

HEADERS += ../Rubberbandman/DatabaseWidget.hpp
SOURCES += ../Rubberbandman/DatabaseWidget.cpp

HEADERS += ../Rubberbandman/DatabaseWorker.hpp
SOURCES += ../Rubberbandman/DatabaseWorker.cpp

HEADERS += DirWalker.hpp
SOURCES += DirWalker.cpp

