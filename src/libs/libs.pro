######################################################################
# src/libs/libs.pro
# hand hacked by SvOlli
# distributed as public domain
######################################################################

TEMPLATE = subdirs

SUBDIRS += CommandLine
SUBDIRS += Common
SUBDIRS += Magic
SUBDIRS += TagLib

Magic.depends = Common TagLib
Sorcerer.depends = Common TagLib

