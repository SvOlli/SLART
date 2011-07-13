######################################################################
# src/libs/Magic/MagicEncoder/MagicEncoder.pro
# hand hacked by SvOlli
# distributed as public domain
######################################################################

TEMPLATE = lib
TARGET = MagicEncoder
CONFIG += qt staticlib
QT += network

TOPSRC = "../../.."
include( $${TOPSRC}/buildconfig.pri )

TRANSLATIONS += ../../../translations/$${TARGET}_de.ts

INCLUDEPATH += $${TOPSRC}/libs/Common
LIBS += -lCommon
contains( TARGETARCH, debug ) {
QT += sql
}

HEADERS += MagicEncoder.hpp
SOURCES += MagicEncoder.cpp

HEADERS += MagicEncoderConfig.hpp
SOURCES += MagicEncoderConfig.cpp

