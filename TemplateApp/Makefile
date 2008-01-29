
PROJECT = $(shell basename $$PWD)

QMAKE ?= qmake-qt4
PLATFORM = $(shell uname -s -m|tr ' ' _)

all:
	mkdir -p ../$(PLATFORM)/$(PROJECT)
	(cd ../$(PLATFORM)/$(PROJECT) ; $(QMAKE) ../../$(PROJECT)/$(PROJECT).pro && make $<)

