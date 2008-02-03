######################################################################
# hand hacked by SvOlli
######################################################################

include ( ../Global.pri )

TEMPLATE = app
TARGET = Innuendo

# Input
#RESOURCES = Resources.qrc

SOURCES += Application.cpp 

HEADERS += MySettings.hpp 
SOURCES += MySettings.cpp 

HEADERS += MainWindow.hpp 
SOURCES += MainWindow.cpp 

HEADERS += MainWidget.hpp 
SOURCES += MainWidget.cpp 

HEADERS += ConfigDialog.hpp 
SOURCES += ConfigDialog.cpp 

HEADERS += ConfigNotifyWidget.hpp 
SOURCES += ConfigNotifyWidget.cpp 

HEADERS += ConfigNotifyApplicationWidget.hpp 
SOURCES += ConfigNotifyApplicationWidget.cpp 

HEADERS += ProxyWidget.hpp 
SOURCES += ProxyWidget.cpp 

HEADERS += SLATCom.hpp
SOURCES += SLATCom.cpp
