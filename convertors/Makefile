# $Id: Makefile,v 1.2 2003/10/03 10:51:51 brun Exp $
#
# Makefile for building all examples

MAKEFLAGS= --no-print-directory

SUBDIRS  = g4toxml roottog4

.PHONY: default

default:
	@for dir in $(SUBDIRS); do (cd $$dir && $(MAKE)); done;

includes:
	@for dir in $(SUBDIRS); do (cd $$dir && $(MAKE) make_includes); done;
	
static:
	@for dir in $(SUBDIRS); do (cd $$dir && $(MAKE) static); done;

clean:
	@for dir in $(SUBDIRS); do (cd $$dir && $(MAKE) clean); done;

