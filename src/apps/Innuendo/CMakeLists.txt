#
# src/apps/Innuendo/CMakeLists.txt
#
# written by SvOlli
# distributed as public domain
#

MESSAGE( STATUS "==== src/apps/Innuendo ====" )

PROJECT( Innuendo )

SET( QT_USE_QTNETWORK 1 )
INCLUDE( ${QT_USE_FILE} )
ADD_DEFINITIONS( ${QT_DEFINITIONS} )
INCLUDE_DIRECTORIES(
   ${SATELLITE_INCLUDE_DIR}
   ${COMMON_INCLUDE_DIR}
)

SET( Innuendo_HEADERS
   ConfigDialog.hpp
   DropDialog.hpp
   ExecButton.hpp
   InnuendoMainWidget.hpp
   LogListWidget.hpp
   SatelliteConfigWidget.hpp
)

SET( Innuendo_SOURCES
   Application.cpp
   ConfigDialog.cpp
   DropDialog.cpp
   ExecButton.cpp
   InnuendoMainWidget.cpp
   LogListWidget.cpp
   SatelliteConfigWidget.cpp
)

SET( Innuendo_RESOURCES
)

FILE( GLOB Innuendo_TS_FILES
   ${TRANSLATIONS_SOURCE_DIR}/Innuendo_*.ts
)

QT4_WRAP_CPP( Innuendo_MOC_HEADERS
   ${Innuendo_HEADERS}
)

QT4_ADD_RESOURCES( Innuendo_RESOURCES_RCC
)

QT4_ADD_TRANSLATION( Innuendo_QM_FILES
   ${Innuendo_TS_FILES}
)

ADD_EXECUTABLE( Innuendo
   ${Innuendo_MOC_HEADERS}
   ${Innuendo_HEADERS}
   ${Innuendo_SOURCES}
   ${Innuendo_QM_FILES}
   ${Innuendo_RESOURCES_RCC}
)

ADD_DEPENDENCIES( Innuendo
   Satellite
   Common
)

TARGET_LINK_LIBRARIES( Innuendo
   ${COMMON_LIBRARY_FLAGS}
   ${SATELLITE_LIBRARY_FLAGS}
   ${QT_LIBRARIES}
)

INSTALL( TARGETS Innuendo
   RUNTIME DESTINATION bin
   LIBRARY DESTINATION lib
)
