# #####################################################################
# src/tests/TestAppCoverArt/TestAppCoverArt.pro
# hand hacked by SvOlli
# released as public domain
# #####################################################################

TARGET = TestAppCoverArt
TEMPLATE = app
QT += widgets sql network

include( ../../buildconfig.pri )

INCLUDEPATH += $${TOPSRC}/libs/Satellite
INCLUDEPATH += $${TOPSRC}/libs/Common
INCLUDEPATH += $${TOPSRC}/libs/Kryptonite
LIBS += -lCommon -lSatellite #-lKryptonite

# Input
# RESOURCES = Resources/TestAppCoverArt.qrc
SOURCES += Application.cpp

HEADERS += MainWidget.hpp
SOURCES += MainWidget.cpp

HEADERS += ../../libs/Kryptonite/Kryptonite.hpp
SOURCES += ../../libs/Kryptonite/Kryptonite.cpp

HEADERS += ../../libs/Kryptonite/KryptoniteJobCover.hpp
SOURCES += ../../libs/Kryptonite/KryptoniteJobCover.cpp

HEADERS += ../../libs/Kryptonite/KryptoniteJobCoverAmazonDE.hpp
SOURCES += ../../libs/Kryptonite/KryptoniteJobCoverAmazonDE.cpp

HEADERS += ../../libs/Kryptonite/KryptoniteJobCoverDiscogs.hpp
SOURCES += ../../libs/Kryptonite/KryptoniteJobCoverDiscogs.cpp

HEADERS += ../../libs/Kryptonite/KryptoniteReply.hpp
SOURCES += ../../libs/Kryptonite/KryptoniteReply.cpp

HEADERS += ../../libs/Kryptonite/IODeviceStreamer.hpp
SOURCES += ../../libs/Kryptonite/IODeviceStreamer.cpp

