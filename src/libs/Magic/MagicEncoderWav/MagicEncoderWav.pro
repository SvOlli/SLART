######################################################################
# src/libs/Magic/MagicEncoderWav/MagicEncoderWav.pro
# written by Sven Oliver Moll
# distributed as public domain
######################################################################

TEMPLATE = lib
TARGET = $$qtLibraryTarget(MagicEncoderWav)
CONFIG += qt plugin
QT += widgets

TOPSRC = "../../.."
include( $${TOPSRC}/buildconfig.pri )

#does not work, bug in qmake?
#TRANSLATIONS += $${TOPSRC}/resources/translations/MagicEncoderWav_de.ts
TRANSLATIONS += ../../../resources/translations/MagicEncoderWav_de.ts

INCLUDEPATH += $${TOPSRC}/libs/Common $${TOPSRC}/libs/TagLib $${TOPSRC}/libs/Magic/MagicEncoder
LIBS += -lMagicEncoder -lCommon -lSatellite
contains( TARGETARCH, debug ) {
QT += widgets sql network
}

HEADERS += MagicEncoderWav.hpp
SOURCES += MagicEncoderWav.cpp

HEADERS += MagicEncoderWavConfig.hpp
SOURCES += MagicEncoderWavConfig.cpp
