# $Id: Makefile,v 1.1 2003/07/22 06:46:57 brun Exp $
#
# Makefile for building all examples

MAKEFLAGS= --no-print-directory

SUBDIRS  = g4toxml roottog4

.PHONY: default

default:
	@for dir in $(SUBDIRS); do (cd $$dir && $(MAKE)); done;

includes:
	@for dir in $(SUBDIRS); do (cd $$dir && $(MAKE) make_includes); done;

clean:
	@for dir in $(SUBDIRS); do (cd $$dir && $(MAKE) clean); done;

