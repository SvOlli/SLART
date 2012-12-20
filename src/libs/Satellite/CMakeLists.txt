#
# src/libs/Satellite/CMakeLists.txt
#
# written by SvOlli
# distributed as public domain
#

MESSAGE( STATUS "==== src/libs/Satellite ====" )

PROJECT( Satellite )

INCLUDE( LocalTools )

SET( QT_USE_QTNETWORK 1 )
INCLUDE( ${QT_USE_FILE} )
ADD_DEFINITIONS( ${QT_DEFINITIONS} )
ADD_DEFINITIONS( -fPIC )

ADD_SOURCE( Satellite           cpp hpp moc )
ADD_SOURCE( SatelliteServer     cpp hpp moc )
ADD_SOURCE( SatelliteSingleSend cpp hpp moc )

QT4_WRAP_CPP( Satellite_MOC_GENERATED
   ${Satellite_MOC_HEADERS}
)

ADD_LIBRARY( Satellite STATIC
   ${Satellite_MOC_GENERATED}
   ${Satellite_HEADERS}
   ${Satellite_SOURCES}
)

PROPAGATE_LIBRARY( Satellite )
