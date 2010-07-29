######################################################################
# src/apps/Partyman/Partyman.pro
# written by Sven Oliver Moll
# distributed as public domain
######################################################################

TEMPLATE = app
TARGET = Partyman
QT += sql network

include( ../../buildconfig.pri )

INCLUDEPATH += ../../libs/Common
LIBS += -lCommon

# Input
RESOURCES = Resources/Partyman.qrc
RC_FILE = Resources/WinIcon.rc

SOURCES += Application.cpp 

HEADERS += MainWidget.hpp
SOURCES += MainWidget.cpp

HEADERS += ControlWidget.hpp
SOURCES += ControlWidget.cpp

HEADERS += ConfigDialog.hpp
SOURCES += ConfigDialog.cpp

HEADERS += PlaylistControlWidget.hpp
SOURCES += PlaylistControlWidget.cpp

HEADERS += PlaylistContentWidget.hpp
SOURCES += PlaylistContentWidget.cpp

HEADERS += PlayerWidget.hpp
SOURCES += PlayerWidget.cpp

HEADERS += TimeSlider.hpp
SOURCES += TimeSlider.cpp

HEADERS += MyMouseEvent.hpp
SOURCES += MyMouseEvent.cpp

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

HEADERS += SearchWidget.hpp
SOURCES += SearchWidget.cpp

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

