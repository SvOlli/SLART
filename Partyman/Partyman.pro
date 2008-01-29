######################################################################
# hand hacked by SvOlli
######################################################################

include( ../Global.pri )

TEMPLATE = app
TARGET = Partyman

# Input
RESOURCES = qrc/Resources.qrc

SOURCES += Application.cpp 

HEADERS += MainWindow.hpp 
SOURCES += MainWindow.cpp 

HEADERS += MainWidget.hpp 
SOURCES += MainWidget.cpp 

HEADERS += MySettings.hpp 
SOURCES += MySettings.cpp 

HEADERS += ControlWidget.hpp 
SOURCES += ControlWidget.cpp 

HEADERS += AboutWidget.hpp 
SOURCES += AboutWidget.cpp 

HEADERS += ConfigDialog.hpp 
SOURCES += ConfigDialog.cpp 

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

HEADERS += FileSysTreeView.hpp 
SOURCES += FileSysTreeView.cpp 

HEADERS += ScrollLine.hpp
SOURCES += ScrollLine.cpp

HEADERS += SearchWidget.hpp
SOURCES += SearchWidget.cpp

