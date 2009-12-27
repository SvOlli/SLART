######################################################################
# src/apps/Creep/Creep.pro
# written by Sven Oliver Moll
# distributed as public domain
######################################################################

TARGET = Creep
TEMPLATE = app
QT -= gui
QT += network
LIBS += -llirc_client

include ( ../../config.pri )

# Input
SOURCES += Application.cpp 

