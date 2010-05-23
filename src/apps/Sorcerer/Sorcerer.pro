######################################################################
# src/apps/Sorcerer/Sorcerer.pro
# written by Sven Oliver Moll
# distributed as public domain
######################################################################

TARGET = Sorcerer
TEMPLATE = app

include( ../../buildconfig.pri )

INCLUDEPATH += ../../libs/Common
LIBS += -lCommon

# Input
RESOURCES = ../../libs/Common/Resources/Common.qrc
RC_FILE = ../Common/Resources/WinIcon.rc

SOURCES += Application.cpp

