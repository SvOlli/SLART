######################################################################
# src/libs/Common/Common.pro
# hand hacked by SvOlli
# distributed as public domain
######################################################################

TARGET = Common
TEMPLATE = lib
CONFIG += qt staticlib
QT += sql network

include( ../../buildconfig.pri )

HEADERS += AboutWidget.hpp
SOURCES += AboutWidget.cpp

HEADERS += Database.hpp
SOURCES += Database.cpp

HEADERS += DirWalker.hpp
SOURCES += DirWalker.cpp

HEADERS += GenericSatMsgHandler.hpp
SOURCES += GenericSatMsgHandler.cpp

HEADERS += GlobalConfigWidget.hpp
SOURCES += GlobalConfigWidget.cpp

HEADERS += ImageWidget.hpp
SOURCES += ImageWidget.cpp

HEADERS += MagicEncoderLoader.hpp
SOURCES += MagicEncoderLoader.cpp

HEADERS += MainWindow.hpp
SOURCES += MainWindow.cpp

HEADERS += MainWindowCheckClose.hpp
SOURCES += MainWindowCheckClose.cpp

HEADERS += MyMouseEvent.hpp
SOURCES += MyMouseEvent.cpp

HEADERS += MySettings.hpp
SOURCES += MySettings.cpp

HEADERS += ProxyWidget.hpp
SOURCES += ProxyWidget.cpp

HEADERS += PasswordChecker.hpp
SOURCES += PasswordChecker.cpp

HEADERS += Satellite.hpp
SOURCES += Satellite.cpp

HEADERS += SatelliteServer.hpp
SOURCES += SatelliteServer.cpp

HEADERS += SatelliteServerRunner.hpp
SOURCES += SatelliteServerRunner.cpp

HEADERS += ScrollLine.hpp
SOURCES += ScrollLine.cpp

HEADERS += SorcererLoader.hpp
SOURCES += SorcererLoader.cpp

HEADERS += TagList.hpp
SOURCES += TagList.cpp

HEADERS += Trace.hpp
SOURCES += Trace.cpp

HEADERS += TrackInfo.hpp
SOURCES += TrackInfo.cpp

HEADERS += TrackInfoWidget.hpp
SOURCES += TrackInfoWidget.cpp

HEADERS += WebServer.hpp
SOURCES += WebServer.cpp

HEADERS += WidgetShot.hpp
SOURCES += WidgetShot.cpp

HEADERS += Xxtea.hpp
SOURCES += Xxtea.cpp

