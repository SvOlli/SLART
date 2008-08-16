
PROJECT = $(shell basename $$PWD)

QMAKE ?= $(shell sh ../configure --qmake)
PLATFORM = $(shell sh ../configure --buildarch)

all:
	mkdir -p ../$(PLATFORM)/$(PROJECT)
	(cd ../$(PLATFORM)/$(PROJECT) ; $(QMAKE) ../../$(PROJECT)/$(PROJECT).pro && make $(shell sh ../configure --jobs) $<)

clean:
	rm -rf ../$(PLATFORM)/$(PROJECT)

