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
RESOURCES = ../../libs/Common/Resources/Common.qrc

HEADERS += SorcererInterface.hpp

HEADERS += SorcererPlugin.hpp
SOURCES += SorcererPlugin.cpp

HEADERS += SorcererWidget.hpp
SOURCES += SorcererWidget.cpp

HEADERS += ../../apps/Innuendo/SatelliteConfigWidget.hpp
SOURCES += ../../apps/Innuendo/SatelliteConfigWidget.cpp

HEADERS += ../../apps/Rubberbandman/DatabaseWidget.hpp
SOURCES += ../../apps/Rubberbandman/DatabaseWidget.cpp

HEADERS += ../../apps/Rubberbandman/DatabaseWorker.hpp
SOURCES += ../../apps/Rubberbandman/DatabaseWorker.cpp
