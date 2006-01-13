# $Id: Makefile,v 1.3 2005/05/17 13:44:25 brun Exp $
#
# Makefile for building all examples

MAKEFLAGS= --no-print-directory

SUBDIRS  = E01 E02 E03 E06

ifdef G4INSTALL
  SUBDIRS += E03/geant4
endif

.PHONY: default

default:
	@for dir in $(SUBDIRS); do (cd $$dir && $(MAKE)); done;:

clean:
	@for dir in $(SUBDIRS); do (cd $$dir && $(MAKE) clean); done;:

