#
# src/apps/Partyman/CMakeLists.txt
#
# written by SvOlli
# distributed as public domain
#

MESSAGE( STATUS "==== src/apps/Partyman ====" )

PROJECT( Partyman )

INCLUDE( LocalTools )

SET( QT_USE_QTNETWORK 1 )
SET( QT_USE_QTSQL 1 )
INCLUDE( ${QT_USE_FILE} )
ADD_DEFINITIONS( ${QT_DEFINITIONS} )
INCLUDE_DIRECTORIES(
   ${SATELLITE_INCLUDE_DIR}
   ${COMMON_INCLUDE_DIR}
)

ADD_SOURCE( CommandLineHandler    cpp hpp moc )
ADD_SOURCE( ControlWidget         cpp hpp moc )
ADD_SOURCE( FileSysTreeItem       cpp hpp     )
ADD_SOURCE( FileSysTreeModel      cpp hpp moc )
ADD_SOURCE( FileSysTreeUpdate     cpp hpp     )
ADD_SOURCE( FileSysTreeView       cpp hpp moc )
ADD_SOURCE( SearchLineEdit        cpp hpp     )
ADD_SOURCE( SearchTrackWidget     cpp hpp moc )
ADD_SOURCE( TimeSlider            cpp hpp     )
ADD_SOURCE( TrackInfoListModel    cpp hpp moc )
ADD_SOURCE( PartymanApplication   cpp         )
ADD_SOURCE( PartymanConfigDialog  cpp hpp moc )
ADD_SOURCE( PartymanMainWindow    cpp hpp moc )
ADD_SOURCE( PlayerWidget          cpp hpp moc )
ADD_SOURCE( PlayerFSM             cpp hpp     )
ADD_SOURCE( PlayerFSMBase         cpp hpp     )
ADD_SOURCE( PlayerFSMDisconnected cpp hpp     )
ADD_SOURCE( PlayerFSMEnding       cpp hpp     )
ADD_SOURCE( PlayerFSMEndingPaused cpp hpp     )
ADD_SOURCE( PlayerFSMLoading      cpp hpp     )
ADD_SOURCE( PlayerFSMPaused       cpp hpp     )
ADD_SOURCE( PlayerFSMPlaying      cpp hpp     )
ADD_SOURCE( PlayerFSMReady        cpp hpp     )
ADD_SOURCE( PlayerFSMSearching    cpp hpp     )
ADD_SOURCE( PlaylistContentWidget cpp hpp moc )

SET( Partyman_RESOURCES
   Resources/Partyman.qrc
)

QT4_ADD_RESOURCES( Partyman_RESOURCES_RCC
   ${Partyman_RESOURCES}
)

FILE( GLOB Partyman_TS_FILES
   ${TRANSLATIONS_SOURCE_DIR}/Partyman_*.ts
)

QT4_WRAP_CPP( Partyman_MOC_GENERATED
   ${Partyman_MOC_HEADERS}
)

QT4_ADD_TRANSLATION( Partyman_QM_FILES
   ${Partyman_TS_FILES}
)

ADD_EXECUTABLE( Partyman
   ${Partyman_MOC_GENERATED}
   ${Partyman_HEADERS}
   ${Partyman_SOURCES}
   ${Partyman_QM_FILES}
   ${Partyman_RESOURCES_RCC}
)

ADD_DEPENDENCIES( Partyman
   Satellite
   Common
)

TARGET_LINK_LIBRARIES( Partyman
   ${COMMON_LIBRARY_FLAGS}
   ${SATELLITE_LIBRARY_FLAGS}
   ${QT_LIBRARIES}
)

INSTALL( TARGETS Partyman
   RUNTIME DESTINATION bin
   LIBRARY DESTINATION lib
)
