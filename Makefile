
PLATFORM = $(shell uname -s -m|tr ' ' _)

SUBDIRS = taglib Funkytown Partyman Stripped Rubberbandman \
          Karmadrome Creep Innuendo

all:
	for dir in $(SUBDIRS); do [ ! -d $$dir ] || make -C $$dir $@ ; done
	ls -l $(PLATFORM)/bin

clean:
	rm -rf $(PLATFORM)

strip: all
	strip -R .note -R .comment $(PLATFORM)/bin/*

tools:
	(cd extra ; ./build-mpg123-dermixd.sh)

toolchain:
	(cd extra ; ./install-dev-packages.sh)

everything: toolchain all tools

