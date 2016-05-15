######################################################################
# src/apps/Partyman/Partyman.pro
# written by Sven Oliver Moll
# distributed as public domain
######################################################################

TEMPLATE = app
TARGET = Partyman
QT += widgets sql network

include( ../../buildconfig.pri )

INCLUDEPATH += $${TOPSRC}/libs/Common
LIBS += -lCommon
INCLUDEPATH += $${TOPSRC}/libs/Satellite
LIBS += -lSatellite
QMAKE_CXXFLAGS_DEBUG += -DCOREDUMP_SIZE_MB=100

# Input
RESOURCES = $${TOPSRC}/resources/$${TARGET}.qrc
RC_FILE = $${TOPSRC}/resources/windows/$${TARGET}.rc

TRANSLATIONS += $${TOPSRC}/resources/translations/$${TARGET}_de.ts

SOURCES += PartymanApplication.cpp

HEADERS += CommandLineHandler.hpp
SOURCES += CommandLineHandler.cpp

HEADERS += ControlWidget.hpp
SOURCES += ControlWidget.cpp

HEADERS += PartymanConfigDialog.hpp
SOURCES += PartymanConfigDialog.cpp

HEADERS += PartymanMainWindow.hpp
SOURCES += PartymanMainWindow.cpp

HEADERS += PlaylistContentWidget.hpp
SOURCES += PlaylistContentWidget.cpp

HEADERS += PlayerWidget.hpp
SOURCES += PlayerWidget.cpp

HEADERS += TimeSlider.hpp
SOURCES += TimeSlider.cpp

HEADERS += FileSysTreeItem.hpp
SOURCES += FileSysTreeItem.cpp

HEADERS += FileSysTreeModel.hpp
SOURCES += FileSysTreeModel.cpp

HEADERS += FileSysTreeUpdate.hpp
SOURCES += FileSysTreeUpdate.cpp

HEADERS += FileSysTreeView.hpp
SOURCES += FileSysTreeView.cpp

HEADERS += SearchLineEdit.hpp
SOURCES += SearchLineEdit.cpp

HEADERS += SearchTrackWidget.hpp
SOURCES += SearchTrackWidget.cpp

HEADERS += TrackInfoListModel.hpp
SOURCES += TrackInfoListModel.cpp

HEADERS += PlayerFSM.hpp
SOURCES += PlayerFSM.cpp

HEADERS += PlayerFSMBase.hpp
SOURCES += PlayerFSMBase.cpp

HEADERS += PlayerFSMDisconnected.hpp
SOURCES += PlayerFSMDisconnected.cpp

HEADERS += PlayerFSMEnding.hpp
SOURCES += PlayerFSMEnding.cpp

HEADERS += PlayerFSMEndingPaused.hpp
SOURCES += PlayerFSMEndingPaused.cpp

HEADERS += PlayerFSMLoading.hpp
SOURCES += PlayerFSMLoading.cpp

HEADERS += PlayerFSMPaused.hpp
SOURCES += PlayerFSMPaused.cpp

HEADERS += PlayerFSMPlaying.hpp
SOURCES += PlayerFSMPlaying.cpp

HEADERS += PlayerFSMReady.hpp
SOURCES += PlayerFSMReady.cpp

HEADERS += PlayerFSMSearching.hpp
SOURCES += PlayerFSMSearching.cpp
