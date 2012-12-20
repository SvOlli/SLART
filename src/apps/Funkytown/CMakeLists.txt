#
# src/apps/Funkytown/CMakeLists.txt
#
# written by SvOlli
# distributed as public domain
#

MESSAGE( STATUS "==== src/apps/Funkytown ====" )

PROJECT( Funkytown )

SET( QT_USE_QTNETWORK 1 )
INCLUDE( ${QT_USE_FILE} )
ADD_DEFINITIONS( ${QT_DEFINITIONS} )
INCLUDE_DIRECTORIES(
   ${SATELLITE_INCLUDE_DIR}
   ${COMMON_INCLUDE_DIR}
)

SET( Funkytown_HEADERS
   ConfigDialog.hpp
   Cookie.hpp
   CookieJar.hpp
   DownloadHandler.hpp
   FunkytownMainWidget.hpp
   MagicQueue.hpp
   TheMagic.hpp
)

SET( Funkytown_SOURCES
   Application.cpp
   ConfigDialog.cpp
   Cookie.cpp
   CookieJar.cpp
   DownloadHandler.cpp
   FunkytownMainWidget.cpp
   MagicQueue.cpp
   TheMagic.cpp
)

SET( Funkytown_RESOURCES
   Resources/Funkytown.qrc
)

FILE( GLOB Funkytown_TS_FILES
   ${TRANSLATIONS_SOURCE_DIR}/Funkytown_*.ts
)

QT4_WRAP_CPP( Funkytown_MOC_HEADERS
   ${Funkytown_HEADERS}
)

QT4_ADD_RESOURCES( Funkytown_RESOURCES_RCC
   ${Funkytown_RESOURCES}
)

QT4_ADD_TRANSLATION( Funkytown_QM_FILES
   ${Funkytown_TS_FILES}
)

ADD_EXECUTABLE( Funkytown
   ${Funkytown_MOC_HEADERS}
   ${Funkytown_HEADERS}
   ${Funkytown_SOURCES}
   ${Funkytown_QM_FILES}
   ${Funkytown_RESOURCES_RCC}
)

ADD_DEPENDENCIES( Funkytown
   Satellite
   Common
)

TARGET_LINK_LIBRARIES( Funkytown
   ${COMMON_LIBRARY_FLAGS}
   ${SATELLITE_LIBRARY_FLAGS}
   ${QT_LIBRARIES}
)

INSTALL( TARGETS Funkytown
   RUNTIME DESTINATION bin
   LIBRARY DESTINATION lib
)
