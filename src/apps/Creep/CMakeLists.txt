#
# src/apps/Creep/CMakeLists.txt
#
# written by SvOlli
# distributed as public domain
#

MESSAGE( STATUS "==== src/apps/Creep ====" )

PROJECT( Creep )

INCLUDE( FindPkgConfig )

PKG_CHECK_MODULES( LIRCCLIENT REQUIRED liblircclient0 )

SET( QT_USE_QTNETWORK 1 )
INCLUDE( ${QT_USE_FILE} )
INCLUDE_DIRECTORIES( ${SATELLITE_INCLUDE_DIR} ${LIRCCLIENT_INCLUDE_DIR} )
ADD_DEFINITIONS( ${QT_DEFINITIONS} )

SET( Creep_HEADERS
)

SET( Creep_SOURCES
   Application.cpp
)

QT4_WRAP_CPP( Creep_MOC_HEADERS
   ${Creep_HEADERS}
)

ADD_EXECUTABLE( Creep
   ${Creep_MOC_HEADERS}
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
