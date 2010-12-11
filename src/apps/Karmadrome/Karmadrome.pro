######################################################################
# src/apps/Karmadrome/Karmadrome.pro
# written by Sven Oliver Moll
# distributed as public domain
######################################################################

TEMPLATE = app
TARGET = Karmadrome
QT += sql network

include( ../../buildconfig.pri )

INCLUDEPATH += $${TOPSRC}/libs/Common
LIBS += -lCommon

# Input
RESOURCES = ../../libs/Common/Resources/Common.qrc
RC_FILE = ../../libs/Common/Resources/WinIcon.rc

SOURCES += Application.cpp

HEADERS += ButtonsWidget.hpp
SOURCES += ButtonsWidget.cpp

HEADERS += ConfigDialog.hpp
SOURCES += ConfigDialog.cpp

HEADERS += ExportFolder.hpp
SOURCES += ExportFolder.cpp

HEADERS += ExportFolderList.hpp
SOURCES += ExportFolderList.cpp

HEADERS += ImportFolder.hpp
SOURCES += ImportFolder.cpp

HEADERS += MainWidget.hpp
SOURCES += MainWidget.cpp

