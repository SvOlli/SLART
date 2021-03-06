######################################################################
# src/apps/Innuendo/Innuendo.pro
# written by Sven Oliver Moll
# distributed as public domain
######################################################################

TARGET = Innuendo
TEMPLATE = app
CONFIG += qt
QT += widgets network

include( ../../buildconfig.pri )

INCLUDEPATH += $${TOPSRC}/libs/Common
LIBS += -lCommon
INCLUDEPATH += $${TOPSRC}/libs/Satellite
LIBS += -lSatellite
contains( TARGETARCH, debug ) {
QT += widgets sql
}
QMAKE_CXXFLAGS_DEBUG += -DCOREDUMP_SIZE_MB=100

# Input
RESOURCES = $${TOPSRC}/resources/$${TARGET}.qrc
RC_FILE = $${TOPSRC}/resources/windows/$${TARGET}.rc

TRANSLATIONS += $${TOPSRC}/resources/translations/$${TARGET}_de.ts

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
