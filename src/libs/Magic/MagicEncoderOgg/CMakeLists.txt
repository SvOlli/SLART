#
# src/libs/Magic/MagicEncoderOgg/CMakeLists.txt
#
# written by SvOlli
# distributed as public domain
#

MESSAGE( STATUS "==== src/libs/Magic/MagicEncoderOgg ====" )

PROJECT( MagicEncoderOgg )

INCLUDE( ${QT_USE_FILE} )
ADD_DEFINITIONS( ${QT_DEFINITIONS} )

INCLUDE( FindPkgConfig )
PKG_CHECK_MODULES( VORBISFILE REQUIRED vorbisfile )

INCLUDE_DIRECTORIES(
   ${COMMON_INCLUDE_DIR}
   ${MAGICENCODER_INCLUDE_DIR}
   ${VORBISFILE_INCLUDE_DIR}
)

FILE( GLOB MagicEncoderOgg_TS_FILES
   ${TRANSLATIONS_SOURCE_DIR}/MagicEncoderOgg_*.ts
)

QT4_ADD_TRANSLATION( MagicEncoderOgg_QM_FILES
   ${MagicEncoderOgg_TS_FILES}
)

SET( MagicEncoderOgg_PUBLIC_HEADERS
   MagicEncoderOgg.hpp
   MagicEncoderOggConfig.hpp
)

SET( MagicEncoderOgg_PRIVATE_HEADERS
)

SET( MagicEncoderOgg_SOURCES
   MagicEncoderOgg.cpp
   MagicEncoderOggConfig.cpp
)

QT4_WRAP_CPP( MagicEncoderOgg_MOC_HEADERS
   ${MagicEncoderOgg_PUBLIC_HEADERS}
   ${MagicEncoderOgg_PRIVATE_HEADERS}
)

ADD_LIBRARY( MagicEncoderOgg MODULE
   ${MagicEncoderOgg_MOC_HEADERS}
   ${MagicEncoderOgg_PUBLIC_HEADERS}
   ${MagicEncoderOgg_PRIVATE_HEADERS}
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
