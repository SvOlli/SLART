
PLATFORM = $(shell sh configure --buildarch)

PREFIX = $(shell sh configure --prefix)

SUBDIRS = taglib Funkytown Partyman Stripped Rubberbandman \
          Karmadrome Creep Innuendo Sorcerer

all:
	for dir in $(SUBDIRS); do [ ! -d $$dir ] || make -C $$dir $@ ; done
	ls -l $(PLATFORM)/bin

clean:
	rm -rf $(PLATFORM)
	@echo
	@echo " note: configuration was deleted as well!"
	@echo

strip: all
	strip -R .note -R .comment $(PLATFORM)/bin/*

tools:
	(cd extra ; ./build-mpg123-dermixd.sh)

toolchain:
	(cd extra ; ./install-dev-packages.sh)

everything: toolchain all tools

install: strip
	mkdir -p $(PREFIX)/bin
	cp -d $(PLATFORM)/bin/* $(PREFIX)/bin

