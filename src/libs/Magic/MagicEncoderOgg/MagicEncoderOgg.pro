######################################################################
# src/libs/Magic/MagicEncoderOgg/MagicEncoderOgg.pro
# written by Sven Oliver Moll
# distributed as public domain
######################################################################

TEMPLATE = lib
TARGET = $$qtLibraryTarget(MagicEncoderOgg)
#QT += sql network
CONFIG += plugin

TOPSRC = "../../.."
include( $${TOPSRC}/buildconfig.pri )

INCLUDEPATH += $${TOPSRC}/libs/Common $${TOPSRC}/libs/TagLib $${TOPSRC}/libs/Magic/MagicEncoder
LIBS += -lCommon -lTag -lMagicEncoder
LIBS += -lvorbisenc -lvorbis -lm -logg
PKGCONFIG += vorbisenc

HEADERS += MagicEncoderOgg.hpp
SOURCES += MagicEncoderOgg.cpp
