#
# src/tests/TestAppXxtea/CMakeLists.txt
#
# written by SvOlli
# distributed as public domain
#

MESSAGE( STATUS "==== src/tests/TestAppXxtea ====" )

PROJECT( TestAppXxtea )

#SET( QT_USE_QTNETWORK 1 )
INCLUDE( ${QT_USE_FILE} )
ADD_DEFINITIONS( ${QT_DEFINITIONS} )
INCLUDE_DIRECTORIES(
   ${COMMANDLINE_INCLUDE_DIR}
   ${COMMON_INCLUDE_DIR}
)

SET( TestAppXxtea_HEADERS
)

SET( TestAppXxtea_SOURCES
   Application.cpp
)

QT4_WRAP_CPP( TestAppXxtea_MOC_HEADERS
   ${TestAppXxtea_HEADERS}
)

ADD_EXECUTABLE( TestAppXxtea
   ${TestAppXxtea_MOC_HEADERS}
   ${TestAppXxtea_HEADERS}
   ${TestAppXxtea_SOURCES}
)

ADD_DEPENDENCIES( TestAppXxtea
   CommandLine
   Common
)

TARGET_LINK_LIBRARIES( TestAppXxtea
   ${COMMANDLINE_LIBRARY_FLAGS}
   ${COMMON_LIBRARY_FLAGS}
   ${QT_LIBRARIES}
)

IF( CMAKE_BUILD_TYPE MATCHES [Dd][Ee][Bb][Uu][Gg] )
   INSTALL( TARGETS TestAppXxtea
      DESTINATION test
   )
ENDIF( CMAKE_BUILD_TYPE MATCHES [Dd][Ee][Bb][Uu][Gg] )
