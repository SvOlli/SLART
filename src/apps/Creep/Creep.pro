######################################################################
# src/apps/Creep/Creep.pro
# written by Sven Oliver Moll
# distributed as public domain
######################################################################

TARGET = Creep
TEMPLATE = app
CONFIG += qt
QT -= gui
QT += widgets network

include ( ../../buildconfig.pri )

INCLUDEPATH += $${TOPSRC}/libs/Common
INCLUDEPATH += $${TOPSRC}/libs/Satellite
LIBS += -llirc_client

# Input
SOURCES += Application.cpp
