######################################################################
# src/src.pro
# hand hacked by SvOlli
# distributed as public domain
######################################################################

message(CONFIG = $${CONFIG})

TEMPLATE = subdirs

SUBDIRS += libs
SUBDIRS += apps
debug:SUBDIRS += tests
debug:SUBDIRS += tools

apps.depends  = libs
debug:tests.depends = libs
debug:libs.depends  = tools

DISTFILES += ../debian/control
DISTFILES += ../debian/rules
DISTFILES += ../extra/install-dev-packages.sh
DISTFILES += ../Makefile
