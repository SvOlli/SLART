
QMAKE ?= qmake

all: release

debug release: src/Makefile
	+(cd src;make $(MAKEFLAGS) $@)

src/Makefile:
	(cd src;$(QMAKE) -r CONFIG+=debug_and_release)

clean:
	(cd src;make distclean)
	rm -rf build

