#
# src/tests/TestAppSatellite/CMakeLists.txt
#
# written by SvOlli
# distributed as public domain
#

MESSAGE( STATUS "==== src/tests/TextAppSatellite ====" )

PROJECT( TestAppSatellite )

INCLUDE( LocalTools )

SET( QT_USE_QTNETWORK 1 )
INCLUDE( ${QT_USE_FILE} )
INCLUDE_DIRECTORIES( ${SATELLITE_INCLUDE_DIR} )
ADD_DEFINITIONS( ${QT_DEFINITIONS} -DSATELLITE_DEBUG=1 )


ADD_SOURCE( Application                          cpp         )
ADD_SOURCE( MainWidget                           cpp hpp moc )
ADD_SOURCE( MainWindow                           cpp hpp moc )
ADD_SOURCE( ../../libs/Satellite/Satellite       cpp hpp moc )
ADD_SOURCE( ../../libs/Satellite/SatelliteServer cpp hpp moc )

QT4_WRAP_CPP( TestAppSatellite_MOC_GENERATED
   ${TestAppSatellite_MOC_HEADERS}
)

ADD_EXECUTABLE( TestAppSatellite
   ${TestAppSatellite_MOC_GENERATED}
   ${TestAppSatellite_HEADERS}
   ${TestAppSatellite_SOURCES}
)

TARGET_LINK_LIBRARIES( TestAppSatellite
   ${QT_LIBRARIES}
)

IF( CMAKE_BUILD_TYPE MATCHES [Dd][Ee][Bb][Uu][Gg] )
   INSTALL( TARGETS TestAppSatellite
      DESTINATION test
   )
ENDIF( CMAKE_BUILD_TYPE MATCHES [Dd][Ee][Bb][Uu][Gg] )
