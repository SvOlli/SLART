######################################################################
# hand hacked by SvOlli
######################################################################

include ( ../Global.pri )

TEMPLATE = app
TARGET = Notorious
QT += sql
LIBS += -lbz2

# Input
RESOURCES = Resources/Common.qrc
RC_FILE = ../Common/Resources/WinIcon.rc

SOURCES += Application.cpp

HEADERS += Database.hpp

HEADERS += AboutWidget.hpp
SOURCES += AboutWidget.cpp

HEADERS += ConfigDialog.hpp
SOURCES += ConfigDialog.cpp

HEADERS += Foundlist.hpp
SOURCES += Foundlist.cpp

HEADERS += FreeDB.hpp
SOURCES += FreeDB.cpp

HEADERS += FreeDBEntry.hpp
SOURCES += FreeDBEntry.cpp

HEADERS += FreeDBImport.hpp
SOURCES += FreeDBImport.cpp

HEADERS += FreeDBQuery.hpp
SOURCES += FreeDBQuery.cpp

HEADERS += MainWidget.hpp
SOURCES += MainWidget.cpp

HEADERS += MainWindow.hpp
SOURCES += MainWindow.cpp

HEADERS += MySettings.hpp
SOURCES += MySettings.cpp

HEADERS += SearchWidget.hpp
SOURCES += SearchWidget.cpp

HEADERS += TarEntry.hpp
SOURCES += TarEntry.cpp

HEADERS += tir.h
SOURCES += tir.c
