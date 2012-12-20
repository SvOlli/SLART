#
# src/tests/TestAppSatellite/CMakeLists.txt
#
# written by SvOlli
# distributed as public domain
#

MESSAGE( STATUS "==== src/tests/TextAppSatellite ====" )

PROJECT( TestAppSatellite )

SET( QT_USE_QTNETWORK 1 )
INCLUDE( ${QT_USE_FILE} )
INCLUDE_DIRECTORIES( ${SATELLITE_INCLUDE_DIR} )
ADD_DEFINITIONS( ${QT_DEFINITIONS} -DSATELLITE_DEBUG=1 )

SET( TestAppSatellite_HEADERS
   MainWidget.hpp
   MainWindow.hpp
   ../../libs/Satellite/Satellite.hpp
   ../../libs/Satellite/SatelliteServer.hpp
)

SET( TestAppSatellite_SOURCES
   Application.cpp
   MainWidget.cpp
   MainWindow.cpp
   ../../libs/Satellite/Satellite.cpp
   ../../libs/Satellite/SatelliteServer.cpp
)

QT4_WRAP_CPP( TestAppSatellite_MOC_HEADERS
   ${TestAppSatellite_HEADERS}
)

ADD_EXECUTABLE( TestAppSatellite
   ${TestAppSatellite_MOC_HEADERS}
   ${TestAppSatellite_HEADERS}
   ${TestAppSatellite_SOURCES}
)

#ADD_DEPENDENCIES( TestAppSatellite
#   Satellite
#)

#   ${SATELLITE_LIBRARY_FLAGS}
TARGET_LINK_LIBRARIES( TestAppSatellite
   ${QT_LIBRARIES}
)

IF( CMAKE_BUILD_TYPE MATCHES [Dd][Ee][Bb][Uu][Gg] )
   INSTALL( TARGETS TestAppSatellite
      DESTINATION test
   )
ENDIF( CMAKE_BUILD_TYPE MATCHES [Dd][Ee][Bb][Uu][Gg] )
