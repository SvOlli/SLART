######################################################################
# hand hacked by SvOlli
######################################################################

include( ../Global.pri )

TEMPLATE = app
TARGET = 

INCLUDEPATH += ../taglib
LIBS += -L../taglib -ltag

# Input
#RESOURCES = Resources.qrc

SOURCES += Application.cpp 

HEADERS += BrowseWidget.hpp 
SOURCES += BrowseWidget.cpp 

HEADERS += FileSysBrowser.hpp 
SOURCES += FileSysBrowser.cpp 

HEADERS += FileSysTreeItem.hpp 
SOURCES += FileSysTreeItem.cpp 

HEADERS += FileSysTreeModel.hpp 
SOURCES += FileSysTreeModel.cpp 

HEADERS += InfoEdit.hpp 
SOURCES += InfoEdit.cpp 

HEADERS += MainWindow.hpp 
SOURCES += MainWindow.cpp 

