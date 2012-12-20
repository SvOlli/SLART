#
# src/libs/Magic/MagicEncoder/CMakeLists.txt
#
# written by SvOlli
# distributed as public domain
#

MESSAGE( STATUS "==== src/libs/Magic/MagicEncoder ====" )

PROJECT( MagicEncoder )

# Satellite is used
#SET( QT_USE_QTNETWORK 1 )
INCLUDE( ${QT_USE_FILE} )
ADD_DEFINITIONS( ${QT_DEFINITIONS} )
ADD_DEFINITIONS( -fPIC )

INCLUDE_DIRECTORIES(
   ${MAGICINTERFACE_INCLUDE_DIR}
   ${COMMON_INCLUDE_DIR}
)

SET( MagicEncoder_PUBLIC_HEADERS
   MagicEncoder.hpp
   MagicEncoderConfig.hpp
)

SET( MagicEncoder_PRIVATE_HEADERS
)

SET( MagicEncoder_SOURCES
   MagicEncoder.cpp
   MagicEncoderConfig.cpp
)

FILE( GLOB MagicEncoder_TS_FILES
   ${TRANSLATIONS_SOURCE_DIR}/MagicEncoder_*.ts
)

QT4_WRAP_CPP( MagicEncoder_MOC_HEADERS
   ${MagicEncoder_PUBLIC_HEADERS}
   ${MagicEncoder_PRIVATE_HEADERS}
)

ADD_LIBRARY( MagicEncoder STATIC
   ${MagicEncoder_MOC_HEADERS}
   ${MagicEncoder_PUBLIC_HEADERS}
   ${MagicEncoder_PRIVATE_HEADERS}
   ${MagicEncoder_SOURCES}
)

PROPAGATE_LIBRARY( MagicEncoder )
