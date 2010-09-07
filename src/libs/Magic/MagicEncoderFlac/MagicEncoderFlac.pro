######################################################################
# src/libs/Magic/MagicEncoderFlac/MagicEncoderFlac.pro
# written by Sven Oliver Moll
# distributed as public domain
######################################################################

TEMPLATE = lib
TARGET = $$qtLibraryTarget(MagicEncoderFlac)
#QT += sql network
CONFIG += plugin

TOPSRC = "../../.."
include( $${TOPSRC}/buildconfig.pri )

INCLUDEPATH += $${TOPSRC}/libs/Common $${TOPSRC}/libs/TagLib $${TOPSRC}/libs/Magic/MagicEncoder
LIBS += -lMagicEncoder -lCommon -lTag
LIBS += -lFLAC++

HEADERS += MagicEncoderFlac.hpp
SOURCES += MagicEncoderFlac.cpp
