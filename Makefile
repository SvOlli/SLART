
PLATFORM = $(shell uname -s -m|tr ' ' _)

SUBDIRS = taglib Funkytown Partyman Stripped Rubberbandman MySpaceHack

all:
	for dir in $(SUBDIRS); do [ ! -d $$dir ] || make -C $$dir $@ ; done

clean:
	rm -rf $(PLATFORM)

strip: all
	strip -R .note -R .comment $(PLATFORM)/bin/*

