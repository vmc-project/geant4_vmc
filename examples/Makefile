# $Id: Makefile,v 1.4 2006/01/13 23:05:52 brun Exp $
#
# Makefile for building all examples

MAKEFLAGS= --no-print-directory

SUBDIRS  = E01 E02 E03 E06

MAKE_GEANT4 := $(shell ls ../include/TG4RunConfiguration.h )
ifeq ($(MAKE_GEANT4),../include/TG4RunConfiguration.h)
  SUBDIRS += E03/geant4
endif


.PHONY: default

default:
	@for dir in $(SUBDIRS); do (cd $$dir && $(MAKE)); done;:

clean:
	@for dir in $(SUBDIRS); do (cd $$dir && $(MAKE) clean); done;:

