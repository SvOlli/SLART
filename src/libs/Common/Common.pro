######################################################################
# src/libs/Common/Common.pro
# hand hacked by SvOlli
# distributed as public domain
######################################################################

TARGET = Common
TEMPLATE = lib
CONFIG += qt staticlib
QT += sql network lua

include( ../../buildconfig.pri )

TRANSLATIONS += $${TOPSRC}/translations/$${TARGET}.ts
TRANSLATIONS += $${TOPSRC}/translations/$${TARGET}_de.ts

HEADERS += AboutWidget.hpp
SOURCES += AboutWidget.cpp

HEADERS += Binary.hpp

HEADERS += CodeEditor.hpp
SOURCES += CodeEditor.cpp

HEADERS += CodeEditorLineNumberArea.hpp
SOURCES += CodeEditorLineNumberArea.cpp

HEADERS += Database.hpp
SOURCES += Database.cpp

HEADERS += DatabaseInterface.hpp
SOURCES += DatabaseInterface.cpp

HEADERS += DatabaseThread.hpp
SOURCES += DatabaseThread.cpp

HEADERS += DirWalker.hpp
SOURCES += DirWalker.cpp

HEADERS += DirWalkerCallbackProxy.hpp
SOURCES += DirWalkerCallbackProxy.cpp

HEADERS += DnDFileInfo.hpp
SOURCES += DnDFileInfo.cpp

HEADERS += GenericSatMsgHandler.hpp
SOURCES += GenericSatMsgHandler.cpp

HEADERS += GlobalConfigWidget.hpp
SOURCES += GlobalConfigWidget.cpp

HEADERS += ImageWidget.hpp
SOURCES += ImageWidget.cpp

HEADERS += LuaEditorDialog.hpp
SOURCES += LuaEditorDialog.cpp

HEADERS += LEDIcon.hpp
SOURCES += LEDIcon.cpp

HEADERS += MagicEncoderLoader.hpp
SOURCES += MagicEncoderLoader.cpp

HEADERS += MagicEncoderProxy.hpp
SOURCES += MagicEncoderProxy.cpp

HEADERS += MainWindow.hpp
SOURCES += MainWindow.cpp

HEADERS += MainWindowCheckClose.hpp
SOURCES += MainWindowCheckClose.cpp

HEADERS += MyLua.hpp
SOURCES += MyLua.cpp

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
SOURCES += SatelliteConfig.cpp

HEADERS += SatelliteServer.hpp
SOURCES += SatelliteServer.cpp

HEADERS += SatelliteServerRunner.hpp
SOURCES += SatelliteServerRunner.cpp

HEADERS += ScrollLine.hpp
SOURCES += ScrollLine.cpp

HEADERS += SingleInstance.hpp
SOURCES += SingleInstance.cpp

HEADERS += Settings.hpp
SOURCES += Settings.cpp
SOURCES += SettingsGenerated.cpp

HEADERS += SorcererLoader.hpp
SOURCES += SorcererLoader.cpp

HEADERS += Synchronize.hpp
SOURCES += Synchronize.cpp

HEADERS += SyntaxHighlighter.hpp
SOURCES += SyntaxHighlighter.cpp

HEADERS += SyntaxHighlighterLua.hpp
SOURCES += SyntaxHighlighterLua.cpp

HEADERS += SyntaxHighlightingRule.hpp
SOURCES += SyntaxHighlightingRule.cpp

HEADERS += SyntaxHighlightingRuleComment.hpp
SOURCES += SyntaxHighlightingRuleComment.cpp

HEADERS += TagMap.hpp
SOURCES += TagMap.cpp

HEADERS += Trace.hpp
SOURCES += Trace.cpp

HEADERS += TrackInfo.hpp
SOURCES += TrackInfo.cpp

HEADERS += TrackInfoWidget.hpp
SOURCES += TrackInfoWidget.cpp

HEADERS += Translate.hpp
SOURCES += Translate.cpp

HEADERS += Version.hpp

HEADERS += WebServer.hpp
SOURCES += WebServer.cpp

HEADERS += WidgetShot.hpp
SOURCES += WidgetShot.cpp

HEADERS += Xxtea.hpp
SOURCES += Xxtea.cpp

