#
# src/tests/TestAppCoverArt/CMakeLists.txt
#
# written by SvOlli
# distributed as public domain
#

MESSAGE( STATUS "==== src/tests/TextAppCoverArt ====" )

PROJECT( TestAppCoverArt )

SET( QT_USE_QTNETWORK 1 )
SET( QT_USE_QTSQL 1 )
INCLUDE( ${QT_USE_FILE} )
ADD_DEFINITIONS( ${QT_DEFINITIONS} )
INCLUDE_DIRECTORIES(
   ${SATELLITE_INCLUDE_DIR}
   ${COMMON_INCLUDE_DIR}
)

ADD_SOURCE( Application                cpp         )
ADD_SOURCE( MainWidget                 cpp hpp moc )
ADD_SOURCE( Downloader                 cpp hpp moc )
ADD_SOURCE( DownloadJobCoverAmazonDE   cpp hpp moc )
ADD_SOURCE( IODeviceStreamer           cpp hpp moc )

SET( TestAppCoverArt_RESOURCES
)

QT4_WRAP_CPP( TestAppCoverArt_MOC_GENERATED
   ${TestAppCoverArt_MOC_HEADERS}
)

QT4_ADD_RESOURCES( TestAppCoverArt_RESOURCES_RCC
   ${TestAppCoverArt_RESOURCES}
)

ADD_EXECUTABLE( TestAppCoverArt
   ${TestAppCoverArt_MOC_GENERATED}
   ${TestAppCoverArt_HEADERS}
   ${TestAppCoverArt_SOURCES}
   ${TestAppCoverArt_RESOURCES_RCC}
)

ADD_DEPENDENCIES( TestAppCoverArt
   Common
   Satellite
)

TARGET_LINK_LIBRARIES( TestAppCoverArt
   ${COMMON_LIBRARY_FLAGS}
   ${SATELLITE_LIBRARY_FLAGS}
   ${QT_LIBRARIES}
)

IF( CMAKE_BUILD_TYPE MATCHES [Dd][Ee][Bb][Uu][Gg] )
   INSTALL( TARGETS TestAppCoverArt
      DESTINATION test
   )
ENDIF( CMAKE_BUILD_TYPE MATCHES [Dd][Ee][Bb][Uu][Gg] )
