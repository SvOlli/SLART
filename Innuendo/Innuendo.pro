######################################################################
# hand hacked by SvOlli
######################################################################

include ( ../Global.pri )

TEMPLATE = app
TARGET = Innuendo

# Input
RESOURCES = Resources/Common.qrc
RC_FILE = ../Common/Resources/WinIcon.rc

SOURCES += Application.cpp 

HEADERS += Database.hpp

HEADERS += MySettings.hpp 
SOURCES += MySettings.cpp 

HEADERS += MainWindow.hpp 
SOURCES += MainWindow.cpp 

HEADERS += MainWidget.hpp 
SOURCES += MainWidget.cpp 

HEADERS += AboutWidget.hpp
SOURCES += AboutWidget.cpp

HEADERS += ExecButton.hpp 
SOURCES += ExecButton.cpp 

HEADERS += ConfigDialog.hpp 
SOURCES += ConfigDialog.cpp 

HEADERS += ConfigNotifyWidget.hpp 
SOURCES += ConfigNotifyWidget.cpp 

HEADERS += ConfigNotifyApplicationWidget.hpp 
SOURCES += ConfigNotifyApplicationWidget.cpp 

HEADERS += ProxyWidget.hpp 
SOURCES += ProxyWidget.cpp 

HEADERS += GlobalConfigWidget.hpp 
SOURCES += GlobalConfigWidget.cpp 

HEADERS += SLARTCom.hpp
SOURCES += SLARTCom.cpp

HEADERS += DropDialog.hpp 
SOURCES += DropDialog.cpp 

