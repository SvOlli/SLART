#
# src/cmake/LocalTools.cmake
#
# written by SvOlli
# distributed as public domain
#

FUNCTION( ADD_SOURCE _NAME )

   SET( options h c hpp cpp private moc )
   CMAKE_PARSE_ARGUMENTS( ADD_SOURCE "${options}" "" "" ${ARGN} )

   IF( ADD_SOURCE_private )
      MESSAGE( STATUS "${_NAME}: private not implemented yet, adding as public" )
   ENDIF( ADD_SOURCE_private )
   IF( ADD_SOURCE_h AND ADD_SOURCE_hpp )
      MESSAGE( FATAL_ERROR "${_NAME} should only provide h or hpp" )
   ENDIF( ADD_SOURCE_h AND ADD_SOURCE_hpp )
   IF( ADD_SOURCE_c AND ADD_SOURCE_cpp )
      MESSAGE( FATAL_ERROR "${_NAME} should only provide c or cpp" )
   ENDIF( ADD_SOURCE_c AND ADD_SOURCE_cpp )

   IF( ADD_SOURCE_h )
#      MESSAGE( STATUS "adding header ${_NAME}.h" )
      SET( ${PROJECT_NAME}_HEADERS ${${PROJECT_NAME}_HEADERS} ${_NAME}.h PARENT_SCOPE )
      IF( ADD_SOURCE_moc )
         SET( ${PROJECT_NAME}_MOC_HEADERS ${${PROJECT_NAME}_MOC_HEADERS} ${_NAME}.h PARENT_SCOPE )
      ENDIF( ADD_SOURCE_moc )
   ENDIF( ADD_SOURCE_h )
   IF( ADD_SOURCE_hpp )
#      MESSAGE( STATUS "adding header ${_NAME}.hpp" )
      SET( ${PROJECT_NAME}_HEADERS ${${PROJECT_NAME}_HEADERS} ${_NAME}.hpp PARENT_SCOPE )
      IF( ADD_SOURCE_moc )
         SET( ${PROJECT_NAME}_MOC_HEADERS ${${PROJECT_NAME}_MOC_HEADERS} ${_NAME}.hpp PARENT_SCOPE )
      ENDIF( ADD_SOURCE_moc )
   ENDIF( ADD_SOURCE_hpp )
   IF( ADD_SOURCE_c )
#      MESSAGE( STATUS "adding source ${_NAME}.c" )
      SET( ${PROJECT_NAME}_SOURCES ${${PROJECT_NAME}_SOURCES} ${_NAME}.c PARENT_SCOPE )
   ENDIF( ADD_SOURCE_c )
   IF( ADD_SOURCE_cpp )
#      MESSAGE( STATUS "adding source ${_NAME}.cpp" )
      SET( ${PROJECT_NAME}_SOURCES ${${PROJECT_NAME}_SOURCES} ${_NAME}.cpp PARENT_SCOPE )
   ENDIF( ADD_SOURCE_cpp )

#   MESSAGE( STATUS "${PROJECT_NAME}_HEADERS = ${${PROJECT_NAME}_HEADERS}" )
#   MESSAGE( STATUS "${PROJECT_NAME}_SOURCES = ${${PROJECT_NAME}_SOURCES}" )

ENDFUNCTION( ADD_SOURCE )


