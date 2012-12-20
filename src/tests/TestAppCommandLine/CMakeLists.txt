#
# src/tests/TestAppCommandLine/CMakeLists.txt
#
# written by SvOlli
# distributed as public domain
#

MESSAGE( STATUS "==== src/tests/TextAppCommandLine ====" )

PROJECT( TestAppCommandLine )

SET( QT_DONT_USE_QTGUI 1 )
INCLUDE( ${QT_USE_FILE} )
INCLUDE_DIRECTORIES( ${COMMANDLINE_INCLUDE_DIR} )
ADD_DEFINITIONS( ${QT_DEFINITIONS} )

SET( TestAppCommandLine_HEADERS
)

SET( TestAppCommandLine_SOURCES
   Application.cpp
)

ADD_EXECUTABLE( TestAppCommandLine
   ${TestAppCommandLine_SOURCES}
)

ADD_DEPENDENCIES( TestAppCommandLine
   CommandLine
)

TARGET_LINK_LIBRARIES( TestAppCommandLine
   ${COMMANDLINE_LIBRARY_FLAGS}
   ${QT_LIBRARIES}
)

IF( CMAKE_BUILD_TYPE MATCHES [Dd][Ee][Bb][Uu][Gg] )
   INSTALL( TARGETS TestAppCommandLine
      DESTINATION test
   )
ENDIF( CMAKE_BUILD_TYPE MATCHES [Dd][Ee][Bb][Uu][Gg] )
