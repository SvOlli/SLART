#
# src/tests/TestAppCodeEditor/CMakeLists.txt
#
# written by SvOlli
# distributed as public domain
#

MESSAGE( STATUS "==== src/tests/TextAppEditor ====" )

PROJECT( TestAppCodeEditor )

FIND_PACKAGE( Lua51 REQUIRED )

#SET( QT_USE_QTNETWORK 1 )
INCLUDE( ${QT_USE_FILE} )
ADD_DEFINITIONS( ${QT_DEFINITIONS} )
INCLUDE_DIRECTORIES(
   ${LUA_INCLUDE_DIR}
   ${COMMON_INCLUDE_DIR}
)

SET( TestAppCodeEditor_HEADERS
   ../../libs/Common/LuaEditorDialog.hpp
   ../../libs/Common/MyLua.hpp
   MainWidget.hpp
)

SET( TestAppCodeEditor_SOURCES
   Application.cpp
   ../../libs/Common/LuaEditorDialog.cpp
   ../../libs/Common/MyLua.cpp
   MainWidget.cpp
)

QT4_WRAP_CPP( TestAppCodeEditor_MOC_HEADERS
   ${TestAppCodeEditor_HEADERS}
)

ADD_EXECUTABLE( TestAppCodeEditor
   ${TestAppCodeEditor_MOC_HEADERS}
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
