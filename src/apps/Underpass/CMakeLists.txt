#
# src/apps/Underpass/CMakeLists.txt
#
# written by SvOlli
# distributed as public domain
#

MESSAGE( STATUS "==== src/apps/Underpass ====" )

PROJECT( Underpass )

INCLUDE( LocalTools )

SET( QT_USE_QTNETWORK 1 )
INCLUDE( ${QT_USE_FILE} )
ADD_DEFINITIONS( ${QT_DEFINITIONS} )
INCLUDE_DIRECTORIES(
   ${SATELLITE_INCLUDE_DIR}
   ${COMMON_INCLUDE_DIR}
)

ADD_SOURCE( StationStorage         cpp hpp moc )
ADD_SOURCE( UnderpassApplication   cpp         )
ADD_SOURCE( UnderpassConfigDialog  cpp hpp moc )
ADD_SOURCE( UnderpassMainWidget    cpp hpp moc )

SET( Underpass_RESOURCES
   ../../resources/Underpass.qrc
)

FILE( GLOB Underpass_TS_FILES
   ${TRANSLATIONS_SOURCE_DIR}/Underpass_*.ts
)

QT4_WRAP_CPP( Underpass_MOC_GENERATED
   ${Underpass_MOC_HEADERS}
)

QT4_ADD_RESOURCES( Underpass_RESOURCES_RCC
   ${Underpass_RESOURCES}
)

QT4_ADD_TRANSLATION( Underpass_QM_FILES
   ${Underpass_TS_FILES}
)

ADD_EXECUTABLE( Underpass
   ${Underpass_MOC_GENERATED}
   ${Underpass_HEADERS}
   ${Underpass_SOURCES}
   ${Underpass_QM_FILES}
   ${Underpass_RESOURCES_RCC}
)

ADD_DEPENDENCIES( Underpass
   Satellite
   Common
)

TARGET_LINK_LIBRARIES( Underpass
   ${COMMON_LIBRARY_FLAGS}
   ${SATELLITE_LIBRARY_FLAGS}
   ${QT_LIBRARIES}
)

INSTALL( TARGETS Underpass
   RUNTIME DESTINATION bin
   LIBRARY DESTINATION lib
)
