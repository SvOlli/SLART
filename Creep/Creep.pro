######################################################################
# hand hacked by SvOlli
######################################################################

include ( ../Global.pri )

TEMPLATE = app
TARGET = Creep
QT -= gui
LIBS += -llirc_client

# Input
SOURCES += Application.cpp 

