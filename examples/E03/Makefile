# $Id: Makefile,v 1.1 2003/03/17 14:56:50 brun Exp $

############################### geant4vmc Makefile ###########################

PACKAGE   = example03

ifeq ($(PLATFORM),)
PLATFORM = $(shell root-config --arch)
endif

TOPDIR  = $(shell pwd)/../..
BINDIR  = $(TOPDIR)/tmp/tgt_$(PLATFORM)/$(PACKAGE)
LIBDIR  = $(TOPDIR)/lib/tgt_$(PLATFORM)

ifeq ($(ROOTSYS),)
ROOT_INCDIR = $(shell root-config --incdir)
ROOT_BINDIR = $(shell root-config --prefix)/bin
else
ROOT_INCDIR = $(ROOTSYS)/include
ROOT_BINDIR = $(ROOTSYS)/bin
endif

include $(TOPDIR)/config/Makefile.$(PLATFORM)

############################### Sources #######################################

# Rules
#
include $(TOPDIR)/config/MakeRules

# Dictionary
#
GDICT    := $(BINDIR)/$(PACKAGE)Cint.cxx
GDICTH   := $(GDICT:.cxx=.h)
GDICTO   := $(patsubst %.cxx,$(BINDIR)/%.o,$(GDICT))

# Sources
#
CXXSRC	:= $(wildcard src/*.cxx) 
SRCS	:= $(CXXSRC)

# C++ Headers
#
HDRS    := $(wildcard include/*.h)
HDRS    := $(filter-out include/$(PACKAGE)LinkDef.h,$(HDRS))
HDRS    += include/$(PACKAGE)LinkDef.h

# Objects
#
CXXOBJ	:= $(patsubst %.cxx,$(BINDIR)/%.o,$(CXXSRC))
OBJS	:= $(CXXOBJ) $(GDICTO)

# Make include list
#
INCDIR  := -Iinclude

# C++ compilation flags
#
CXXFLAGS := $(CXXOPTS) -I. $(INCDIR) -I$(ROOT_INCDIR) 

DEPINC 		+= -I. -I$(ROOT_INCDIR) $(INCDIR)

############################### Targets #######################################


SLIBRARY	= $(LIBDIR)/lib$(PACKAGE).$(SL)
ALIBRARY	= $(LIBDIR)/lib$(PACKAGE).a

default:	depend $(SLIBRARY)


$(LIBDIR)/lib$(PACKAGE).$(SL):  $(OBJS)
$(LIBDIR)/lib$(PACKAGE).a:      $(OBJS)

DICT:=	 	$(GDICT)

$(DICT): 	$(HDRS)

depend:		$(SRCS)

TOCLEAN		= $(BINDIR)
TOCLEANALL	= $(BINDIR) $(LIBDIR)

include $(TOPDIR)/config/MakeMacros

############################### Dependencies ##################################

-include $(BINDIR)/Make-depend
