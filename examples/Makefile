# $Id: Makefile,v 1.1.1.1 2002/09/27 10:00:02 rdm Exp $
#
# Makefile for building all examples

MAKEFLAGS= --no-print-directory

SUBDIRS  = E01 E02 E03

ifndef MCINSTALL
  MCINSTALL = ..
endif

.PHONY: default

default:
	@for dir in $(SUBDIRS); do (cd $$dir && $(MAKE)); done;:

clean:
	@for dir in $(SUBDIRS); do (cd $$dir && $(MAKE) clean); done;:

