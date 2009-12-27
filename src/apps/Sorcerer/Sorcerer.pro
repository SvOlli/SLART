######################################################################
# src/apps/Sorcerer/Sorcerer.pro
# written by Sven Oliver Moll
# distributed as public domain
######################################################################

TEMPLATE = lib
TARGET = $$qtLibraryTarget(Sorcerer)
QT += sql network
CONFIG += plugin

include( ../../config.pri )

INCLUDEPATH += ../../libs/Common ../../libs/TagLib
LIBS += -lCommon -lTag

# Input
RESOURCES = ../../libs/Common/Resources/Common.qrc

HEADERS += Interface.hpp

HEADERS += Plugin.hpp
SOURCES += Plugin.cpp

HEADERS += MainWidget.hpp 
SOURCES += MainWidget.cpp 

#HEADERS += MainWindow.hpp 
#SOURCES += MainWindow.cpp 

#HEADERS += MySettings.hpp 
#SOURCES += MySettings.cpp 

#HEADERS += AboutWidget.hpp
#SOURCES += AboutWidget.cpp

HEADERS += ../../apps/Innuendo/ConfigNotifyApplicationWidget.hpp
SOURCES += ../../apps/Innuendo/ConfigNotifyApplicationWidget.cpp

HEADERS += ../../apps/Innuendo/ConfigNotifyWidget.hpp
SOURCES += ../../apps/Innuendo/ConfigNotifyWidget.cpp

#HEADERS += ProxyWidget.hpp
#SOURCES += ProxyWidget.cpp

#HEADERS += Database.hpp
#SOURCES += Database.cpp

#HEADERS += TrackInfo.hpp
#SOURCES += TrackInfo.cpp

HEADERS += ../../apps/Rubberbandman/DatabaseWidget.hpp
SOURCES += ../../apps/Rubberbandman/DatabaseWidget.cpp

HEADERS += ../../apps/Rubberbandman/DatabaseWorker.hpp
SOURCES += ../../apps/Rubberbandman/DatabaseWorker.cpp

#HEADERS += DirWalker.hpp
#SOURCES += DirWalker.cpp

