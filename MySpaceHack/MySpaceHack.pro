######################################################################
# hand hacked by SvOlli
######################################################################

include( ../Global.pri )

TEMPLATE = app
TARGET = MySpaceHack
DEPENDPATH += ../Funkytown
INCLUDEPATH += ../Funkytown

DEFINES += STANDALONE

# Input
SOURCES += Application.cpp

HEADERS += MainWindow.hpp
SOURCES += MainWindow.cpp

HEADERS += MainWidget.hpp
SOURCES += MainWidget.cpp

HEADERS += MySettings.hpp
SOURCES += MySettings.cpp

HEADERS += DownloadHandler.hpp
SOURCES += DownloadHandler.cpp

HEADERS += PostDownloadHandler.hpp
SOURCES += PostDownloadHandler.cpp

HEADERS += PostDownloadHandlerHTML.hpp
SOURCES += PostDownloadHandlerHTML.cpp

HEADERS += PostDownloadHandlerXML.hpp
SOURCES += PostDownloadHandlerXML.cpp

HEADERS += PostDownloadHandlerMP3.hpp
SOURCES += PostDownloadHandlerMP3.cpp

HEADERS += SetupDialog.hpp
SOURCES += SetupDialog.cpp

HEADERS += ProxyWidget.hpp
SOURCES += ProxyWidget.cpp