FUNCTION( QT )

   SET( options concurrent core dbus gui network opengl openglextentions printsupport sql widgets xml )
   CMAKE_PARSE_ARGUMENTS( ADD_SOURCE "${options}" "" "" ${ARGN} )
   
   SET( QT_DEPENDS "" )

   IF( QT_concurrent )
      FIND_PACKAGE( Qt5Concurrent )
      SET( QT_DEPENDS ${QT_DEPENDS} Qt5::Concurrent )
   ENDIF( QT_concurrent )

   IF( QT_core )
      FIND_PACKAGE( Qt5Core )
      SET( QT_DEPENDS ${QT_DEPENDS} Qt5::Core )
   ENDIF( QT_core )

   IF( QT_dbus )
      FIND_PACKAGE( Qt5DBus )
      SET( QT_DEPENDS ${QT_DEPENDS} Qt5::DBus )
   ENDIF( QT_dbus )

   IF( QT_gui )
      FIND_PACKAGE( Qt5Gui )
      SET( QT_DEPENDS ${QT_DEPENDS} Qt5::Gui )
   ENDIF( QT_gui )

   IF( QT_network )
      FIND_PACKAGE( Qt5Network )
      SET( QT_DEPENDS ${QT_DEPENDS} Qt5::Network )
   ENDIF( QT_network )

   IF( QT_opengl )
      FIND_PACKAGE( Qt5OpenGL )
      SET( QT_DEPENDS ${QT_DEPENDS} Qt5::OpenGL )
   ENDIF( QT_opengl )

   IF( QT_openglextensions )
      FIND_PACKAGE( Qt5OpenGLExtensions )
      SET( QT_DEPENDS ${QT_DEPENDS} Qt5::OpenGLExtensions )
   ENDIF( QT_openglextensions )

   IF( QT_printsupport )
      FIND_PACKAGE( Qt5PrintSupport )
      SET( QT_DEPENDS ${QT_DEPENDS} Qt5::PrintSupport )
   ENDIF( QT_printsupport )

   IF( QT_sql )
      FIND_PACKAGE( Qt5Sql )
      SET( QT_DEPENDS ${QT_DEPENDS} Qt5::Sql )
   ENDIF( QT_sql )

   IF( QT_test )
      FIND_PACKAGE( Qt5Test )
      SET( QT_DEPENDS ${QT_DEPENDS} Qt5::Test )
   ENDIF( QT_test )

   IF( QT_widgets )
      FIND_PACKAGE( Qt5Widgets )
      SET( QT_DEPENDS ${QT_DEPENDS} Qt5::Widgets )
   ENDIF( QT_widgets )

   IF( QT_xml )
      FIND_PACKAGE( Qt5Xml )
      SET( QT_DEPENDS ${QT_DEPENDS} Qt5::Xml )
   ENDIF( QT_xml )

ENDFUNCTION( QT )


FUNCTION( PROPAGATE_LIBRARY _NAME )

   CMAKE_PARSE_ARGUMENTS( PROPAGATE_LIBRARY "" "LIBNAME" "" ${ARGN} )

   IF( DEFINED PROPAGATE_LIBRARY_LIBNAME )
      SET( LIBNAME ${PROPAGATE_LIBRARY_LIBNAME} )
   ELSE( DEFINED PROPAGATE_LIBRARY_LIBNAME )
      SET( LIBNAME ${_NAME} )
   ENDIF( DEFINED PROPAGATE_LIBRARY_LIBNAME )

   STRING( TOUPPER ${_NAME} _NAME_UPPER )
   STRING( TOLOWER ${_NAME} _NAME_LOWER )

   SET( ${_NAME_UPPER}_INCLUDE_DIR ${${_NAME}_SOURCE_DIR} CACHE PATH "${_NAME} include directory" )
   SET( ${_NAME_UPPER}_LIBRARY_DIR ${${_NAME}_BINARY_DIR} CACHE PATH "${_NAME} library directory" )
   SET( ${_NAME_UPPER}_LIBRARY_FLAGS "-L${${_NAME}_BINARY_DIR} -l${LIBNAME}" CACHE STRING "{_NAME} linker flags")

   MESSAGE( STATUS "propagating local library ${_NAME}" )
   MESSAGE( STATUS "  include dir: ${_NAME_UPPER}_INCLUDE_DIR=${${_NAME_UPPER}_INCLUDE_DIR}" )
   MESSAGE( STATUS "  library dir: ${_NAME_UPPER}_LIBRARY_DIR=${${_NAME_UPPER}_LIBRARY_DIR}" )
   MESSAGE( STATUS "  library flag: ${_NAME_UPPER}_LIBRARY_FLAGS=${${_NAME_UPPER}_LIBRARY_FLAGS}" )

ENDFUNCTION( PROPAGATE_LIBRARY )

