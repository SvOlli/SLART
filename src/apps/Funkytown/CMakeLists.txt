#
# src/apps/Funkytown/CMakeLists.txt
#
# written by SvOlli
# distributed as public domain
#

MESSAGE( STATUS "==== src/apps/Funkytown ====" )

PROJECT( Funkytown )

INCLUDE( LocalTools )

SET( QT_USE_QTNETWORK 1 )
INCLUDE( ${QT_USE_FILE} )
ADD_DEFINITIONS( ${QT_DEFINITIONS} )
INCLUDE_DIRECTORIES(
   ${SATELLITE_INCLUDE_DIR}
   ${COMMON_INCLUDE_DIR}
)

ADD_SOURCE( Cookie                cpp hpp     )
ADD_SOURCE( CookieJar             cpp hpp     )
ADD_SOURCE( DownloadHandler       cpp hpp moc )
ADD_SOURCE( FunkytownApplication  cpp         )
ADD_SOURCE( FunkytownConfigDialog cpp hpp moc )
ADD_SOURCE( FunkytownMainWidget   cpp hpp moc )
ADD_SOURCE( MagicQueue            cpp hpp moc )
ADD_SOURCE( TheMagic              cpp hpp moc )

SET( Funkytown_RESOURCES
   ../../resources/Funkytown.qrc
)

FILE( GLOB Funkytown_TS_FILES
   ${TRANSLATIONS_SOURCE_DIR}/Funkytown_*.ts
)

QT4_WRAP_CPP( Funkytown_MOC_GENERATED
   ${Funkytown_MOC_HEADERS}
)

QT4_ADD_RESOURCES( Funkytown_RESOURCES_RCC
   ${Funkytown_RESOURCES}
)

QT4_ADD_TRANSLATION( Funkytown_QM_FILES
   ${Funkytown_TS_FILES}
)

ADD_EXECUTABLE( Funkytown
   ${Funkytown_MOC_GENERATED}
   ${Funkytown_HEADERS}
   ${Funkytown_SOURCES}
   ${Funkytown_QM_FILES}
   ${Funkytown_RESOURCES_RCC}
)

ADD_DEPENDENCIES( Funkytown
   Satellite
   Common
)

TARGET_LINK_LIBRARIES( Funkytown
   ${COMMON_LIBRARY_FLAGS}
   ${SATELLITE_LIBRARY_FLAGS}
   ${QT_LIBRARIES}
)

INSTALL( TARGETS Funkytown
   RUNTIME DESTINATION bin
   LIBRARY DESTINATION lib
)
