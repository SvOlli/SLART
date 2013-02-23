######################################################################
# src/libs/libs.pro
# hand hacked by SvOlli
# distributed as public domain
######################################################################

TEMPLATE = subdirs

SUBDIRS += CommandLine
SUBDIRS += Common
SUBDIRS += Kryptonite
SUBDIRS += Magic
SUBDIRS += Satellite
SUBDIRS += TagLib

Magic.depends = Common TagLib
Common.depends = Satellite

