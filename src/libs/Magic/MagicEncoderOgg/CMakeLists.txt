#
# src/libs/Magic/MagicEncoderOgg/CMakeLists.txt
#
# written by SvOlli
# distributed as public domain
#

MESSAGE( STATUS "==== src/libs/Magic/MagicEncoderOgg ====" )

PROJECT( MagicEncoderOgg )

INCLUDE( LocalTools )
INCLUDE( FindPkgConfig )
PKG_CHECK_MODULES( VORBISFILE REQUIRED vorbisfile )

INCLUDE( ${QT_USE_FILE} )
ADD_DEFINITIONS( ${QT_DEFINITIONS} )
INCLUDE_DIRECTORIES(
   ${COMMON_INCLUDE_DIR}
   ${MAGICENCODER_INCLUDE_DIR}
   ${VORBISFILE_INCLUDE_DIR}
)

ADD_SOURCE( MagicEncoderOgg       cpp hpp moc )
ADD_SOURCE( MagicEncoderOggConfig cpp hpp moc )

FILE( GLOB MagicEncoderOgg_TS_FILES
   ${TRANSLATIONS_SOURCE_DIR}/MagicEncoderOgg_*.ts
)

QT4_ADD_TRANSLATION( MagicEncoderOgg_QM_FILES
   ${MagicEncoderOgg_TS_FILES}
)

QT4_WRAP_CPP( MagicEncoderOgg_MOC_GENERATED
   ${MagicEncoderOgg_MOC_HEADERS}
)

ADD_LIBRARY( MagicEncoderOgg MODULE
   ${MagicEncoderOgg_MOC_GENERATED}
   ${MagicEncoderOgg_HEADERS}
   ${MagicEncoderOgg_SOURCES}
   ${MagicEncoderOgg_QM_FILES}
)

TARGET_LINK_LIBRARIES( MagicEncoderOgg 
   ${MAGICENCODER_LIBRARY_FLAGS}
   ${COMMON_LIBRARY_FLAGS}
   ${VORBISFILE_LIBRARIES}
   ${QT_LIBRARIES}
)

ADD_DEPENDENCIES( MagicEncoderOgg
   Common
   MagicEncoder
)

INSTALL( TARGETS MagicEncoderOgg
   RUNTIME DESTINATION bin
   LIBRARY DESTINATION lib
)
