######################################################################
# hand hacked by SvOlli
######################################################################

include ( ../Global.pri )

TEMPLATE = app
TARGET = Karmadrome
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

HEADERS += Database.hpp
SOURCES += Database.cpp

HEADERS += TrackInfo.hpp
SOURCES += TrackInfo.cpp

HEADERS += TrackInfoWidget.hpp
SOURCES += TrackInfoWidget.cpp

HEADERS += ScrollLine.hpp
SOURCES += ScrollLine.cpp

HEADERS += ButtonsWidget.hpp 
SOURCES += ButtonsWidget.cpp 

HEADERS += SLARTCom.hpp 
SOURCES += SLARTCom.cpp 

HEADERS += ConfigDialog.hpp 
SOURCES += ConfigDialog.cpp 

HEADERS += GlobalConfigWidget.hpp 
SOURCES += GlobalConfigWidget.cpp 

