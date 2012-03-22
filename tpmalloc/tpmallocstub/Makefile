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

############################### geant4_vmc Makefile ###########################

PACKAGE   = tpmallocstub

ifeq ($(PLATFORM),)
PLATFORM = $(shell root-config --arch)
endif

TOPDIR  = $(shell pwd)/../..
BINDIR  = $(TOPDIR)/tmp/tgt_$(PLATFORM)/$(PACKAGE)
LIBDIR  = $(TOPDIR)/lib/tgt_$(PLATFORM)
INCDIR  = $(TOPDIR)/include/$(PACKAGE)

ifeq ($(ROOTSYS),)
ROOT_INCDIR := $(shell root-config --incdir)
ROOT_BINDIR := $(shell root-config --bindir)
ROOT_ETCDIR := $(shell root-config --etcdir)/vmc
else
ROOT_INCDIR = $(ROOTSYS)/include
ROOT_BINDIR = $(ROOTSYS)/bin
ROOT_ETCDIR = $(ROOTSYS)/etc/vmc
endif

include $(ROOT_ETCDIR)/Makefile.$(PLATFORM)

ifneq ($(findstring gcc,$(CC)),)
GCC_MAJOR    := $(shell $(CC) -dumpversion 2>&1 | cut -d'.' -f1)
GCC_MINOR    := $(shell $(CC) -dumpversion 2>&1 | cut -d'.' -f2)
GCC_VERS     := gcc-$(GCC_MAJOR).$(GCC_MINOR)
endif


############################### Sources #######################################

# Rules
#
include $(ROOT_ETCDIR)/MakeRules

# Sources
#
CSRC	:= $(wildcard src/*.c) 
SRCS	:= $(CSRC) 

# C++ Headers copied to include
#
HEADERS	:= $(wildcard include/*.h)

# Objects
#
COBJ	:= $(patsubst %.c,$(BINDIR)/%.o,$(CSRC))
OBJS	:= $(COBJ)

# Make include list
#
INCLUDES := -Iinclude

# C compilation flags
#
CFLAGS := -fPIC -O3 $(INCLUDES)

DEPINC 		+= -I. $(INCLUDES)

############################### Targets #######################################


SLIBRARY	= $(LIBDIR)/lib$(PACKAGE).$(SL)
ALIBRARY	= $(LIBDIR)/lib$(PACKAGE).a

default:	depend $(SLIBRARY) includes
#includes:       clean_includes make_includes
includes:       make_includes
static: 	$(ALIBRARY) 

$(LIBDIR)/lib$(PACKAGE).$(SL):  $(OBJS)
$(LIBDIR)/lib$(PACKAGE).a:      $(OBJS)

DICT:=	 	$(GDICT)

$(DICT): 	$(HDRS)

depend:		$(SRCS)

TOCLEAN		= $(BINDIR) $(INCDIR)
TOCLEANALL	= $(BINDIR) $(INCDIR) $(LIBDIR)

include $(ROOT_ETCDIR)/MakeMacros

############################### Dependencies ##################################

-include $(BINDIR)/Make-depend
