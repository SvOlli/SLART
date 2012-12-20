#
# src/tools/SettingsGenerator/CMakeLists.txt
#
# written by SvOlli
# distributed as public domain
#

MESSAGE( STATUS "==== src/tools/SettingsGenerator ====" )

PROJECT( SettingsGenerator )

INCLUDE( LocalTools )

SET( QT_DONT_USE_QTGUI 1 )
INCLUDE( ${QT_USE_FILE} )
ADD_DEFINITIONS( ${QT_DEFINITIONS} )

ADD_SOURCE( Application              cpp     )
ADD_SOURCE( SettingsGeneratorHandler cpp hpp )
ADD_SOURCE( SettingsGeneratorStorage cpp hpp )

ADD_EXECUTABLE( SettingsGenerator
   ${SettingsGenerator_SOURCES}
   ${SettingsGenerator_HEADERS}
)
TARGET_LINK_LIBRARIES( SettingsGenerator
   ${QT_LIBRARIES}
)

IF( CMAKE_BUILD_TYPE MATCHES [Dd][Ee][Bb][Uu][Gg] )
   INSTALL( TARGETS SettingsGenerator
      DESTINATION tools
   )
ENDIF( CMAKE_BUILD_TYPE MATCHES [Dd][Ee][Bb][Uu][Gg] )
