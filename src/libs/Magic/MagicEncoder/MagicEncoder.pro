######################################################################
# src/libs/Magic/MagicEncoder/MagicEncoder.pro
# hand hacked by SvOlli
# distributed as public domain
######################################################################

TEMPLATE = lib
TARGET = MagicEncoder
CONFIG += qt staticlib
#TARGET = $$qtLibraryTarget(MagicEncoder)
#CONFIG += plugin
QT += network

TOPSRC = "../../.."
include( $${TOPSRC}/buildconfig.pri )

INCLUDEPATH += $${TOPSRC}/libs/Common
LIBS += -lCommon

HEADERS += MagicEncoder.hpp
SOURCES += MagicEncoder.cpp
