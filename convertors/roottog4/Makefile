# $Id: Makefile,v 1.7 2002/11/13 13:36:41 brun Exp $

############################### geant4_vmc Makefile ###########################

PACKAGE   = roottog4

ifndef PLATFORM
  PLATFORM = $(shell uname)
endif

TOPDIR  = $(shell pwd)/../..
BINDIR  = $(TOPDIR)/tmp/tgt_$(PLATFORM)/$(PACKAGE)
LIBDIR  = $(TOPDIR)/lib/tgt_$(PLATFORM)

include $(TOPDIR)/config/Makefile.$(PLATFORM)

############################### Sources #######################################

# Source directories
#
#GDIRS:=	roottog4

# Rules
#
include $(TOPDIR)/config/MakeRules

# Dictionary
#
#GDICT    := $(BINDIR)/geant4Cint.cxx
#GDICTH   := $(GDICT:.cxx=.h)
#GDICTO   := $(patsubst %.cxx,$(BINDIR)/%.o,$(GDICT))

# Sources
#
SRCS	:= $(wildcard src/*.cxx) 

# C++ Headers processed by CINT
#
#HDRS	:= run/include/TG4VRunConfiguration.h \
#	   run/include/TG4RunConfiguration.h \
#	   run/include/TGeant4.h \
#	   $(wildcard interfaces/include/*.h) \
#	   geant4vmcLinkDef.h

# C++ Headers copied to include
#
HEADERS	:= $(wildcard include/*.h)

# Objects
#
OBJS	:= $(patsubst %.cxx,$(BINDIR)/%.o,$(SRCS))

# Make include list
#
ifndef G4INCLUDE
  G4INCLUDE := $(G4INSTALL)/include
endif  

INCDIR  := -Iinclude -I$(G4INCLUDE) \
	   -I$(CLHEP_BASE_DIR)/include \
	   -I$(G4INSTALL)/source/interfaces/common/include

# C++ compilation flags
#
CXXFLAGS := $(CXXOPTS) $(G4MCDEFS) -I. $(INCDIR) -I$(ROOTSYS)/include 

# Debug option 
#
ifdef G4DEBUG
  CXXFLAGS += -DMCDEBUG
endif

######################## Full Geant4 VIS, UI ##################################
#
# Full Geant4 UI, VIS, interactivity setting.
# It enables to use all Geant4 graphics drivers and UIs,  
# however not all these options were tested with Geant4 VMC
# and even though a correct behaviour is expected,
# it is not guaranteed. 
# 
include $(G4INSTALL)/config/G4UI_USE.gmk
include $(G4INSTALL)/config/G4VIS_USE.gmk
include $(G4INSTALL)/config/interactivity.gmk

CXXFLAGS += $(CPPFLAGS)

DEPINC 		+= -I. -I$(ROOTSYS)/include $(INCDIR)

############################### Targets #######################################


SLIBRARY	= $(LIBDIR)/lib$(PACKAGE).$(SL)
ALIBRARY	= $(LIBDIR)/lib$(PACKAGE).a

default:	depend $(SLIBRARY)
all:		default includes


$(LIBDIR)/lib$(PACKAGE).$(SL):  $(OBJS)
$(LIBDIR)/lib$(PACKAGE).a:      $(OBJS)

DICT:=	 	$(GDICT)

$(DICT): 	$(HDRS)

depend:		$(SRCS)

TOCLEAN		= $(BINDIR) $(TOPDIR)/include
TOCLEANALL	= $(BINDIR) $(TOPDIR)/include $(LIBDIR)

MAKEDIST	= $(TOPDIR)/config/makedist.sh lib
MAKEDISTSRC	= $(TOPDIR)/config/makedist.sh

include $(TOPDIR)/config/MakeMacros

############################### Dependencies ##################################

-include $(BINDIR)/Make-depend
