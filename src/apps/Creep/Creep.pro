######################################################################
# src/apps/Creep/Creep.pro
# written by Sven Oliver Moll
# distributed as public domain
######################################################################

TARGET = Creep
TEMPLATE = app
QT -= gui
QT += network

INCLUDEPATH += ../../libs/Common
LIBS += -llirc_client

include ( ../../buildconfig.pri )

# Input
SOURCES += Application.cpp 

