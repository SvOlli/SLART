######################################################################
# src/libs/Magic/Magic.pro
# hand hacked by SvOlli
# distributed as public domain
######################################################################

TEMPLATE = subdirs

HEADERS += MagicEncoderInterface.hpp

SUBDIRS += MagicEncoder
SUBDIRS += MagicEncoderFlac
SUBDIRS += MagicEncoderMp3
SUBDIRS += MagicEncoderOgg
SUBDIRS += MagicEncoderWav

MagicEncoderFlac.depends += MagicEncoder
MagicEncoderOgg.depends  += MagicEncoder
MagicEncoderMp3.depends  += MagicEncoder
MagicEncoderWav.depends  += MagicEncoder
