# $Id$

#------------------------------------------------
# The Geant4 Virtual Monte Carlo package
# Copyright (C) 2007, 2008 Ivana Hrivnacova
# All rights reserved.
#
# For the licensing terms see geant4_vmc/LICENSE.
# Contact: vmc@pcroot.cern.ch
#-------------------------------------------------

# The makefile adopted from the old ALICE Makefiles system,
# developped by ALICE Offline project (http://aliceinfo.cern.ch/Offline)

# Makefile for building all packages provided with geant4_vmc¨
# geant4_vmc, g4root and VMC examples

MAKEFLAGS= --no-print-directory

SUBDIRS  = g4root mtroot source examples

.PHONY: default static

default:
	@for dir in $(SUBDIRS); do (cd $$dir && echo "Making $$dir ..."  && $(MAKE)); done;:

static:
	@for dir in $(SUBDIRS); do (cd $$dir && echo "Making $$dir ..."  && $(MAKE) static); done;:

clean:
	@for dir in $(SUBDIRS); do (cd $$dir && $(MAKE) clean); done;:

dist:
	@cd source && $(MAKE) dist;

distsrc:
	@cd source && $(MAKE) distsrc;
        
install:
	@cd source && $(MAKE) install;        
