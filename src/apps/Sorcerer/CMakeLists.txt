#
# src/apps/Sorcerer/CMakeLists.txt
#
# written by SvOlli
# distributed as public domain
#

MESSAGE( STATUS "==== src/apps/Sorcerer ====" )

PROJECT( Sorcerer )

INCLUDE( LocalTools )

SET( QT_USE_QTNETWORK 1 )
SET( QT_USE_QTSQL 1 )
INCLUDE( ${QT_USE_FILE} )
ADD_DEFINITIONS( ${QT_DEFINITIONS} )
INCLUDE_DIRECTORIES(
   ${SATELLITE_INCLUDE_DIR}
   ${COMMON_INCLUDE_DIR}
   ${TAGLIB_INCLUDE_DIR}
)

ADD_SOURCE( SorcererApplication               cpp         )
ADD_SOURCE( SorcererMainWidget                cpp hpp moc )
ADD_SOURCE( ../Innuendo/SatelliteConfigWidget cpp hpp moc )
ADD_SOURCE( ../Rubberbandman/DatabaseWidget   cpp hpp moc )
ADD_SOURCE( ../Rubberbandman/DatabaseWorker   cpp hpp moc )
ADD_SOURCE( ../Rubberbandman/DirWalkerUpdate  cpp hpp     )

SET( Sorcerer_RESOURCES
)

FILE( GLOB Sorcerer_TS_FILES
   ${TRANSLATIONS_SOURCE_DIR}/Sorcerer_*.ts
)

QT4_WRAP_CPP( Sorcerer_MOC_GENERATED
   ${Sorcerer_MOC_HEADERS}
)

QT4_ADD_TRANSLATION( Sorcerer_QM_FILES
   ${Sorcerer_TS_FILES}
)

QT4_ADD_RESOURCES( Sorcerer_RESOURCES_RCC
   ${Sorcerer_RESOURCES}
)

ADD_EXECUTABLE( Sorcerer
   ${Sorcerer_MOC_GENERATED}
   ${Sorcerer_HEADERS}
   ${Sorcerer_SOURCES}
   ${Sorcerer_RESOURCES_RCC}
   ${Sorcerer_QM_FILES}
)

ADD_DEPENDENCIES( Sorcerer
   Satellite
   Common
   TagLib
)

TARGET_LINK_LIBRARIES( Sorcerer
   ${COMMON_LIBRARY_FLAGS}
   ${SATELLITE_LIBRARY_FLAGS}
   ${TAGLIB_LIBRARY_FLAGS}
   ${QT_LIBRARIES}
)

INSTALL( TARGETS Sorcerer
   RUNTIME DESTINATION bin
   LIBRARY DESTINATION lib
)
