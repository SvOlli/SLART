
PLATFORM = $(shell uname -s -m|tr ' ' _)

SUBDIRS = taglib Funkytown Partyman Stripped Rubberbandman Innuendo

all:
	for dir in $(SUBDIRS); do [ ! -d $$dir ] || make -C $$dir $@ ; done
	ls -l $(PLATFORM)/bin

clean:
	rm -rf $(PLATFORM)

strip: all
	strip -R .note -R .comment $(PLATFORM)/bin/*

