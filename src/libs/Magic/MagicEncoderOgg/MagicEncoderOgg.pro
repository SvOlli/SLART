######################################################################
# src/libs/Magic/MagicEncoderOgg/MagicEncoderOgg.pro
# written by Sven Oliver Moll
# distributed as public domain
######################################################################

TEMPLATE = lib
TARGET = $$qtLibraryTarget(MagicEncoderOgg)
CONFIG += plugin

TOPSRC = "../../.."
include( $${TOPSRC}/buildconfig.pri )

#does not work, bug in qmake?
#TRANSLATIONS += $${TOPSRC}/translations/MagicEncoderOgg_de.ts
TRANSLATIONS += ../../../translations/MagicEncoderOgg_de.ts

INCLUDEPATH += $${TOPSRC}/libs/Common $${TOPSRC}/libs/TagLib $${TOPSRC}/libs/Magic/MagicEncoder
LIBS += -lMagicEncoder -lCommon -lTag
LIBS += -lvorbisenc -lvorbis -lm -logg
PKGCONFIG += vorbisenc

HEADERS += MagicEncoderOgg.hpp
SOURCES += MagicEncoderOgg.cpp

HEADERS += MagicEncoderOggConfig.hpp
SOURCES += MagicEncoderOggConfig.cpp
