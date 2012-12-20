#
# src/apps/Creep/CMakeLists.txt
#
# written by SvOlli
# distributed as public domain
#

MESSAGE( STATUS "==== src/apps/Creep ====" )

PROJECT( Creep )

INCLUDE( LocalTools )
INCLUDE( FindPkgConfig )

PKG_CHECK_MODULES( LIRCCLIENT REQUIRED liblircclient0 )

SET( QT_USE_QTNETWORK 1 )
INCLUDE( ${QT_USE_FILE} )
ADD_DEFINITIONS( ${QT_DEFINITIONS} )
INCLUDE_DIRECTORIES(
   ${SATELLITE_INCLUDE_DIR}
   ${LIRCCLIENT_INCLUDE_DIR}
)

ADD_SOURCE( Application cpp )

QT4_WRAP_CPP( Creep_MOC_GENERATED
   ${Creep_MOC_HEADERS}
)

ADD_EXECUTABLE( Creep
   ${Creep_MOC_GENERATED}
   ${Creep_HEADERS}
   ${Creep_SOURCES}
)

ADD_DEPENDENCIES( Creep
   Satellite
   Common
)

TARGET_LINK_LIBRARIES( Creep
   ${LIRCCLIENT_LIBRARIES}
   ${QT_LIBRARIES}
)

INSTALL( TARGETS Creep
   LIBRARY DESTINATION lib
   RUNTIME DESTINATION bin
)
