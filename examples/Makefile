# $Id$

#------------------------------------------------
# The Virtual Monte Carlo examples
# Copyright (C) 2007, 2008 Ivana Hrivnacova
# All rights reserved.
#
# For the licensing terms see geant4_vmc/LICENSE.
# Contact: vmc@pcroot.cern.ch
#-------------------------------------------------

# The makefile adopted from the old ALICE Makefiles system,
# developped by ALICE Offline project (http://aliceinfo.cern.ch/Offline)

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

