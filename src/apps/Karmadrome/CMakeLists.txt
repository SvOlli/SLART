#
# src/apps/Karmadrome/CMakeLists.txt
#
# written by SvOlli
# distributed as public domain
#

MESSAGE( STATUS "==== src/apps/Karmadrome ====" )

PROJECT( Karmadrome )

SET( QT_USE_QTNETWORK 1 )
SET( QT_USE_QTSQL 1 )
INCLUDE( ${QT_USE_FILE} )
ADD_DEFINITIONS( ${QT_DEFINITIONS} )
INCLUDE_DIRECTORIES(
   ${SATELLITE_INCLUDE_DIR}
   ${COMMON_INCLUDE_DIR}
)

SET( Karmadrome_HEADERS
   ButtonsWidget.hpp
   ExportFolder.hpp
   ExportFolderList.hpp
   ImportFolder.hpp
   KarmadromeConfigDialog.hpp
   KarmadromeMainWidget.hpp
)

SET( Karmadrome_SOURCES
   ButtonsWidget.cpp
   ExportFolder.cpp
   ExportFolderList.cpp
   ImportFolder.cpp
   KarmadromeApplication.cpp
   KarmadromeConfigDialog.cpp
   KarmadromeMainWidget.cpp
)

SET( Karmadrome_RESOURCES
)

FILE( GLOB Karmadrome_TS_FILES
   ${TRANSLATIONS_SOURCE_DIR}/Karmadrome_*.ts
)

QT4_WRAP_CPP( Karmadrome_MOC_HEADERS
   ${Karmadrome_HEADERS}
)

QT4_ADD_RESOURCES( Karmadrome_RESOURCES_RCC
   ${Karmadrome_RESOURCES}
)

QT4_ADD_TRANSLATION( Karmadrome_QM_FILES
   ${Karmadrome_TS_FILES}
)

ADD_EXECUTABLE( Karmadrome
   ${Karmadrome_MOC_HEADERS}
   ${Karmadrome_HEADERS}
   ${Karmadrome_SOURCES}
   ${Karmadrome_QM_FILES}
   ${Karmadrome_RESOURCES_RCC}
)

ADD_DEPENDENCIES( Karmadrome
   Satellite
   Common
)

TARGET_LINK_LIBRARIES( Karmadrome
   ${COMMON_LIBRARY_FLAGS}
   ${SATELLITE_LIBRARY_FLAGS}
   ${QT_LIBRARIES}
)

INSTALL( TARGETS Karmadrome
   RUNTIME DESTINATION bin
   LIBRARY DESTINATION lib
)
