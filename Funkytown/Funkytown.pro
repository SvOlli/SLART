######################################################################
# hand hacked by SvOlli
######################################################################

include( ../Global.pri )

TEMPLATE = app
TARGET = Funkytown

# Input
RESOURCES = Resources/Common.qrc
RESOURCES += Resources/Funkytown.qrc

SOURCES += Application.cpp

HEADERS += Database.hpp

HEADERS += MainWindow.hpp
SOURCES += MainWindow.cpp

HEADERS += MainWidget.hpp
SOURCES += MainWidget.cpp

HEADERS += MySettings.hpp
SOURCES += MySettings.cpp

HEADERS += AboutWidget.hpp
SOURCES += AboutWidget.cpp

HEADERS += ConfigDialog.hpp
SOURCES += ConfigDialog.cpp

HEADERS += Cookie.hpp
SOURCES += Cookie.cpp

HEADERS += CookieJar.hpp
SOURCES += CookieJar.cpp

HEADERS += DownloadHandler.hpp
SOURCES += DownloadHandler.cpp

HEADERS += GlobalConfigWidget.hpp
SOURCES += GlobalConfigWidget.cpp

HEADERS += MagicQueue.hpp
SOURCES += MagicQueue.cpp

HEADERS += ProxyWidget.hpp
SOURCES += ProxyWidget.cpp

HEADERS += ScrollLine.hpp
SOURCES += ScrollLine.cpp

HEADERS += TheMagic.hpp
SOURCES += TheMagic.cpp

