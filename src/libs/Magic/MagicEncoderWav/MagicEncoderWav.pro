######################################################################
# src/libs/Magic/MagicEncoderWav/MagicEncoderWav.pro
# written by Sven Oliver Moll
# distributed as public domain
######################################################################

TEMPLATE = lib
TARGET = $$qtLibraryTarget(MagicEncoderWav)
CONFIG += plugin

TOPSRC = "../../.."
include( $${TOPSRC}/buildconfig.pri )

#does not work, bug in qmake?
#TRANSLATIONS += $${TOPSRC}/translations/MagicEncoderWav_de.ts
TRANSLATIONS += ../../../translations/MagicEncoderWav_de.ts

INCLUDEPATH += $${TOPSRC}/libs/Common $${TOPSRC}/libs/TagLib $${TOPSRC}/libs/Magic/MagicEncoder
LIBS += -lMagicEncoder -lCommon -lTag
contains( TARGETARCH, debug ) {
QT += sql network
}

HEADERS += MagicEncoderWav.hpp
SOURCES += MagicEncoderWav.cpp

HEADERS += MagicEncoderWavConfig.hpp
SOURCES += MagicEncoderWavConfig.cpp
