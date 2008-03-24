
PROJECT = $(shell basename $$PWD)

QMAKE ?= $(shell which qmake-qt4 || which qmake)
PLATFORM = $(shell uname -s -m|tr ' ' _)

all:
	mkdir -p ../$(PLATFORM)/$(PROJECT)
	(cd ../$(PLATFORM)/$(PROJECT) ; $(QMAKE) ../../$(PROJECT)/$(PROJECT).pro && make $(JOBS) $<)

clean:
	rm -rf ../$(PLATFORM)/$(PROJECT)

