#
# src/apps/Stripped/CMakeLists.txt
#
# written by SvOlli
# distributed as public domain
#

MESSAGE( STATUS "==== src/apps/Stripped ====" )

PROJECT( Stripped )

INCLUDE( LocalTools )
INCLUDE( FindPkgConfig )
PKG_CHECK_MODULES( CDDBPARANOIA REQUIRED libcdio_paranoia )

SET( QT_USE_QTNETWORK 1 )
INCLUDE( ${QT_USE_FILE} )
INCLUDE_DIRECTORIES(
   ${MAGICINTERFACE_INCLUDE_DIR}
   ${CDDBPARANOIA_INCLUDE}
   ${SATELLITE_INCLUDE_DIR}
   ${COMMON_INCLUDE_DIR}
)
ADD_DEFINITIONS( ${QT_DEFINITIONS} )

ADD_SOURCE( CDDBClient           cpp hpp moc )
ADD_SOURCE( CDEditCheckBox       cpp hpp moc )
ADD_SOURCE( CDEdit               cpp hpp moc )
ADD_SOURCE( CDEditLineEdit       cpp hpp moc )
ADD_SOURCE( CDInfo               cpp hpp     )
ADD_SOURCE( CDReader             cpp hpp moc )
ADD_SOURCE( CDReaderThread       cpp hpp moc )
ADD_SOURCE( ParanoiaStatus       cpp hpp moc )
ADD_SOURCE( StrippedApplication  cpp         )
ADD_SOURCE( StrippedConfigDialog cpp hpp moc )
ADD_SOURCE( StrippedMainWindow   cpp hpp moc )

SET( Stripped_RESOURCES
   ../../resources/Stripped.qrc
)

FILE( GLOB Stripped_TS_FILES
   ${TRANSLATIONS_SOURCE_DIR}/Stripped_*.ts
)

QT4_WRAP_CPP( Stripped_MOC_GENERATED
   ${Stripped_MOC_HEADERS}
)

QT4_ADD_RESOURCES( Stripped_RESOURCES_RCC
   ${Stripped_RESOURCES}
)

QT4_ADD_TRANSLATION( Stripped_QM_FILES
   ${Stripped_TS_FILES}
)

ADD_EXECUTABLE( Stripped
   ${Stripped_MOC_GENERATED}
   ${Stripped_HEADERS}
   ${Stripped_SOURCES}
   ${Stripped_QM_FILES}
   ${Stripped_RESOURCES_RCC}
)

ADD_DEPENDENCIES( Stripped
   MagicInterface
   Satellite
   Common
)

TARGET_LINK_LIBRARIES( Stripped
   ${COMMON_LIBRARY_FLAGS}
   ${SATELLITE_LIBRARY_FLAGS}
   ${CDDBPARANOIA_LIBRARIES}
   ${QT_LIBRARIES}
)

INSTALL( TARGETS Stripped
   RUNTIME DESTINATION bin
   LIBRARY DESTINATION lib
)
