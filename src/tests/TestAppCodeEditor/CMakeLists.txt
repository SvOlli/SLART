#
# src/tests/TestAppCodeEditor/CMakeLists.txt
#
# written by SvOlli
# distributed as public domain
#

MESSAGE( STATUS "==== src/tests/TextAppEditor ====" )

PROJECT( TestAppCodeEditor )

INCLUDE( LocalTools )
FIND_PACKAGE( Lua51 REQUIRED )

#SET( QT_USE_QTNETWORK 1 )
INCLUDE( ${QT_USE_FILE} )
ADD_DEFINITIONS( ${QT_DEFINITIONS} )
INCLUDE_DIRECTORIES(
   ${LUA_INCLUDE_DIR}
   ${COMMON_INCLUDE_DIR}
)

ADD_SOURCE( Application                       cpp         )
ADD_SOURCE( ../../libs/Common/LuaEditorDialog cpp hpp moc )
ADD_SOURCE( ../../libs/Common/MyLua           cpp hpp moc )
ADD_SOURCE( MainWidget                        cpp hpp moc )

QT4_WRAP_CPP( TestAppCodeEditor_MOC_GENERATED
   ${TestAppCodeEditor_MOC_HEADERS}
)

ADD_EXECUTABLE( TestAppCodeEditor
   ${TestAppCodeEditor_MOC_GENERATED}
   ${TestAppCodeEditor_HEADERS}
   ${TestAppCodeEditor_SOURCES}
)

ADD_DEPENDENCIES( TestAppCodeEditor
   Common
)

TARGET_LINK_LIBRARIES( TestAppCodeEditor
   ${COMMON_LIBRARY_FLAGS}
   ${QT_LIBRARIES}
   ${LUA_LIBRARIES}
)

IF( CMAKE_BUILD_TYPE MATCHES [Dd][Ee][Bb][Uu][Gg] )
   INSTALL( TARGETS TestAppCodeEditor
      DESTINATION test
   )
ENDIF( CMAKE_BUILD_TYPE MATCHES [Dd][Ee][Bb][Uu][Gg] )
