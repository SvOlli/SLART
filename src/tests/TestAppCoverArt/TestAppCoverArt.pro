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

HEADERS += Downloader.hpp
SOURCES += Downloader.cpp

HEADERS += DownloadJobCoverAmazonDE.hpp
SOURCES += DownloadJobCoverAmazonDE.cpp

HEADERS += IODeviceStreamer.hpp
SOURCES += IODeviceStreamer.cpp

