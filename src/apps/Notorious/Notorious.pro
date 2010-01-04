######################################################################
# src/apps/Notorious/Notorious.pro
# written by Sven Oliver Moll
# distributed as public domain
######################################################################

TEMPLATE = app
TARGET = Notorious
QT += sql network
LIBS += -lbz2

include( ../../config.pri )

INCLUDEPATH += ../../libs/Common
LIBS += -lCommon

# Input
RESOURCES = ../../libs/Common/Resources/Common.qrc
RC_FILE = ../../libs/Common/Resources/WinIcon.rc

SOURCES += Application.cpp

#HEADERS += Database.hpp

#HEADERS += AboutWidget.hpp
#SOURCES += AboutWidget.cpp

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

#HEADERS += MainWindow.hpp
#SOURCES += MainWindow.cpp

#HEADERS += MySettings.hpp
#SOURCES += MySettings.cpp

HEADERS += SearchWidget.hpp
SOURCES += SearchWidget.cpp

HEADERS += TarEntry.hpp
SOURCES += TarEntry.cpp

HEADERS += tir.h
SOURCES += tir.c
