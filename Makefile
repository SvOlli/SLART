
PLATFORM = $(shell sh configure --buildarch)
VERSION = $(shell sh configure --version)
PREFIX = $(shell sh configure --prefix)

SUBDIRS = taglib Funkytown Partyman Stripped Rubberbandman \
          Karmadrome Creep Innuendo Sorcerer
TARCONT = configure Makefile Global.pri TemplateApp Common \
          $(SUBDIRS) extra/*.sh docs

all:
	for dir in $(SUBDIRS); do [ ! -d $$dir ] || make -C $$dir $@ ; done
	ls -l $(PLATFORM)/{bin,lib} || true

clean:
	for i in $(SUBDIRS); do rm -rf $(PLATFORM)/$$i $(PLATFORM)/{bin,lib}/*$$i*;done

distclean:
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
	mkdir -p $(DESTDIR)$(PREFIX)/{bin,lib}
	cp -d $(PLATFORM)/bin/* $(DESTDIR)$(PREFIX)/bin
	cp -d $(PLATFORM)/lib/* $(DESTDIR)$(PREFIX)/lib

tar:
	ln -s SLART ../slart-$(VERSION)
	(cd ..;tar jcf slart-$(VERSION).tar.bz2 slart-$(VERSION)/{$(shell echo $(TARCONT)|tr ' ' ',')};ls -l slart-$(VERSION).tar.bz2)
	rm ../slart-$(VERSION)

deb:
	[ -e ../slart-$(VERSION) ] || ln -sv SLART ../slart-$(VERSION)
	[ -e debian ] || cp -r extra/debian debian
	extra/gitlog2changelog.sh > debian/changelog
	cd ../slart-$(VERSION) && fakeroot dpkg-buildpackage
	rm -f *-stamp
	[ ! -d debian ] || rm -r debian
	[ ! -h ../slart-$(VERSION) ] || rm ../slart-$(VERSION)
