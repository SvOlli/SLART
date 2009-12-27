######################################################################
# src/libs/CommandLine/CommandLine.pro
# hand hacked by SvOlli
# distributed as public domain
######################################################################

TARGET = Common
TEMPLATE = lib
CONFIG += qt staticlib
QT += sql network

include( ../../config.pri )

HEADERS += AboutWidget.hpp
SOURCES += AboutWidget.cpp

HEADERS += Database.hpp
SOURCES += Database.cpp

HEADERS += DirWalker.hpp
SOURCES += DirWalker.cpp

HEADERS += GlobalConfigWidget.hpp
SOURCES += GlobalConfigWidget.cpp

HEADERS += MainWindow.hpp
SOURCES += MainWindow.cpp

HEADERS += MySettings.hpp
SOURCES += MySettings.cpp

HEADERS += ProxyWidget.hpp
SOURCES += ProxyWidget.cpp

HEADERS += PasswordChecker.hpp
SOURCES += PasswordChecker.cpp

HEADERS += ScrollLine.hpp
SOURCES += ScrollLine.cpp

HEADERS += SLARTCom.hpp
SOURCES += SLARTCom.cpp

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

HEADERS += WidgetShot.hpp
SOURCES += WidgetShot.cpp

