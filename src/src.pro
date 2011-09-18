######################################################################
# src/src.pro
# hand hacked by SvOlli
# distributed as public domain
######################################################################

TEMPLATE = subdirs

SUBDIRS += libs
SUBDIRS += apps
SUBDIRS += tests
debug:SUBDIRS += tools

apps.depends  = libs
tests.depends = libs
libs.depends  = tools

