######################################################################
# src/apps/Sorcerer/Sorcerer.pro
# written by Sven Oliver Moll
# distributed as public domain
######################################################################

TARGET = Sorcerer
TEMPLATE = app
CONFIG += qt
QT += widgets sql network

include( ../../buildconfig.pri )

INCLUDEPATH += $${TOPSRC}/libs/Common $${TOPSRC}/libs/TagLib
LIBS += -lCommon -lTag
INCLUDEPATH += $${TOPSRC}/libs/Satellite
LIBS += -lSatellite
QMAKE_CXXFLAGS_DEBUG += -DCOREDUMP_SIZE_MB=100

# Input
RESOURCES = $${TOPSRC}/resources/$${TARGET}.qrc
RC_FILE = $${TOPSRC}/resources/windows/$${TARGET}.rc

TRANSLATIONS += $${TOPSRC}/resources/translations/Sorcerer_de.ts

SOURCES += SorcererApplication.cpp

HEADERS += SorcererMainWidget.hpp
SOURCES += SorcererMainWidget.cpp

HEADERS += $${TOPSRC}/apps/Innuendo/SatelliteConfigWidget.hpp
SOURCES += $${TOPSRC}/apps/Innuendo/SatelliteConfigWidget.cpp

HEADERS += $${TOPSRC}/apps/Rubberbandman/DatabaseWidget.hpp
SOURCES += $${TOPSRC}/apps/Rubberbandman/DatabaseWidget.cpp

HEADERS += $${TOPSRC}/apps/Rubberbandman/DatabaseWorker.hpp
SOURCES += $${TOPSRC}/apps/Rubberbandman/DatabaseWorker.cpp

HEADERS += $${TOPSRC}/apps/Rubberbandman/DirWalkerUpdate.hpp
SOURCES += $${TOPSRC}/apps/Rubberbandman/DirWalkerUpdate.cpp

