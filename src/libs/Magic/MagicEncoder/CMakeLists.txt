#
# src/libs/Magic/MagicEncoder/CMakeLists.txt
#
# written by SvOlli
# distributed as public domain
#

MESSAGE( STATUS "==== src/libs/Magic/MagicEncoder ====" )

PROJECT( MagicEncoder )

INCLUDE( LocalTools )

# Satellite is used
#SET( QT_USE_QTNETWORK 1 )
INCLUDE( ${QT_USE_FILE} )
ADD_DEFINITIONS( ${QT_DEFINITIONS} )
ADD_DEFINITIONS( -fPIC )
INCLUDE_DIRECTORIES(
   ${MAGICINTERFACE_INCLUDE_DIR}
   ${COMMON_INCLUDE_DIR}
)

ADD_SOURCE( MagicEncoder       cpp hpp moc )
ADD_SOURCE( MagicEncoderConfig cpp hpp moc )

FILE( GLOB MagicEncoder_TS_FILES
   ${TRANSLATIONS_SOURCE_DIR}/MagicEncoder_*.ts
)

QT4_WRAP_CPP( MagicEncoder_MOC_GENERATED
   ${MagicEncoder_MOC_HEADERS}
)

ADD_LIBRARY( MagicEncoder STATIC
   ${MagicEncoder_MOC_GENERATED}
   ${MagicEncoder_HEADERS}
   ${MagicEncoder_SOURCES}
)

PROPAGATE_LIBRARY( MagicEncoder )
