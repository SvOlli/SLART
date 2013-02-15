#
# src/tests/TestAppFileSysCompleter/CMakeLists.txt
#
# written by SvOlli
# distributed as public domain
#

MESSAGE( STATUS "==== src/tests/TestAppFileSysCompleter ====" )

PROJECT( TestAppFileSysCompleter )

#SET( QT_USE_QTNETWORK 1 )
INCLUDE( ${QT_USE_FILE} )
ADD_DEFINITIONS( ${QT_DEFINITIONS} )
INCLUDE_DIRECTORIES(
   ${COMMANDLINE_INCLUDE_DIR}
   ${COMMON_INCLUDE_DIR}
)

ADD_SOURCE( Application cpp )

QT4_WRAP_CPP( TestAppFileSysCompleter_MOC_GENERATED
   ${TestAppFileSysCompleter_MOC_HEADERS}
)

ADD_EXECUTABLE( TestAppFileSysCompleter
   ${TestAppFileSysCompleter_MOC_GENERATED}
   ${TestAppFileSysCompleter_HEADERS}
   ${TestAppFileSysCompleter_SOURCES}
)

ADD_DEPENDENCIES( TestAppFileSysCompleter
   CommandLine
   Common
)

TARGET_LINK_LIBRARIES( TestAppFileSysCompleter
   ${COMMANDLINE_LIBRARY_FLAGS}
   ${COMMON_LIBRARY_FLAGS}
   ${QT_LIBRARIES}
)

IF( CMAKE_BUILD_TYPE MATCHES [Dd][Ee][Bb][Uu][Gg] )
   INSTALL( TARGETS TestAppFileSysCompleter
      DESTINATION test
   )
ENDIF( CMAKE_BUILD_TYPE MATCHES [Dd][Ee][Bb][Uu][Gg] )
