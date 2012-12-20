#
# src/libs/Magic/MagicEncoderWav/CMakeLists.txt
#
# written by SvOlli
# distributed as public domain
#

MESSAGE( STATUS "==== src/libs/Magic/MagicEncoderWav ====" )

PROJECT( MagicEncoderWav )

INCLUDE( LocalTools )

INCLUDE( ${QT_USE_FILE} )
ADD_DEFINITIONS( ${QT_DEFINITIONS} )
INCLUDE_DIRECTORIES(
   ${COMMON_INCLUDE_DIR}
   ${MAGICENCODER_INCLUDE_DIR}
)

ADD_SOURCE( MagicEncoderWav       cpp hpp moc )
ADD_SOURCE( MagicEncoderWavConfig cpp hpp moc )

FILE( GLOB MagicEncoderWav_TS_FILES
   ${TRANSLATIONS_SOURCE_DIR}/MagicEncoderWav_*.ts
)

QT4_ADD_TRANSLATION( MagicEncoderWav_QM_FILES
   ${MagicEncoderWav_TS_FILES}
)

QT4_WRAP_CPP( MagicEncoderWav_MOC_GENERATED
   ${MagicEncoderWav_MOC_HEADERS}
)

ADD_LIBRARY( MagicEncoderWav MODULE
   ${MagicEncoderWav_MOC_GENERATED}
   ${MagicEncoderWav_HEADERS}
   ${MagicEncoderWav_SOURCES}
   ${MagicEncoderWav_QM_FILES}
)

TARGET_LINK_LIBRARIES( MagicEncoderWav
   ${MAGICENCODER_LIBRARY_FLAGS}
   ${COMMON_LIBRARY_FLAGS}
   ${QT_LIBRARIES}
)

ADD_DEPENDENCIES( MagicEncoderWav
   Common
   MagicEncoder
)

INSTALL( TARGETS MagicEncoderWav
   RUNTIME DESTINATION bin
   LIBRARY DESTINATION lib
)
