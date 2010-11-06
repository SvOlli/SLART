######################################################################
# src/apps/Creep/Creep.pro
# written by Sven Oliver Moll
# distributed as public domain
######################################################################

TARGET = Creep
TEMPLATE = app
QT -= gui
QT += network

include ( ../../buildconfig.pri )

INCLUDEPATH += $${TOPSRC}/libs/Common
LIBS += -llirc_client

# Input
SOURCES += Application.cpp

