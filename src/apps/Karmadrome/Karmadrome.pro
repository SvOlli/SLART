######################################################################
# src/apps/Karmadrome/Karmadrome.pro
# written by Sven Oliver Moll
# distributed as public domain
######################################################################

TEMPLATE = app
TARGET = Karmadrome
QT += sql network

include( ../../buildconfig.pri )

INCLUDEPATH += ../../libs/Common
LIBS += -lCommon

# Input
RESOURCES = ../../libs/Common/Resources/Common.qrc
RC_FILE = ../../libs/Common/Resources/WinIcon.rc

SOURCES += Application.cpp 

#HEADERS += MySettings.hpp 
#SOURCES += MySettings.cpp 

#HEADERS += MainWindow.hpp 
#SOURCES += MainWindow.cpp 

#HEADERS += WidgetShot.hpp 
#SOURCES += WidgetShot.cpp 

HEADERS += MainWidget.hpp 
SOURCES += MainWidget.cpp 

HEADERS += ImportExport.hpp 
SOURCES += ImportExport.cpp 

#HEADERS += AboutWidget.hpp
#SOURCES += AboutWidget.cpp

#HEADERS += Database.hpp
#SOURCES += Database.cpp

#HEADERS += TrackInfo.hpp
#SOURCES += TrackInfo.cpp

#HEADERS += TrackInfoWidget.hpp
#SOURCES += TrackInfoWidget.cpp

#HEADERS += ScrollLine.hpp
#SOURCES += ScrollLine.cpp

HEADERS += ButtonsWidget.hpp 
SOURCES += ButtonsWidget.cpp 

#HEADERS += SLARTCom.hpp 
#SOURCES += SLARTCom.cpp 

HEADERS += ConfigDialog.hpp 
SOURCES += ConfigDialog.cpp 

#HEADERS += GlobalConfigWidget.hpp 
#SOURCES += GlobalConfigWidget.cpp 

