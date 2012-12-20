#
# src/apps/Partyman/CMakeLists.txt
#
# written by SvOlli
# distributed as public domain
#

MESSAGE( STATUS "==== src/apps/Partyman ====" )

PROJECT( Partyman )

SET( QT_USE_QTNETWORK 1 )
SET( QT_USE_QTSQL 1 )
INCLUDE( ${QT_USE_FILE} )
ADD_DEFINITIONS( ${QT_DEFINITIONS} )
INCLUDE_DIRECTORIES(
   ${SATELLITE_INCLUDE_DIR}
   ${COMMON_INCLUDE_DIR}
)

SET( Partyman_HEADERS
   CommandLineHandler.hpp
   ControlWidget.hpp
   PartymanConfigDialog.hpp
   PartymanMainWindow.hpp
   PlaylistContentWidget.hpp
   PlayerWidget.hpp
   TimeSlider.hpp
   FileSysTreeItem.hpp
   FileSysTreeModel.hpp
   FileSysTreeUpdate.hpp
   FileSysTreeView.hpp
   SearchLineEdit.hpp
   SearchTrackWidget.hpp
   TrackInfoListModel.hpp
   PlayerFSM.hpp
   PlayerFSMBase.hpp
   PlayerFSMDisconnected.hpp
   PlayerFSMEnding.hpp
   PlayerFSMEndingPaused.hpp
   PlayerFSMLoading.hpp
   PlayerFSMPaused.hpp
   PlayerFSMPlaying.hpp
   PlayerFSMReady.hpp
   PlayerFSMSearching.hpp
)

SET( Partyman_SOURCES
   PartymanApplication.cpp
   CommandLineHandler.cpp
   ControlWidget.cpp
   PartymanConfigDialog.cpp
   PartymanMainWindow.cpp
   PlaylistContentWidget.cpp
   PlayerWidget.cpp
   TimeSlider.cpp
   FileSysTreeItem.cpp
   FileSysTreeModel.cpp
   FileSysTreeUpdate.cpp
   FileSysTreeView.cpp
   SearchLineEdit.cpp
   SearchTrackWidget.cpp
   TrackInfoListModel.cpp
   PlayerFSM.cpp
   PlayerFSMBase.cpp
   PlayerFSMDisconnected.cpp
   PlayerFSMEnding.cpp
   PlayerFSMEndingPaused.cpp
   PlayerFSMLoading.cpp
   PlayerFSMPaused.cpp
   PlayerFSMPlaying.cpp
   PlayerFSMReady.cpp
   PlayerFSMSearching.cpp
)

SET( Partyman_RESOURCES
   Resources/Partyman.qrc
)

QT4_ADD_RESOURCES( Partyman_RESOURCES_RCC
   ${Partyman_RESOURCES}
)

FILE( GLOB Partyman_TS_FILES
   ${TRANSLATIONS_SOURCE_DIR}/Partyman_*.ts
)

QT4_WRAP_CPP( Partyman_MOC_HEADERS
   ${Partyman_HEADERS}
)

QT4_ADD_TRANSLATION( Partyman_QM_FILES
   ${Partyman_TS_FILES}
)

ADD_EXECUTABLE( Partyman
   ${Partyman_MOC_HEADERS}
   ${Partyman_HEADERS}
   ${Partyman_SOURCES}
   ${Partyman_QM_FILES}
   ${Partyman_RESOURCES_RCC}
)

ADD_DEPENDENCIES( Partyman
   Satellite
   Common
)

TARGET_LINK_LIBRARIES( Partyman
   ${COMMON_LIBRARY_FLAGS}
   ${SATELLITE_LIBRARY_FLAGS}
   ${QT_LIBRARIES}
)

INSTALL( TARGETS Partyman
   RUNTIME DESTINATION bin
   LIBRARY DESTINATION lib
)
