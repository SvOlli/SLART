######################################################################
# src/tools/SettingsGenerator/SettingsGenerator.pro
# hand hacked by SvOlli
# released as public domain
######################################################################

TARGET = SettingsGenerator
TEMPLATE = app
QT -= gui

include( ../../buildconfig.pri )

# Input
SOURCES += Application.cpp

HEADERS += SettingsGeneratorHandler.hpp
SOURCES += SettingsGeneratorHandler.cpp

HEADERS += SettingsGeneratorStorage.hpp
SOURCES += SettingsGeneratorStorage.cpp
