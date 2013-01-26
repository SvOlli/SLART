#
# src/apps/Notorious/CMakeLists.txt
#
# written by SvOlli
# distributed as public domain
#

MESSAGE( STATUS "==== src/apps/Notorious ====" )

PROJECT( Notorious )

INCLUDE( LocalTools )
INCLUDE( FindBZip2 )
IF( NOT BZIP2_FOUND )
   MESSAGE( SEND_ERROR "BZip2 not found" )
ENDIF( NOT BZIP2_FOUND )

SET( QT_USE_QTNETWORK 1 )
SET( QT_USE_QTSQL 1 )
INCLUDE( ${QT_USE_FILE} )
ADD_DEFINITIONS( ${QT_DEFINITIONS} )
INCLUDE_DIRECTORIES(
   ${SATELLITE_INCLUDE_DIR}
   ${COMMON_INCLUDE_DIR}
   ${BZIP2_INCLUDE_DIR}
)


ADD_SOURCE( Foundlist             cpp hpp moc )
ADD_SOURCE( FreeDB                cpp hpp moc )
ADD_SOURCE( FreeDBEntry           cpp hpp     )
ADD_SOURCE( FreeDBImport          cpp hpp moc )
ADD_SOURCE( FreeDBQuery           cpp hpp moc )
ADD_SOURCE( NotoriousApplication  cpp         )
ADD_SOURCE( NotoriousConfigDialog cpp hpp moc )
ADD_SOURCE( NotoriousMainWidget   cpp hpp moc )
ADD_SOURCE( SearchWidget          cpp hpp moc )
ADD_SOURCE( TarEntry              cpp hpp     )
ADD_SOURCE( tir                   c   h       )

SET( Notorious_RESOURCES
   ../../resources/Notorious.qrc
)

FILE( GLOB Notorious_TS_FILES
   ${TRANSLATIONS_SOURCE_DIR}/Notorious_*.ts
)

QT4_WRAP_CPP( Notorious_MOC_GENERATED
   ${Notorious_MOC_HEADERS}
)

QT4_ADD_RESOURCES( Notorious_RESOURCES_RCC
   ${Notorious_RESOURCES}
)

QT4_ADD_TRANSLATION( Notorious_QM_FILES
   ${Notorious_TS_FILES}
)

ADD_EXECUTABLE( Notorious
   ${Notorious_MOC_GENERATED}
   ${Notorious_HEADERS}
   ${Notorious_SOURCES}
   ${Notorious_QM_FILES}
   ${Notorious_RESOURCES_RCC}
)

ADD_DEPENDENCIES( Notorious
   Satellite
   Common
)

TARGET_LINK_LIBRARIES( Notorious
   ${COMMON_LIBRARY_FLAGS}
   ${SATELLITE_LIBRARY_FLAGS}
   ${QT_LIBRARIES}
   ${BZIP2_LIBRARIES}
)

INSTALL( TARGETS Notorious
   RUNTIME DESTINATION bin
   LIBRARY DESTINATION lib
)
