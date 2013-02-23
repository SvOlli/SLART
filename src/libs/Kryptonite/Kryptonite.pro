######################################################################
# src/libs/Kryptonite/Kryptonite.pro
# hand hacked by SvOlli
# distributed as public domain
######################################################################

TARGET = Kryptonite
TEMPLATE = lib
CONFIG += qt staticlib
QT += network

include( ../../buildconfig.pri )

HEADERS += IODeviceStreamer.hpp
SOURCES += IODeviceStreamer.cpp

HEADERS += Kryptonite.hpp
SOURCES += Kryptonite.cpp

HEADERS += KryptoniteJobCover.hpp
SOURCES += KryptoniteJobCover.cpp

HEADERS += KryptoniteJobCoverAmazonDE.hpp
SOURCES += KryptoniteJobCoverAmazonDE.cpp

HEADERS += KryptoniteReply.hpp
SOURCES += KryptoniteReply.cpp
