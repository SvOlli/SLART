#
# src/libs/Magic/MagicEncoderFlac/CMakeLists.txt
#
# written by SvOlli
# distributed as public domain
#

MESSAGE( STATUS "==== src/libs/Magic/MagicEncoderFlac ====" )

PROJECT( MagicEncoderFlac )

INCLUDE( LocalTools )

INCLUDE( ${QT_USE_FILE} )
ADD_DEFINITIONS( ${QT_DEFINITIONS} )

INCLUDE( FindPkgConfig )
PKG_CHECK_MODULES( FLAC++ REQUIRED flac++ )

INCLUDE_DIRECTORIES(
   ${COMMON_INCLUDE_DIR}
   ${MAGICENCODER_INCLUDE_DIR}
   ${FLAC++_INCLUDE_DIR}
)

ADD_SOURCE( MagicEncoderFlac       cpp hpp moc )
ADD_SOURCE( MagicEncoderFlacConfig cpp hpp moc )

FILE( GLOB MagicEncoderFlac_TS_FILES
   ${TRANSLATIONS_SOURCE_DIR}/MagicEncoderFlac_*.ts
)

QT4_ADD_TRANSLATION( MagicEncoderFlac_QM_FILES
   ${MagicEncoderFlac_TS_FILES}
)

QT4_WRAP_CPP( MagicEncoderFlac_MOC_GENERATED
   ${MagicEncoderFlac_MOC_HEADERS}
)

ADD_LIBRARY( MagicEncoderFlac MODULE
   ${MagicEncoderFlac_MOC_GENERATED}
   ${MagicEncoderFlac_HEADERS}
   ${MagicEncoderFlac_SOURCES}
   ${MagicEncoderFlac_QM_FILES}
)

TARGET_LINK_LIBRARIES( MagicEncoderFlac
   ${MAGICENCODER_LIBRARY_FLAGS}
   ${COMMON_LIBRARY_FLAGS}
   ${FLAC++_LIBRARIES}
   ${QT_LIBRARIES}
)

ADD_DEPENDENCIES( MagicEncoderFlac
   Common
   MagicEncoder
)

INSTALL( TARGETS MagicEncoderFlac
   RUNTIME DESTINATION bin
   LIBRARY DESTINATION lib
)
