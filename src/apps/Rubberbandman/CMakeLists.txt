#
# src/apps/Rubberbandman/CMakeLists.txt
#
# written by SvOlli
# distributed as public domain
#

MESSAGE( STATUS "==== src/apps/Rubberbandman ====" )

PROJECT( Rubberbandman )

SET( QT_USE_QTNETWORK 1 )
SET( QT_USE_QTSQL 1 )
INCLUDE( ${QT_USE_FILE} )
ADD_DEFINITIONS( ${QT_DEFINITIONS} )
INCLUDE_DIRECTORIES(
   ${SATELLITE_INCLUDE_DIR}
   ${COMMON_INCLUDE_DIR}
   ${TAGLIB_INCLUDE_DIR}
)

SET( Rubberbandman_HEADERS
   BrowseWidget.hpp
   ConfigDialog.hpp
   Console.hpp
   DatabaseWidget.hpp
   DatabaseWorker.hpp
   DirWalkerDelete.hpp
   DirWalkerMove.hpp
   DirWalkerUpdate.hpp
   FileSysBrowser.hpp
   InfoEdit.hpp
   LineEdit.hpp
   MyTreeView.hpp
   RecurseWorker.hpp
   RubberbandmanMainWidget.hpp
   SatelliteWidget.hpp
   TrackReader.hpp
   TrackWriter.hpp
)

SET( Rubberbandman_SOURCES
   Application.cpp
   BrowseWidget.cpp
   ConfigDialog.cpp
   Console.cpp
   DatabaseWidget.cpp
   DatabaseWorker.cpp
   DirWalkerDelete.cpp
   DirWalkerMove.cpp
   DirWalkerUpdate.cpp
   FileSysBrowser.cpp
   InfoEdit.cpp
   LineEdit.cpp
   MyTreeView.cpp
   RecurseWorker.cpp
   RubberbandmanMainWidget.cpp
   SatelliteWidget.cpp
   TrackReader.cpp
   TrackWriter.cpp
)

SET( Rubberbandman_RESOURCES
)

FILE( GLOB Rubberbandman_TS_FILES
   ${TRANSLATIONS_SOURCE_DIR}/Rubberbandman_*.ts
   ${TRANSLATIONS_SOURCE_DIR}/Rubberbandman.ts
)

QT4_WRAP_CPP( Rubberbandman_MOC_HEADERS
   ${Rubberbandman_HEADERS}
)

QT4_ADD_RESOURCES( Rubberbandman_RESOURCES_RCC
   ${Rubberbandman_RESOURCES}
)

QT4_ADD_TRANSLATION( Rubberbandman_QM_FILES
   ${Rubberbandman_TS_FILES}
)

ADD_EXECUTABLE( Rubberbandman
   ${Rubberbandman_MOC_HEADERS}
   ${Rubberbandman_HEADERS}
   ${Rubberbandman_SOURCES}
   ${Rubberbandman_QM_FILES}
   ${Rubberbandman_RESOURCES_RCC}
)

ADD_DEPENDENCIES( Rubberbandman
   Satellite
   Common
   TagLib
)

TARGET_LINK_LIBRARIES( Rubberbandman
   ${COMMON_LIBRARY_FLAGS}
   ${SATELLITE_LIBRARY_FLAGS}
   ${TAGLIB_LIBRARY_FLAGS}
   ${QT_LIBRARIES}
)

INSTALL( TARGETS Rubberbandman
   RUNTIME DESTINATION bin
   LIBRARY DESTINATION lib
)
