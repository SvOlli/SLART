######################################################################
# hand hacked by SvOlli
######################################################################

include ( ../Global.pri )

TEMPLATE = app
TARGET = TemplateApp

# Input
#RESOURCES = Resources.qrc

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

