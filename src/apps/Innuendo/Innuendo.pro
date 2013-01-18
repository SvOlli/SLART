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
INCLUDEPATH += $${TOPSRC}/libs/Satellite
LIBS += -lSatellite
contains( TARGETARCH, debug ) {
QT += sql
}
QMAKE_CXXFLAGS_DEBUG += -DCOREDUMP_SIZE_MB=100

# Input
RC_FILE = $${TOPSRC}/libs/Common/Resources/WinIcon.rc

TRANSLATIONS += $${TOPSRC}/translations/$${TARGET}_de.ts

SOURCES += InnuendoApplication.cpp

HEADERS += DropDialog.hpp
SOURCES += DropDialog.cpp

HEADERS += ExecButton.hpp
SOURCES += ExecButton.cpp

HEADERS += InnuendoConfigDialog.hpp
SOURCES += InnuendoConfigDialog.cpp

HEADERS += InnuendoMainWidget.hpp
SOURCES += InnuendoMainWidget.cpp

HEADERS += LogListWidget.hpp
SOURCES += LogListWidget.cpp

HEADERS += SatelliteConfigWidget.hpp
SOURCES += SatelliteConfigWidget.cpp
