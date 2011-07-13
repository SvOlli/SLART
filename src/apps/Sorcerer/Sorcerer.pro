######################################################################
# src/apps/Sorcerer/Sorcerer.pro
# written by Sven Oliver Moll
# distributed as public domain
######################################################################

TARGET = Sorcerer
TEMPLATE = app

include( ../../buildconfig.pri )

INCLUDEPATH += $${TOPSRC}/libs/Common
LIBS += -lCommon
contains( TARGETARCH, debug ) {
QT += sql network
}
QMAKE_CXXFLAGS_DEBUG += -DCOREDUMP_SIZE_MB=100

# Input
RESOURCES = ../../libs/Common/Resources/Common.qrc
RC_FILE = ../Common/Resources/WinIcon.rc

SOURCES += Application.cpp
