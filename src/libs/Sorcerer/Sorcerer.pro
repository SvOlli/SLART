######################################################################
# src/libs/Sorcerer/Sorcerer.pro
# written by Sven Oliver Moll
# distributed as public domain
######################################################################

TEMPLATE = lib
TARGET = $$qtLibraryTarget(Sorcerer)
QT += sql network
CONFIG += plugin

include( ../../buildconfig.pri )

INCLUDEPATH += $${TOPSRC}/libs/Common $${TOPSRC}/libs/TagLib
LIBS += -lCommon -lTag

# Input
RESOURCES += $${TOPSRC}/libs/Common/Resources/Common.qrc

TRANSLATIONS += $${TOPSRC}/translations/Sorcerer_de.ts

HEADERS += SorcererInterface.hpp

HEADERS += SorcererPlugin.hpp
SOURCES += SorcererPlugin.cpp

HEADERS += SorcererWidget.hpp
SOURCES += SorcererWidget.cpp

HEADERS += $${TOPSRC}/apps/Innuendo/SatelliteConfigWidget.hpp
SOURCES += $${TOPSRC}/apps/Innuendo/SatelliteConfigWidget.cpp

HEADERS += $${TOPSRC}/apps/Rubberbandman/DatabaseWidget.hpp
SOURCES += $${TOPSRC}/apps/Rubberbandman/DatabaseWidget.cpp

HEADERS += $${TOPSRC}/apps/Rubberbandman/DatabaseWorker.hpp
SOURCES += $${TOPSRC}/apps/Rubberbandman/DatabaseWorker.cpp
