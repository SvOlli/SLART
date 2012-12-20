#
# src/apps/Sorcerer/CMakeLists.txt
#
# written by SvOlli
# distributed as public domain
#

MESSAGE( STATUS "==== src/apps/Sorcerer ====" )

SET( QT_USE_QTNETWORK 1 )
SET( QT_USE_QTSQL 1 )
INCLUDE( ${QT_USE_FILE} )
ADD_DEFINITIONS( ${QT_DEFINITIONS} )
INCLUDE_DIRECTORIES(
   ${SATELLITE_INCLUDE_DIR}
   ${COMMON_INCLUDE_DIR}
   ${TAGLIB_INCLUDE_DIR}
)

SET( Sorcerer_HEADERS
   SorcererMainWidget.hpp
   ../Innuendo/SatelliteConfigWidget.hpp
   ../Rubberbandman/DatabaseWidget.hpp
   ../Rubberbandman/DatabaseWorker.hpp
   ../Rubberbandman/DirWalkerUpdate.hpp
)

SET( Sorcerer_SOURCES
   SorcererApplication.cpp
   SorcererMainWidget.cpp
   ../Innuendo/SatelliteConfigWidget.cpp
   ../Rubberbandman/DatabaseWidget.cpp
   ../Rubberbandman/DatabaseWorker.cpp
   ../Rubberbandman/DirWalkerUpdate.cpp
)

SET( Sorcerer_RESOURCES
)

FILE( GLOB Sorcerer_TS_FILES
   ${TRANSLATIONS_SOURCE_DIR}/Sorcerer_*.ts
)

QT4_WRAP_CPP( Sorcerer_MOC_HEADERS
   ${Sorcerer_HEADERS}
)

QT4_ADD_TRANSLATION( Sorcerer_QM_FILES
   ${Sorcerer_TS_FILES}
)

QT4_ADD_RESOURCES( Sorcerer_RESOURCES_RCC
   ${Sorcerer_RESOURCES}
)

ADD_EXECUTABLE( Sorcerer
   ${Sorcerer_MOC_HEADERS}
   ${Sorcerer_HEADERS}
   ${Sorcerer_SOURCES}
   ${Sorcerer_RESOURCES_RCC}
   ${Sorcerer_QM_FILES}
   ${Sorcerer_RESOURCES_RCC}
)

ADD_DEPENDENCIES( Sorcerer
   Satellite
   Common
   TagLib
)

TARGET_LINK_LIBRARIES( Sorcerer
   ${COMMON_LIBRARY_FLAGS}
   ${SATELLITE_LIBRARY_FLAGS}
   ${TAGLIB_LIBRARY_FLAGS}
   ${QT_LIBRARIES}
)

INSTALL( TARGETS Sorcerer
   RUNTIME DESTINATION bin
   LIBRARY DESTINATION lib
)
