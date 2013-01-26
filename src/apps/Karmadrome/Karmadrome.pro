######################################################################
# src/apps/Karmadrome/Karmadrome.pro
# written by Sven Oliver Moll
# distributed as public domain
######################################################################

TEMPLATE = app
TARGET = Karmadrome
QT += sql network

include( ../../buildconfig.pri )

INCLUDEPATH += $${TOPSRC}/libs/Common
LIBS += -lCommon
INCLUDEPATH += $${TOPSRC}/libs/Satellite
LIBS += -lSatellite
QMAKE_CXXFLAGS_DEBUG += -DCOREDUMP_SIZE_MB=100

# Input
RESOURCES = $${TOPSRC}/resources/$${TARGET}.qrc
RC_FILE = $${TOPSRC}/resources/windows/$${TARGET}.rc

TRANSLATIONS += $${TOPSRC}/resources/translations/$${TARGET}_de.ts

SOURCES += KarmadromeApplication.cpp

HEADERS += ButtonsWidget.hpp
SOURCES += ButtonsWidget.cpp

HEADERS += ExportFolder.hpp
SOURCES += ExportFolder.cpp

HEADERS += ExportFolderList.hpp
SOURCES += ExportFolderList.cpp

HEADERS += ImportFolder.hpp
SOURCES += ImportFolder.cpp

HEADERS += KarmadromeConfigDialog.hpp
SOURCES += KarmadromeConfigDialog.cpp

HEADERS += KarmadromeMainWidget.hpp
SOURCES += KarmadromeMainWidget.cpp
