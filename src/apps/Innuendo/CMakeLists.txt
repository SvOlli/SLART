#
# src/apps/Innuendo/CMakeLists.txt
#
# written by SvOlli
# distributed as public domain
#

MESSAGE( STATUS "==== src/apps/Innuendo ====" )

PROJECT( Innuendo )

INCLUDE( LocalTools )

SET( QT_USE_QTNETWORK 1 )
INCLUDE( ${QT_USE_FILE} )
ADD_DEFINITIONS( ${QT_DEFINITIONS} )
INCLUDE_DIRECTORIES(
   ${SATELLITE_INCLUDE_DIR}
   ${COMMON_INCLUDE_DIR}
)

ADD_SOURCE( ConfigDialog          cpp hpp moc )
ADD_SOURCE( DropDialog            cpp hpp moc )
ADD_SOURCE( ExecButton            cpp hpp moc )
ADD_SOURCE( InnuendoApplication   cpp         )
ADD_SOURCE( InnuendoMainWidget    cpp hpp moc )
ADD_SOURCE( LogListWidget         cpp hpp moc )
ADD_SOURCE( SatelliteConfigWidget cpp hpp moc )

SET( Innuendo_RESOURCES
)

FILE( GLOB Innuendo_TS_FILES
   ${TRANSLATIONS_SOURCE_DIR}/Innuendo_*.ts
)

QT4_WRAP_CPP( Innuendo_MOC_GENERATED
   ${Innuendo_MOC_HEADERS}
)

QT4_ADD_RESOURCES( Innuendo_RESOURCES_RCC
)

QT4_ADD_TRANSLATION( Innuendo_QM_FILES
   ${Innuendo_TS_FILES}
)

ADD_EXECUTABLE( Innuendo
   ${Innuendo_MOC_GENERATED}
   ${Innuendo_HEADERS}
   ${Innuendo_SOURCES}
   ${Innuendo_QM_FILES}
   ${Innuendo_RESOURCES_RCC}
)

ADD_DEPENDENCIES( Innuendo
   Satellite
   Common
)

TARGET_LINK_LIBRARIES( Innuendo
   ${COMMON_LIBRARY_FLAGS}
   ${SATELLITE_LIBRARY_FLAGS}
   ${QT_LIBRARIES}
)

INSTALL( TARGETS Innuendo
   RUNTIME DESTINATION bin
   LIBRARY DESTINATION lib
)
