#
# src/apps/Rubberbandman/CMakeLists.txt
#
# written by SvOlli
# distributed as public domain
#

MESSAGE( STATUS "==== src/apps/Rubberbandman ====" )

PROJECT( Rubberbandman )

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


ADD_SOURCE( BrowseWidget             cpp hpp moc )
ADD_SOURCE( ConfigDialog             cpp hpp moc )
ADD_SOURCE( Console                  cpp hpp moc )
ADD_SOURCE( DatabaseWidget           cpp hpp moc )
ADD_SOURCE( DatabaseWorker           cpp hpp moc )
ADD_SOURCE( DirWalkerDelete          cpp hpp     )
ADD_SOURCE( DirWalkerMove            cpp hpp     )
ADD_SOURCE( DirWalkerUpdate          cpp hpp     )
ADD_SOURCE( FileSysBrowser           cpp hpp moc )
ADD_SOURCE( InfoEdit                 cpp hpp moc )
ADD_SOURCE( LineEdit                 cpp hpp moc )
ADD_SOURCE( MyTreeView               cpp hpp moc )
ADD_SOURCE( RecurseWorker            cpp hpp moc )
ADD_SOURCE( RubberbandmanApplication cpp         )
ADD_SOURCE( RubberbandmanMainWidget  cpp hpp moc )
ADD_SOURCE( SatelliteWidget          cpp hpp moc )
ADD_SOURCE( TrackReader              cpp hpp moc )
ADD_SOURCE( TrackWriter              cpp hpp moc )

SET( Rubberbandman_RESOURCES
)

FILE( GLOB Rubberbandman_TS_FILES
   ${TRANSLATIONS_SOURCE_DIR}/Rubberbandman_*.ts
   ${TRANSLATIONS_SOURCE_DIR}/Rubberbandman.ts
)

QT4_WRAP_CPP( Rubberbandman_MOC_GENERATED
   ${Rubberbandman_MOC_HEADERS}
)

QT4_ADD_RESOURCES( Rubberbandman_RESOURCES_RCC
   ${Rubberbandman_RESOURCES}
)

QT4_ADD_TRANSLATION( Rubberbandman_QM_FILES
   ${Rubberbandman_TS_FILES}
)

ADD_EXECUTABLE( Rubberbandman
   ${Rubberbandman_MOC_GENERATED}
   ${Rubberbandman_HEADERS}
   ${Rubberbandman_SOURCES}
   ${Rubberbandman_QM_FILES}
   ${Rubberbandman_RESOURCES_RCC}
)

ADD_DEPENDENCIES( Rubberbandman
   Satellite
   Common
   TagLib
)

TARGET_LINK_LIBRARIES( Rubberbandman
   ${COMMON_LIBRARY_FLAGS}
   ${SATELLITE_LIBRARY_FLAGS}
   ${TAGLIB_LIBRARY_FLAGS}
   ${QT_LIBRARIES}
)

INSTALL( TARGETS Rubberbandman
   RUNTIME DESTINATION bin
   LIBRARY DESTINATION lib
)
