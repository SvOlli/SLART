#
# src/libs/Satellite/CMakeLists.txt
#
# written by SvOlli
# distributed as public domain
#

MESSAGE( STATUS "==== src/libs/Satellite ====" )

PROJECT( Satellite )

INCLUDE( LocalLibrary )

SET( QT_USE_QTNETWORK 1 )
INCLUDE( ${QT_USE_FILE} )
ADD_DEFINITIONS( ${QT_DEFINITIONS} )
INCLUDE_DIRECTORIES( . )
ADD_DEFINITIONS( -fPIC )

SET( Satellite_PUBLIC_HEADERS
   Satellite.hpp
   SatelliteServer.hpp
   SatelliteSingleSend.hpp
)

SET( Satellite_PRIVATE_HEADERS
)

SET( Satellite_SOURCES
   Satellite.cpp
   SatelliteServer.cpp
   SatelliteSingleSend.cpp
)

QT4_WRAP_CPP( Satellite_MOC_HEADERS
   ${Satellite_PUBLIC_HEADERS}
   ${Satellite_PRIVATE_HEADERS}
)

ADD_LIBRARY( Satellite STATIC
   ${Satellite_MOC_HEADERS}
   ${Satellite_PUBLIC_HEADERS}
   ${Satellite_PRIVATE_HEADERS}
   ${Satellite_SOURCES}
)

PROPAGATE_LIBRARY( Satellite )
