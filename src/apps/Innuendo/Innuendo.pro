######################################################################
# src/apps/Innuendo/Innuendo.pro 
# written by Sven Oliver Moll
# distributed as public domain
######################################################################

TARGET = Innuendo
TEMPLATE = app
QT += network

include( ../../config.pri )

INCLUDEPATH += ../../libs/Common
LIBS += -lCommon

# Input
RESOURCES = ../../libs/Common/Resources/Common.qrc
RC_FILE = ../../libs/Common/Resources/WinIcon.rc

SOURCES += Application.cpp 

#HEADERS += Database.hpp

#HEADERS += MySettings.hpp 
#SOURCES += MySettings.cpp 

#HEADERS += MainWindow.hpp 
#SOURCES += MainWindow.cpp 

#HEADERS += WidgetShot.hpp 
#SOURCES += WidgetShot.cpp 

HEADERS += MainWidget.hpp 
SOURCES += MainWidget.cpp 

#HEADERS += AboutWidget.hpp
#SOURCES += AboutWidget.cpp

HEADERS += ExecButton.hpp 
SOURCES += ExecButton.cpp 

HEADERS += ConfigDialog.hpp 
SOURCES += ConfigDialog.cpp 

HEADERS += ConfigNotifyWidget.hpp 
SOURCES += ConfigNotifyWidget.cpp 

HEADERS += ConfigNotifyApplicationWidget.hpp 
SOURCES += ConfigNotifyApplicationWidget.cpp 

#HEADERS += ProxyWidget.hpp 
#SOURCES += ProxyWidget.cpp 

#HEADERS += GlobalConfigWidget.hpp 
#SOURCES += GlobalConfigWidget.cpp 

#HEADERS += SLARTCom.hpp
#SOURCES += SLARTCom.cpp

HEADERS += DropDialog.hpp 
SOURCES += DropDialog.cpp 

