######################################################################
# src/apps/Sorcerer/Sorcerer.pro
# written by Sven Oliver Moll
# distributed as public domain
######################################################################

TARGET = Sorcerer
TEMPLATE = app
QT += sql network

include( ../../buildconfig.pri )

INCLUDEPATH += $${TOPSRC}/libs/Common $${TOPSRC}/libs/TagLib
LIBS += -lCommon -lTag
INCLUDEPATH += $${TOPSRC}/libs/Satellite
LIBS += -lSatellite
contains( TARGETARCH, debug ) {
QT += sql network
}
QMAKE_CXXFLAGS_DEBUG += -DCOREDUMP_SIZE_MB=100

# Input
RESOURCES += $${TOPSRC}/libs/Common/Resources/Common.qrc
RC_FILE = $${TOPSRC}/libs/Common/Resources/WinIcon.rc

TRANSLATIONS += $${TOPSRC}/translations/Sorcerer_de.ts

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

