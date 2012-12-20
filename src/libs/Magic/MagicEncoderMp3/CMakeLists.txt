#
# src/libs/Magic/MagicEncoderMp3/CMakeLists.txt
#
# written by SvOlli
# distributed as public domain
#

MESSAGE( STATUS "==== src/libs/Magic/MagicEncoderMp3 ====" )

PROJECT( MagicEncoderMp3 )

INCLUDE( ${QT_USE_FILE} )
ADD_DEFINITIONS( ${QT_DEFINITIONS} )

INCLUDE( FindLame )
INCLUDE_DIRECTORIES(
   ${COMMON_INCLUDE_DIR}
   ${MAGICENCODER_INCLUDE_DIR}
   ${LAME_INCLUDE_DIRS}
)

FILE( GLOB MagicEncoderMp3_TS_FILES
   ${TRANSLATIONS_SOURCE_DIR}/MagicEncoderMp3_*.ts
)

QT4_ADD_TRANSLATION( MagicEncoderMp3_QM_FILES
   ${MagicEncoderMp3_TS_FILES}
)

SET( MagicEncoderMp3_PUBLIC_HEADERS
   MagicEncoderMp3.hpp
   MagicEncoderMp3Config.hpp
)

SET( MagicEncoderMp3_PRIVATE_HEADERS
)

SET( MagicEncoderMp3_SOURCES
   MagicEncoderMp3.cpp
   MagicEncoderMp3Config.cpp
)

QT4_WRAP_CPP( MagicEncoderMp3_MOC_HEADERS
   ${MagicEncoderMp3_PUBLIC_HEADERS}
   ${MagicEncoderMp3_PRIVATE_HEADERS}
)

ADD_LIBRARY( MagicEncoderMp3 MODULE
   ${MagicEncoderMp3_MOC_HEADERS}
   ${MagicEncoderMp3_PUBLIC_HEADERS}
   ${MagicEncoderMp3_PRIVATE_HEADERS}
   ${MagicEncoderMp3_SOURCES}
   ${MagicEncoderMp3_QM_FILES}
)

TARGET_LINK_LIBRARIES( MagicEncoderMp3
   ${MAGICENCODER_LIBRARY_FLAGS}
   ${COMMON_LIBRARY_FLAGS}
   ${LAME_LIBRARIES}
   ${QT_LIBRARIES}
)

ADD_DEPENDENCIES( MagicEncoderMp3
   Common
   MagicEncoder
)

INSTALL( TARGETS MagicEncoderMp3
   RUNTIME DESTINATION bin
   LIBRARY DESTINATION lib
)
