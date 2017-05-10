######################################################################
# src/apps/Underpass/Underpass.pro
# written by Sven Oliver Moll
# distributed as public domain
######################################################################

TARGET = Underpass
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

SOURCES += UnderpassApplication.cpp

HEADERS += StationStorage.hpp
SOURCES += StationStorage.cpp

HEADERS += UnderpassConfigDialog.hpp
SOURCES += UnderpassConfigDialog.cpp

HEADERS += UnderpassMainWidget.hpp
SOURCES += UnderpassMainWidget.cpp

