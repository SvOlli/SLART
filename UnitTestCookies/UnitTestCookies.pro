######################################################################
# hand hacked by SvOlli
######################################################################

include ( ../Global.pri )

TEMPLATE = app
TARGET = UnitTestCookies
DESTDIR = ../test

Qt -= gui

# Input
#RESOURCES = Resources.qrc

SOURCES += Application.cpp 

HEADERS += UnitTest.hpp
SOURCES += UnitTest.cpp

HEADERS += ../Funkytown/Cookie.hpp
SOURCES += ../Funkytown/Cookie.cpp

HEADERS += ../Funkytown/CookieJar.hpp
SOURCES += ../Funkytown/CookieJar.cpp

