######################################################################
# src/libs/Magic/MagicEncoderMp3/MagicEncoderMp3.pro
# written by Sven Oliver Moll
# distributed as public domain
######################################################################

TEMPLATE = lib
TARGET = $$qtLibraryTarget(MagicEncoderMp3)
CONFIG += plugin

TOPSRC = "../../.."
include( $${TOPSRC}/buildconfig.pri )

#does not work, bug in qmake?
#TRANSLATIONS += $${TOPSRC}/resources/translations/MagicEncoderMp3_de.ts
TRANSLATIONS += ../../../resources/translations/MagicEncoderMp3_de.ts

INCLUDEPATH += $${TOPSRC}/libs/Common $${TOPSRC}/libs/TagLib $${TOPSRC}/libs/Magic/MagicEncoder
LIBS += -lMagicEncoder -lCommon -lSatellite
contains( TARGETARCH, debug ) {
QT += widgets sql network
}
LIBS += -lmp3lame

HEADERS += MagicEncoderMp3.hpp
SOURCES += MagicEncoderMp3.cpp

HEADERS += MagicEncoderMp3Config.hpp
SOURCES += MagicEncoderMp3Config.cpp
