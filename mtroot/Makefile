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

PACKAGE   = mtroot

ifeq ($(PLATFORM),)
PLATFORM = $(shell root-config --arch)
endif

TOPDIR  = $(shell pwd)/..
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

# Pass -std=c++11 flag from Root 
# (as it is required for generation dicxtionaries with Root 6-beta)
ROOT_CFLAGS :=  $(filter -std=c++11,$(shell root-config --cflags))

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

# Dictionary
#
#GDICT    := $(BINDIR)/mtrootCint.cxx
#GDICTH   := $(GDICT:.cxx=.h)
#GDICTO   := $(patsubst %.cxx,%.o,$(GDICT))

# Sources
#
CXXSRC	:= $(wildcard src/*.cxx) 
SRCS	:= $(CXXSRC)

# C++ Headers processed by CINT
#
#HDRS    := $(wildcard include/*.h)
#HDRS    := $(filter-out include/$(PACKAGE)LinkDef.h,$(HDRS))
#HDRS    += include/$(PACKAGE)LinkDef.h

# C++ Headers copied to include
#
HEADERS	:= $(wildcard include/*.h)

# Objects
#
CXXOBJ	:= $(patsubst %.cxx,$(BINDIR)/%.o,$(CXXSRC))
#OBJS	:= $(CXXOBJ) $(GDICTO)
OBJS	:= $(CXXOBJ)

# Make include list
#
ifeq ($(G4INSTALL),)
#  G4CFLAGS := $(shell geant4-config --cflags)
  G4CFLAGS := -DG4INTY_USE_XT -DG4VIS_USE_OPENGL -DG4UI_USE_TCSH -DG4VIS_USE_RAYTRACERX -DG4VIS_USE_OPENGLX -DG4MULTITHREADED -ftls-model=global-dynamic -I/work/projects/vmc/mt/geant4/bin/../include/Geant4
else   
  ifndef G4INCLUDE
    G4CFLAGS := -I$(G4INSTALL)/include
  else
    G4CFLAGS := -I$(G4INCLUDE)
  endif 
endif   

INCLUDES := -Iinclude \
	   -I$(CLHEP_BASE_DIR)/include

# C++ compilation flags
#
CXXFLAGS := $(CXXOPTS) -I. $(INCLUDES) -I$(ROOT_INCDIR) $(G4CFLAGS) $(ROOT_CFLAGS)

DEPINC 		+= -I. -I$(ROOT_INCDIR) $(INCLUDES)

############################### Targets #######################################


SLIBRARY	= $(LIBDIR)/lib$(PACKAGE).$(SL)
ALIBRARY	= $(LIBDIR)/lib$(PACKAGE).a

default:	depend $(SLIBRARY) includes
#includes:       clean_includes make_includes
includes:       make_includes
static: 	$(ALIBRARY) 

$(LIBDIR)/lib$(PACKAGE).$(SL):  $(OBJS)
$(LIBDIR)/lib$(PACKAGE).a:      $(OBJS)

#DICT:=	 	$(GDICT)

#$(DICT): 	$(HDRS)

depend:		$(SRCS)

TOCLEAN		= $(BINDIR) $(INCDIR)
TOCLEANALL	= $(BINDIR) $(INCDIR) $(LIBDIR)

include $(ROOT_ETCDIR)/MakeMacros

############################### Dependencies ##################################

-include $(BINDIR)/Make-depend
