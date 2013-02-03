
QMAKE ?= qmake
DOCS = example.lircrc README.GPL README.LGPL README.TagLib SLARTmessages.txt StyleSheet.txt

all: release

cdebug:
	mkdir -p cbuild/debug
	+(cd cbuild/debug && cmake ../../src -DCMAKE_BUILD_TYPE=debug && make)

crelease:
	mkdir -p cbuild/release
	+(cd cbuild/release && cmake ../../src && make)

debug release: src/Makefile
	+(cd src;make $(MAKEFLAGS) $@)

src/Makefile:
	(cd src;$(QMAKE) -r CONFIG+=debug_and_release)

clean:
	(cd src;make distclean)
	rm -rf build

install: release
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	mkdir -p $(DESTDIR)$(PREFIX)/lib
	mkdir -p $(DESTDIR)$(PREFIX)/share/doc/slart
	mkdir -p $(DESTDIR)$(PREFIX)/share/slart/stylesheets
	mkdir -p $(DESTDIR)$(PREFIX)/share/pixmaps
	mkdir -p $(DESTDIR)$(PREFIX)/share/applications
	cp -d build/release/bin/* $(DESTDIR)$(PREFIX)/bin
	cp -d build/release/lib/*.so* $(DESTDIR)$(PREFIX)/lib
	#cp -d extra/icons/* $(DESTDIR)$(PREFIX)/share/pixmaps
        for i in $$(cd $(DESTDIR)$(PREFIX)/bin;ls|grep -v Creep) ; do \
          convert src/resources/icons/$$i.png \
          $(DESTDIR)$(PREFIX)/share/pixmap/$$i.xpm; done
	cp -d extra/menu/*.desktop $(DESTDIR)$(PREFIX)/share/applications
	cp -d extra/stylesheets/* \
          $(DESTDIR)$(PREFIX)/share/slart/stylesheets
	[ ! -f build/release/README.DerMixD ] || \
	  gzip -9 <build/release/README.DerMixD \
	    >>$(DESTDIR)$(PREFIX)/share/doc/slart/README.DerMixD.gz
	for f in $(DOCS); do \
	  gzip -9 <docs/$$f >$(DESTDIR)$(PREFIX)/share/doc/slart/$$f.gz ; done
	strip -R .note -R .comment $(DESTDIR)$(PREFIX)/bin/*
	strip --strip-unneeded $(DESTDIR)$(PREFIX)/lib/*

doc:
	extra/generate-docs.sh

translations:
	extra/generate-translations.sh

