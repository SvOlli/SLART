
PROJECT = $(shell basename $$PWD)

QMAKE ?= $(shell type -p qmake-qt4 || type -p qmake)
QSHELL ?= $(shell type -p cmd.exe || type -p sh)
PLATFORM = $(shell uname -s -m|tr ' ' _)

all:
	mkdir -p ../$(PLATFORM)/$(PROJECT)
	(cd ../$(PLATFORM)/$(PROJECT) ; SHELL=$(QSHELL) $(QMAKE) ../../$(PROJECT)/$(PROJECT).pro && make $<)

