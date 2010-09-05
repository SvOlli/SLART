######################################################################
# src/libs/Magic/MagicEncoderMp3/MagicEncoderMp3.pro
# written by Sven Oliver Moll
# distributed as public domain
######################################################################

TEMPLATE = lib
TARGET = $$qtLibraryTarget(MagicEncoderMp3)
#QT += sql network
CONFIG += plugin

TOPSRC = "../../.."
include( $${TOPSRC}/buildconfig.pri )

INCLUDEPATH += $${TOPSRC}/libs/Common $${TOPSRC}/libs/TagLib $${TOPSRC}/libs/Magic/MagicEncoder
LIBS += -lCommon -lTag -lMagicEncoder
LIBS += -lmp3lame

HEADERS += MagicEncoderMp3.hpp
SOURCES += MagicEncoderMp3.cpp
