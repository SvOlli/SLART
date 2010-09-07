######################################################################
# src/libs/Magic/MagicEncoderWav/MagicEncoderWav.pro
# written by Sven Oliver Moll
# distributed as public domain
######################################################################

TEMPLATE = lib
TARGET = $$qtLibraryTarget(MagicEncoderWav)
#QT += sql network
CONFIG += plugin

TOPSRC = "../../.."
include( $${TOPSRC}/buildconfig.pri )

INCLUDEPATH += $${TOPSRC}/libs/Common $${TOPSRC}/libs/TagLib $${TOPSRC}/libs/Magic/MagicEncoder
LIBS += -lMagicEncoder -lCommon -lTag

HEADERS += MagicEncoderWav.hpp
SOURCES += MagicEncoderWav.cpp
