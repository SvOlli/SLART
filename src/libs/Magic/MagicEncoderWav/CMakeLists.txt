#
# src/libs/Magic/MagicEncoderWav/CMakeLists.txt
#
# written by SvOlli
# distributed as public domain
#

MESSAGE( STATUS "==== src/libs/Magic/MagicEncoderWav ====" )

PROJECT( MagicEncoderWav )

INCLUDE( ${QT_USE_FILE} )
ADD_DEFINITIONS( ${QT_DEFINITIONS} )


INCLUDE_DIRECTORIES(
   ${COMMON_INCLUDE_DIR}
   ${MAGICENCODER_INCLUDE_DIR}
)

FILE( GLOB MagicEncoderWav_TS_FILES
   ${TRANSLATIONS_SOURCE_DIR}/MagicEncoderWav_*.ts
)

QT4_ADD_TRANSLATION( MagicEncoderWav_QM_FILES
   ${MagicEncoderWav_TS_FILES}
)

SET( MagicEncoderWav_PUBLIC_HEADERS
   MagicEncoderWav.hpp
   MagicEncoderWavConfig.hpp
)

SET( MagicEncoderWav_PRIVATE_HEADERS
)

SET( MagicEncoderWav_SOURCES
   MagicEncoderWav.cpp
   MagicEncoderWavConfig.cpp
)

QT4_WRAP_CPP( MagicEncoderWav_MOC_HEADERS
   ${MagicEncoderWav_PUBLIC_HEADERS}
   ${MagicEncoderWav_PRIVATE_HEADERS}
)

ADD_LIBRARY( MagicEncoderWav MODULE
   ${MagicEncoderWav_MOC_HEADERS}
   ${MagicEncoderWav_PUBLIC_HEADERS}
   ${MagicEncoderWav_PRIVATE_HEADERS}
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
