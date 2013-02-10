#
# src/tests/TestAppGSettings/CMakeLists.txt
#
# written by SvOlli
# distributed as public domain
#

MESSAGE( STATUS "==== src/tests/TestAppGSettings ====" )

PROJECT( TestAppGSettings )

#SET( QT_USE_QTNETWORK 1 )
INCLUDE( ${QT_USE_FILE} )
ADD_DEFINITIONS( ${QT_DEFINITIONS} )
INCLUDE_DIRECTORIES(
   ${COMMANDLINE_INCLUDE_DIR}
   ${COMMON_INCLUDE_DIR}
)

ADD_SOURCE( Application cpp )

QT4_WRAP_CPP( TestAppGSettings_MOC_GENERATED
   ${TestAppGSettings_MOC_HEADERS}
)

ADD_EXECUTABLE( TestAppGSettings
   ${TestAppGSettings_MOC_GENERATED}
   ${TestAppGSettings_HEADERS}
   ${TestAppGSettings_SOURCES}
)

ADD_DEPENDENCIES( TestAppGSettings
   CommandLine
   Common
)

TARGET_LINK_LIBRARIES( TestAppGSettings
   ${COMMANDLINE_LIBRARY_FLAGS}
   ${COMMON_LIBRARY_FLAGS}
   ${QT_LIBRARIES}
)

IF( CMAKE_BUILD_TYPE MATCHES [Dd][Ee][Bb][Uu][Gg] )
   INSTALL( TARGETS TestAppGSettings
      DESTINATION test
   )
ENDIF( CMAKE_BUILD_TYPE MATCHES [Dd][Ee][Bb][Uu][Gg] )
