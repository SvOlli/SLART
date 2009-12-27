######################################################################
# src/apps/Partyman/Partyman.pro
# written by Sven Oliver Moll
# distributed as public domain
######################################################################

TEMPLATE = app
TARGET = Partyman
QT += sql network

include( ../../config.pri )

INCLUDEPATH += ../../libs/Common
LIBS += -lCommon

# Input
RESOURCES = Resources/Partyman.qrc
RC_FILE = Resources/WinIcon.rc

SOURCES += Application.cpp 

#HEADERS += MainWindow.hpp
#SOURCES += MainWindow.cpp

#HEADERS += WidgetShot.hpp
#SOURCES += WidgetShot.cpp

HEADERS += MainWidget.hpp
SOURCES += MainWidget.cpp

#HEADERS += MySettings.hpp
#SOURCES += MySettings.cpp

#HEADERS += Database.hpp
#SOURCES += Database.cpp

#HEADERS += TrackInfo.hpp
#SOURCES += TrackInfo.cpp

#HEADERS += SLARTCom.hpp
#SOURCES += SLARTCom.cpp

HEADERS += ControlWidget.hpp
SOURCES += ControlWidget.cpp

#HEADERS += AboutWidget.hpp
#SOURCES += AboutWidget.cpp

HEADERS += ConfigDialog.hpp
SOURCES += ConfigDialog.cpp

#HEADERS += GlobalConfigWidget.hpp
#SOURCES += GlobalConfigWidget.cpp

HEADERS += PlaylistWidget.hpp
SOURCES += PlaylistWidget.cpp

HEADERS += PlaylistContentWidget.hpp
SOURCES += PlaylistContentWidget.cpp

HEADERS += PlayerWidget.hpp
SOURCES += PlayerWidget.cpp

HEADERS += FileSysTreeItem.hpp
SOURCES += FileSysTreeItem.cpp

HEADERS += FileSysTreeModel.hpp
SOURCES += FileSysTreeModel.cpp

HEADERS += FileSysTreeUpdate.hpp
SOURCES += FileSysTreeUpdate.cpp

HEADERS += FileSysTreeView.hpp
SOURCES += FileSysTreeView.cpp

#HEADERS += ScrollLine.hpp
#SOURCES += ScrollLine.cpp

HEADERS += SearchWidget.hpp
SOURCES += SearchWidget.cpp

#HEADERS += TrackInfoWidget.hpp
#SOURCES += TrackInfoWidget.cpp

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

