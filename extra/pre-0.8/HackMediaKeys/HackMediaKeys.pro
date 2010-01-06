######################################################################
# hand hacked by SvOlli
######################################################################

include ( ../Global.pri )

DEFINES += EVIL_POWER_BUTTON_HACK=1
TEMPLATE = app
TARGET = HackMediaKeys
DESTDIR = ../test
include ( ../globalshortcut/globalshortcut.pri )

# Input
#RESOURCES = Resources.qrc

SOURCES += Application.cpp 

HEADERS += EventHandler.hpp 
SOURCES += EventHandler.cpp 

HEADERS += MySettings.hpp 
SOURCES += MySettings.cpp 

