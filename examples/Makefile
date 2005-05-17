# $Id: Makefile,v 1.2 2003/03/13 21:38:48 brun Exp $
#
# Makefile for building all examples

MAKEFLAGS= --no-print-directory

SUBDIRS  = E01 E02 E03 E06

ifndef MCINSTALL
  MCINSTALL = ..
endif

.PHONY: default

default:
	@for dir in $(SUBDIRS); do (cd $$dir && $(MAKE)); done;:

clean:
	@for dir in $(SUBDIRS); do (cd $$dir && $(MAKE) clean); done;:

