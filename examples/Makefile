# $Id: $
#
# Makefile for building all examples

MAKEFLAGS= --no-print-directory

SUBDIRS  = E01 E02

ifndef MCINSTALL
  MCINSTALL = ..
endif

.PHONY: default

default:
	@for dir in $(SUBDIRS); do (cd $$dir && $(MAKE)); done;:

clean:
	@for dir in $(SUBDIRS); do (cd $$dir && $(MAKE) clean); done;:

