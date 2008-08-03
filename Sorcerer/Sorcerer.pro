######################################################################
# hand hacked by SvOlli
######################################################################

include ( ../Global.pri )

TEMPLATE = app
TARGET = Sorcerer

INCLUDEPATH += ../taglib
LIBS += -L../taglib -ltag
QT += sql

# Input
#RESOURCES = Resources.qrc

SOURCES += Application.cpp 

HEADERS += MySettings.hpp 
SOURCES += MySettings.cpp 

HEADERS += MainWindow.hpp 
SOURCES += MainWindow.cpp 

HEADERS += MainWidget.hpp 
SOURCES += MainWidget.cpp 

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

HEADERS += DirWalker.hpp
SOURCES += DirWalker.cpp

