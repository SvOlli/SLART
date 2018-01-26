
QMAKE ?= qmake
PREFIX ?= /usr
DOCS = example.lircrc README.GPL README.LGPL README.TagLib SLARTmessages.txt StyleSheet.txt

.PHONY: release debug clean distclean all tar deb install doc translations

all: release

tar:
	./configure
	dpkg-buildpackage -S

deb:
	dpkg-buildpackage -b

release: build-release/Makefile
	+make -C build-release all

debug: build-debug/Makefile
	+make -C build-debug all

build-release/Makefile:
	(mkdir -p build-release;cd build-release;$(QMAKE) -r CONFIG+=release CONFIG-=debug ../src/src.pro)

build-debug/Makefile:
	(mkdir -p build-debug;cd build-debug;$(QMAKE) -r CONFIG+=debug CONFIG-=release ../src/src.pro)

distclean:
	rm -rf build-release build-debug docs/generated

clean:
	[ ! -d build-release ] || { cd build-release; make clean; }
	[ ! -d build-debug ]   || { cd build-debug; make clean; }

install: release
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	mkdir -p $(DESTDIR)$(PREFIX)/lib
	mkdir -p $(DESTDIR)$(PREFIX)/share/doc/slart
	mkdir -p $(DESTDIR)$(PREFIX)/share/slart/stylesheets
	mkdir -p $(DESTDIR)$(PREFIX)/share/pixmaps
	mkdir -p $(DESTDIR)$(PREFIX)/share/applications
	cp -d build-release/bin/* $(DESTDIR)$(PREFIX)/bin
	cp -d build-release/lib/*.so* $(DESTDIR)$(PREFIX)/lib
	#cp -d extra/icons/* $(DESTDIR)$(PREFIX)/share/pixmaps
	for i in $$(cd $(DESTDIR)$(PREFIX)/bin;ls [D-Z]*) ; do \
	  convert src/resources/icons/$$i.png \
	  $(DESTDIR)$(PREFIX)/share/pixmaps/$$i.xpm; done
	cp -d extra/menu/*.desktop $(DESTDIR)$(PREFIX)/share/applications
	cp -d extra/stylesheets/* \
          $(DESTDIR)$(PREFIX)/share/slart/stylesheets
	[ ! -f build-release/README.DerMixD ] || \
	  gzip -9 <build-release/README.DerMixD \
	    >>$(DESTDIR)$(PREFIX)/share/doc/slart/README.DerMixD.gz
	for f in $(DOCS); do \
	  gzip -9 <docs/$$f >$(DESTDIR)$(PREFIX)/share/doc/slart/$$f.gz ; done
	strip -R .note -R .comment $(DESTDIR)$(PREFIX)/bin/*
	strip --strip-unneeded $(DESTDIR)$(PREFIX)/lib/*

doc:
	extra/generate-docs.sh

translations:
	extra/generate-translations.sh

