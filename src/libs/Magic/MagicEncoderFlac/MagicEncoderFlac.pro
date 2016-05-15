######################################################################
# src/libs/Magic/MagicEncoderFlac/MagicEncoderFlac.pro
# written by Sven Oliver Moll
# distributed as public domain
######################################################################

TEMPLATE = lib
TARGET = $$qtLibraryTarget(MagicEncoderFlac)
CONFIG += plugin

TOPSRC = "../../.."
include( $${TOPSRC}/buildconfig.pri )

#does not work, bug in qmake?
#TRANSLATIONS += $${TOPSRC}/resources/translations/MagicEncoderFlac_de.ts
TRANSLATIONS += ../../../resources/translations/MagicEncoderFlac_de.ts

INCLUDEPATH += $${TOPSRC}/libs/Common $${TOPSRC}/libs/TagLib $${TOPSRC}/libs/Magic/MagicEncoder
LIBS += -lMagicEncoder -lCommon -lSatellite
contains( TARGETARCH, debug ) {
QT += widgets sql network
}
LIBS += -lFLAC++

HEADERS += MagicEncoderFlac.hpp
SOURCES += MagicEncoderFlac.cpp

HEADERS += MagicEncoderFlacConfig.hpp
SOURCES += MagicEncoderFlacConfig.cpp

