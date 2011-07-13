######################################################################
# src/apps/Innuendo/Innuendo.pro
# written by Sven Oliver Moll
# distributed as public domain
######################################################################

TARGET = Innuendo
TEMPLATE = app
QT += network

include( ../../buildconfig.pri )

INCLUDEPATH += $${TOPSRC}/libs/Common
LIBS += -lCommon
contains( TARGETARCH, debug ) {
QT += sql
}
QMAKE_CXXFLAGS_DEBUG += -DCOREDUMP_SIZE_MB=100

# Input
RESOURCES = $${TOPSRC}/libs/Common/Resources/Common.qrc
RC_FILE = $${TOPSRC}/libs/Common/Resources/WinIcon.rc

TRANSLATIONS += $${TOPSRC}/translations/$${TARGET}_de.ts

SOURCES += Application.cpp

HEADERS += LogListWidget.hpp
SOURCES += LogListWidget.cpp

HEADERS += MainWidget.hpp
SOURCES += MainWidget.cpp

HEADERS += ExecButton.hpp
SOURCES += ExecButton.cpp

HEADERS += ConfigDialog.hpp
SOURCES += ConfigDialog.cpp

HEADERS += SatelliteConfigWidget.hpp
SOURCES += SatelliteConfigWidget.cpp

HEADERS += DropDialog.hpp
SOURCES += DropDialog.cpp

