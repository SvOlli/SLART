
PROJECT = $(shell basename $$PWD)

QMAKE ?= $(shell type -p qmake-qt4 || type -p qmake)
PLATFORM = $(shell uname -s -m|tr ' ' _)

all:
	mkdir -p ../$(PLATFORM)/$(PROJECT)
	(cd ../$(PLATFORM)/$(PROJECT) ; $(QMAKE) ../../$(PROJECT)/$(PROJECT).pro && make $<)

