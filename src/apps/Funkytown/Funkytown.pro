######################################################################
# src/apps/Funkytown/Funkytown.pro
# written by Sven Oliver Moll
# distributed as public domain
######################################################################

TARGET = Funkytown
TEMPLATE = app
QT += network

include( ../../config.pri )

INCLUDEPATH += ../../libs/Common
LIBS += -lCommon

# Input
RESOURCES = ../../libs/Common/Resources/Common.qrc
RESOURCES += Resources/Funkytown.qrc
RC_FILE = ../Common/Resources/WinIcon.rc

SOURCES += Application.cpp

#HEADERS += Database.hpp

#HEADERS += MainWindow.hpp
#SOURCES += MainWindow.cpp

HEADERS += MainWidget.hpp
SOURCES += MainWidget.cpp

#HEADERS += MySettings.hpp
#SOURCES += MySettings.cpp

#HEADERS += AboutWidget.hpp
#SOURCES += AboutWidget.cpp

HEADERS += ConfigDialog.hpp
SOURCES += ConfigDialog.cpp

HEADERS += Cookie.hpp
SOURCES += Cookie.cpp

HEADERS += CookieJar.hpp
SOURCES += CookieJar.cpp

HEADERS += DownloadHandler.hpp
SOURCES += DownloadHandler.cpp

#HEADERS += GlobalConfigWidget.hpp
#SOURCES += GlobalConfigWidget.cpp

HEADERS += MagicQueue.hpp
SOURCES += MagicQueue.cpp

#HEADERS += ProxyWidget.hpp
#SOURCES += ProxyWidget.cpp

#HEADERS += ScrollLine.hpp
#SOURCES += ScrollLine.cpp

HEADERS += TheMagic.hpp
SOURCES += TheMagic.cpp

