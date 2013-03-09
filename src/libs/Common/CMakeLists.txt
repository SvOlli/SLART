#
# src/libs/Common/CMakeLists.txt
#
# written by SvOlli
# distributed as public domain
#

MESSAGE( STATUS "==== src/libs/Common ====" )

PROJECT( Common )

INCLUDE( LocalTools )
FIND_PACKAGE( Lua51 REQUIRED )
PKG_CHECK_MODULES( GIO2 REQUIRED gio-2.0 )

SET( QT_USE_QTSQL 1 )
SET( QT_USE_QTNETWORK 1 )
INCLUDE( ${QT_USE_FILE} )
ADD_DEFINITIONS( ${QT_DEFINITIONS} )
ADD_DEFINITIONS( -fPIC )
INCLUDE_DIRECTORIES(
   .
   ${LUA_INCLUDE_DIR}
   ${GIO2_INCLUDE_DIRS}
   ${SATELLITE_INCLUDE_DIR}
   ${MAGICINTERFACE_INCLUDE_DIR}
)

ADD_SOURCE( AboutWidget                   cpp hpp moc )
ADD_SOURCE( Binary                            hpp     )
ADD_SOURCE( CodeEditor                    cpp hpp moc )
ADD_SOURCE( CodeEditorLineNumberArea      cpp hpp     )
ADD_SOURCE( CrashCleanup                  cpp hpp     )
ADD_SOURCE( Database                      cpp hpp     )
ADD_SOURCE( DatabaseInterface             cpp hpp     )
ADD_SOURCE( DatabaseThread                cpp hpp moc private )
ADD_SOURCE( DirWalker                     cpp hpp     )
ADD_SOURCE( DirWalkerCallbackProxy        cpp hpp     )
ADD_SOURCE( DnDFileInfo                   cpp hpp moc )
ADD_SOURCE( FileIconProvider              cpp hpp     )
ADD_SOURCE( GenericSatelliteHandler       cpp hpp moc )
ADD_SOURCE( GlobalConfigWidget            cpp hpp moc )
ADD_SOURCE( GSettingsWrapper              cpp hpp     )
ADD_SOURCE( ImageWidget                   cpp hpp moc )
ADD_SOURCE( LuaEditorDialog               cpp hpp moc )
ADD_SOURCE( LEDIcon                       cpp hpp     )
ADD_SOURCE( MagicEncoderLoader            cpp hpp     )
ADD_SOURCE( MagicEncoderProxy             cpp hpp moc )
ADD_SOURCE( MainWindow                    cpp hpp moc )
ADD_SOURCE( MainWindowCheckClose          cpp hpp moc )
ADD_SOURCE( MyLua                         cpp hpp moc )
ADD_SOURCE( MyMouseEvent                  cpp hpp     )
ADD_SOURCE( ProxyWidget                   cpp hpp moc )
ADD_SOURCE( PasswordChecker               cpp hpp moc )
ADD_SOURCE( PathEnterWidget               cpp hpp moc )
ADD_SOURCE( ScgiRequest                   cpp hpp     )
ADD_SOURCE( ScrollLine                    cpp hpp moc )
ADD_SOURCE( Sequence                      cpp hpp moc )
ADD_SOURCE( SingleInstance                cpp hpp moc )
ADD_SOURCE( Settings                      cpp hpp moc )
ADD_SOURCE( SettingsGenerated             cpp         )
ADD_SOURCE( ShellLikeTokenizer            cpp hpp     )
ADD_SOURCE( SorcererLoader                cpp hpp     )
ADD_SOURCE( Synchronize                   cpp hpp     )
ADD_SOURCE( SyntaxHighlighter             cpp hpp moc )
ADD_SOURCE( SyntaxHighlighterLua          cpp hpp moc )
ADD_SOURCE( SyntaxHighlightingRule        cpp hpp     )
ADD_SOURCE( SyntaxHighlightingRuleComment cpp hpp     )
ADD_SOURCE( TagMap                        cpp hpp     )
ADD_SOURCE( ThreadAutoStart               cpp hpp moc )
ADD_SOURCE( Trace                         cpp hpp     )
ADD_SOURCE( TrackInfo                     cpp hpp     )
ADD_SOURCE( TrackInfoWidget               cpp hpp moc )
ADD_SOURCE( Translate                     cpp hpp     )
ADD_SOURCE( WebServer                     cpp hpp moc )
ADD_SOURCE( WidgetShot                    cpp hpp     )
ADD_SOURCE( WindowIconChanger             cpp hpp moc )
ADD_SOURCE( Xxtea                         cpp hpp     )

SET( Common_RESOURCES
)

FILE( GLOB Common_TS_FILES
   ${TRANSLATIONS_SOURCE_DIR}/Common_*.ts
   ${TRANSLATIONS_SOURCE_DIR}/Common.ts
)

QT4_WRAP_CPP( Common_MOC_GENERATED
   ${Common_MOC_HEADERS}
)

QT4_ADD_TRANSLATION( Common_QM_FILES
   ${Common_TS_FILES}
)

QT4_ADD_RESOURCES( Common_RESOURCES_RCC
   ${Common_RESOURCES}
)

ADD_LIBRARY( Common STATIC
   ${Common_MOC_GENERATED}
   ${Common_HEADERS}
   ${Common_PRIVATE_HEADERS}
   ${Common_SOURCES}
   ${Common_RESOURCES_RCC}
   ${Common_QM_FILES}
)

ADD_DEPENDENCIES( Common Satellite )

PROPAGATE_LIBRARY( Common )
