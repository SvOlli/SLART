# #####################################################################
# src/tests/TestAppCoverArt/TestAppCoverArt.pro
# hand hacked by SvOlli
# released as public domain
# #####################################################################

TARGET = TestAppCoverArt
TEMPLATE = app
QT += sql network

include( ../../buildconfig.pri )

INCLUDEPATH += $${TOPSRC}/libs/Satellite
LIBS += -lSatellite
INCLUDEPATH += $${TOPSRC}/libs/Common
LIBS += -lCommon -lSatellite

# Input
# RESOURCES = Resources/TestAppCoverArt.qrc
SOURCES += Application.cpp

HEADERS += MainWidget.hpp
SOURCES += MainWidget.cpp

HEADERS += ../../libs/Common/Downloader.hpp
SOURCES += ../../libs/Common/Downloader.cpp

HEADERS += ../../libs/Common/DownloadReply.hpp
SOURCES += ../../libs/Common/DownloadReply.cpp

HEADERS += DownloadJobCoverAmazonDE.hpp
SOURCES += DownloadJobCoverAmazonDE.cpp

HEADERS += ../../libs/Common/IODeviceStreamer.hpp
SOURCES += ../../libs/Common/IODeviceStreamer.cpp

