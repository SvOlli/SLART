#
# src/tests/TestAppWebServer/CMakeLists.txt
#
# written by SvOlli
# distributed as public domain
#

MESSAGE( STATUS "==== src/tests/TextAppWebServer ====" )

PROJECT( TestAppWebServer )

SET( QT_USE_QTNETWORK 1 )
SET( QT_USE_QTSQL 1 )
INCLUDE( ${QT_USE_FILE} )
ADD_DEFINITIONS( ${QT_DEFINITIONS} )
INCLUDE_DIRECTORIES(
   ${SATELLITE_INCLUDE_DIR}
   ${COMMON_INCLUDE_DIR}
)

ADD_SOURCE( Application          cpp         )
ADD_SOURCE( MainWidget           cpp hpp moc )
ADD_SOURCE( WebServerSanityTests cpp hpp     )

SET( TestAppWebServer_RESOURCES
   Resources/TestAppWebServer.qrc
)

QT4_WRAP_CPP( TestAppWebServer_MOC_GENERATED
   ${TestAppWebServer_MOC_HEADERS}
)

QT4_ADD_RESOURCES( TestAppWebServer_RESOURCES_RCC
   ${TestAppWebServer_RESOURCES}
)

ADD_EXECUTABLE( TestAppWebServer
   ${TestAppWebServer_MOC_GENERATED}
   ${TestAppWebServer_HEADERS}
   ${TestAppWebServer_SOURCES}
   ${TestAppWebServer_RESOURCES_RCC}
)

ADD_DEPENDENCIES( TestAppWebServer
   Common
   Satellite
)

TARGET_LINK_LIBRARIES( TestAppWebServer
   ${COMMON_LIBRARY_FLAGS}
   ${SATELLITE_LIBRARY_FLAGS}
   ${QT_LIBRARIES}
)

IF( CMAKE_BUILD_TYPE MATCHES [Dd][Ee][Bb][Uu][Gg] )
   INSTALL( TARGETS TestAppWebServer
      DESTINATION test
   )
ENDIF( CMAKE_BUILD_TYPE MATCHES [Dd][Ee][Bb][Uu][Gg] )
