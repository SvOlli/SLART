#
# src/libs/Common/CMakeLists.txt
#
# written by SvOlli
# distributed as public domain
#

MESSAGE( STATUS "==== src/libs/Common ====" )

PROJECT( Common )

FIND_PACKAGE( Lua51 REQUIRED )

INCLUDE( LocalLibrary )

SET( QT_USE_QTSQL 1 )
SET( QT_USE_QTNETWORK 1 )
INCLUDE( ${QT_USE_FILE} )
ADD_DEFINITIONS( ${QT_DEFINITIONS} )
INCLUDE_DIRECTORIES(
   .
   ${LUA_INCLUDE_DIR}
   ${SATELLITE_INCLUDE_DIR}
   ${MAGICINTERFACE_INCLUDE_DIR}
)

ADD_DEFINITIONS( -fPIC )

SET( Common_PUBLIC_HEADERS
   AboutWidget.hpp
   Binary.hpp
   CodeEditor.hpp
   CodeEditorLineNumberArea.hpp
   Database.hpp
   DatabaseInterface.hpp
   DirWalker.hpp
   DirWalkerCallbackProxy.hpp
   DnDFileInfo.hpp
   GenericSatelliteHandler.hpp
   GlobalConfigWidget.hpp
   ImageWidget.hpp
   LuaEditorDialog.hpp
   LEDIcon.hpp
   MagicEncoderLoader.hpp
   MagicEncoderProxy.hpp
   MainWindow.hpp
   MainWindowCheckClose.hpp
   MyLua.hpp
   MyMouseEvent.hpp
   MySettings.hpp
   ProxyWidget.hpp
   PasswordChecker.hpp
   ScgiRequest.hpp
   ScrollLine.hpp
   SingleInstance.hpp
   Settings.hpp
   SorcererLoader.hpp
   Synchronize.hpp
   SyntaxHighlighter.hpp
   SyntaxHighlighterLua.hpp
   SyntaxHighlightingRule.hpp
   SyntaxHighlightingRuleComment.hpp
   TagMap.hpp
   ThreadAutoStart.hpp
   Trace.hpp
   TrackInfo.hpp
   TrackInfoWidget.hpp
   Translate.hpp
   Version.hpp
   WebServer.hpp
   WidgetShot.hpp
   Xxtea.hpp
)

SET( Common_PRIVATE_HEADERS
   DatabaseThread.hpp
)

SET( Common_SOURCES
   AboutWidget.cpp
   CodeEditor.cpp
   CodeEditorLineNumberArea.cpp
   Database.cpp
   DatabaseInterface.cpp
   DatabaseThread.cpp
   DirWalker.cpp
   DirWalkerCallbackProxy.cpp
   DnDFileInfo.cpp
   GenericSatelliteHandler.cpp
   GlobalConfigWidget.cpp
   ImageWidget.cpp
   LuaEditorDialog.cpp
   LEDIcon.cpp
   MagicEncoderLoader.cpp
   MagicEncoderProxy.cpp
   MainWindow.cpp
   MainWindowCheckClose.cpp
   MyLua.cpp
   MyMouseEvent.cpp
   MySettings.cpp
   ProxyWidget.cpp
   PasswordChecker.cpp
   ScgiRequest.cpp
   ScrollLine.cpp
   SingleInstance.cpp
   Settings.cpp
   SettingsGenerated.cpp
   SorcererLoader.cpp
   Synchronize.cpp
   SyntaxHighlighter.cpp
   SyntaxHighlighterLua.cpp
   SyntaxHighlightingRule.cpp
   SyntaxHighlightingRuleComment.cpp
   TagMap.cpp
   ThreadAutoStart.cpp
   Trace.cpp
   TrackInfo.cpp
   TrackInfoWidget.cpp
   Translate.cpp
   WebServer.cpp
   WidgetShot.cpp
   Xxtea.cpp
)

SET( Common_RESOURCES
   Resources/Common.qrc
)

FILE( GLOB Common_TS_FILES
   ${TRANSLATIONS_SOURCE_DIR}/Common_*.ts
   ${TRANSLATIONS_SOURCE_DIR}/Common.ts
)

QT4_WRAP_CPP( Common_MOC_HEADERS
   ${Common_PUBLIC_HEADERS}
   ${Common_PRIVATE_HEADERS}
)

QT4_ADD_TRANSLATION( Common_QM_FILES
   ${Common_TS_FILES}
)

QT4_ADD_RESOURCES( Common_RESOURCES_RCC
   ${Common_RESOURCES}
)

ADD_LIBRARY( Common STATIC
   ${Common_MOC_HEADERS}
   ${Common_PUBLIC_HEADERS}
   ${Common_PRIVATE_HEADERS}
   ${Common_SOURCES}
   ${Common_RESOURCES_RCC}
   ${Common_QM_FILES}
)

ADD_DEPENDENCIES( Common Satellite )

PROPAGATE_LIBRARY( Common )
