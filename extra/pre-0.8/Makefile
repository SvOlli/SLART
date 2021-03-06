
PLATFORM = $(shell sh configure --buildarch)
VERSION = $(shell sh configure --version)
PREFIX = $(shell sh configure --prefix)

SUBDIRS = taglib Funkytown Partyman Stripped Rubberbandman \
          Karmadrome Creep Innuendo Sorcerer
TARCONT = configure Makefile docs Global.pri TemplateApp Common \
          $(SUBDIRS) extra/*.sh extra/icons extra/menu extra/stylesheets
DOCS = COPYING example.lircrc README.TagLib SLARTmessages.txt StyleSheet.txt

all:
	for dir in $(SUBDIRS); do [ ! -d $$dir ] || make -C $$dir $@ ; done
	ls -l $(PLATFORM)/bin $(PLATFORM)/lib || true

clean:
	for i in $(SUBDIRS); do rm -rf $(PLATFORM)/$$i $(PLATFORM)/bin/*$$i* $(PLATFORM)/lib/*$$i*;done

distclean:
	rm -rf $(PLATFORM)
	@echo
	@echo " note: configuration was deleted as well!"
	@echo

strip: all
	strip -R .note -R .comment $(PLATFORM)/bin/*

install: strip
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	mkdir -p $(DESTDIR)$(PREFIX)/lib
	mkdir -p $(DESTDIR)$(PREFIX)/share/doc/slart
	mkdir -p $(DESTDIR)$(PREFIX)/share/slart/stylesheets
	mkdir -p $(DESTDIR)$(PREFIX)/share/pixmaps
	mkdir -p $(DESTDIR)$(PREFIX)/share/applications
	cp -d $(PLATFORM)/bin/* $(DESTDIR)$(PREFIX)/bin
	cp -d $(PLATFORM)/lib/* $(DESTDIR)$(PREFIX)/lib
	cp -d extra/icons/* $(DESTDIR)$(PREFIX)/share/pixmaps
	cp -d extra/menu/*.desktop $(DESTDIR)$(PREFIX)/share/applications
	cp -d extra/stylesheets/* \
          $(DESTDIR)$(PREFIX)/share/slart/stylesheets
	[ ! -f $(PLATFORM)/README.DerMixD ] || \
	  gzip -9 <$(PLATFORM)/README.DerMixD \
	    >>$(DESTDIR)$(PREFIX)/share/doc/slart/README.DerMixD.gz
	for f in $(DOCS); do \
	  gzip -9 <docs/$$f >$(DESTDIR)$(PREFIX)/share/doc/slart/$$f.gz ; done

tar:
	[ -e ../slart-$(VERSION) ] || ln -s SLART ../slart-$(VERSION)
	(cd ..;tar jcf slart-$(VERSION).tar.bz2 $(foreach ent,$(TARCONT),slart-$(VERSION)/$(ent));ls -l slart-$(VERSION).tar.bz2)
	rm ../slart-$(VERSION)

deb:
	extra/debian/prepare-debian.sh
	(cd ../slart-$(VERSION) && dpkg-buildpackage -uc -b -rfakeroot)
	extra/debian/cleanup-debian.sh

dist: tar deb
	ls -l ../slart?$(VERSION)*

