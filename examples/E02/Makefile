# $Id: Makefile,v 1.1.1.1 2002/09/27 10:00:02 rdm Exp $

############################### geant4vmc Makefile ###########################

PACKAGE   = example02

ifndef PLATFORM
  PLATFORM = $(shell uname)
endif

TOPDIR  = $(shell pwd)/../..
BINDIR  = $(TOPDIR)/tmp/tgt_$(PLATFORM)/$(PACKAGE)
LIBDIR  = $(TOPDIR)/lib/tgt_$(PLATFORM)

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
HDRS	:= $(wildcard include/*.h) include/$(PACKAGE)LinkDef.h

# Objects
#
CXXOBJ	:= $(patsubst %.cxx,$(BINDIR)/%.o,$(CXXSRC))
OBJS	:= $(CXXOBJ) $(GDICTO)

# Make include list
#
INCDIR  := -Iinclude

# C++ compilation flags
#
CXXFLAGS := $(CXXOPTS) -I. $(INCDIR) -I$(ROOTSYS)/include 

DEPINC 		+= -I. -I$(ROOTSYS)/include $(INCDIR)

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
