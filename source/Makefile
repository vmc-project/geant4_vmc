# $Id:  $

############################### geant4_vmc Makefile ###########################

PACKAGE   = geant4vmc

TOPDIR  = $(shell pwd)/..
BINDIR  = $(TOPDIR)/tmp/tgt_$(G4SYSTEM)
LIBDIR  = $(TOPDIR)/lib/tgt_$(G4SYSTEM)

include ../config/Makefile.$(G4SYSTEM)

############################### Sources #######################################

# Source directories
#
GDIRS:=	global geometry digits+hits physics event run interfaces visualization

# Rules
#
include ../config/MakeRules

# Dictionary
#
GDICT    := $(BINDIR)/geant4Cint.cxx
GDICTH   := $(GDICT:.cxx=.h)
GDICTO   := $(patsubst %.cxx,$(BINDIR)/%.o,$(GDICT))

# Sources
#
CXXSRC	:= $(wildcard $(patsubst %,%/src/*.cxx,$(GDIRS))) 
SRCS	:= $(CXXSRC)

# C++ Headers
#
HDRS	:= run/include/TG4VRunConfiguration.h \
	   run/include/TG4RunConfiguration.h \
	   run/include/TGeant4.h \
	   $(wildcard interfaces/include/*.h) \
	   geant4vmcLinkDef.h

# Objects
#
CXXOBJ	:= $(patsubst %.cxx,$(BINDIR)/%.o,$(CXXSRC))
OBJS	:= $(CXXOBJ) $(GDICTO)

# Make include list
#
INCDIR  := $(patsubst %,-I%/include,$(GDIRS)) -I$(G4INSTALL)/include \
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

######################## Supported Geant4 VIS, UI #############################
#
# Supported Geant4 VIS, UI setting: 
# OPENGLX, OPENGLXM, XM, TCSH
#
ifndef G4VIS_NONE
  CPPFLAGS += -DG4VIS_USE
ifdef G4VIS_USE_OPENGLX
  G4VIS_USE_OPENGL = 1
  CPPFLAGS += -DG4VIS_USE_OPENGLX
  INC_X11   = 1
endif
ifdef G4VIS_USE_OPENGLXM
  G4VIS_USE_OPENGL = 1
  CPPFLAGS += -DG4VIS_USE_OPENGLXM
  INC_XM    = 1
endif
ifdef G4VIS_USE_OPENGL
  CPPFLAGS += -DG4VIS_USE_OPENGL
  CPPFLAGS += -I$(G4INSTALL)/source/visualization/OpenGL/include
  INC_GL    = 1
endif
  CPPFLAGS += -I$(G4INSTALL)/source/visualization/management/include
  CPPFLAGS += -I$(G4INSTALL)/source/visualization/modeling/include
endif #G4VIS_NONE

ifndef G4UI_NONE
  CPPFLAGS += -I$(G4INSTALL)/source/interfaces/basic/include
  CPPFLAGS += -I$(G4INSTALL)/source/interfaces/common/include
  CPPFLAGS += -I$(G4INSTALL)/source/interfaces/GAG/include
  CPPFLAGS += -DG4UI_USE_TERMINAL
ifdef G4UI_USE_TCSH
  G4UI_USE  = 1
  CPPFLAGS += -DG4UI_USE_TCSH
endif
endif #G4UI_NONE

ifdef INC_GL
  CPPFLAGS += $(OGLFLAGS)
endif
ifdef INC_XM
  INC_X11   = 1
  CPPFLAGS += $(XMFLAGS)
endif
ifdef INC_X11
  CPPFLAGS += $(X11FLAGS)
endif


######################## Full Geant4 VIS, UI ##################################
#
# Full Geant4 UI, VIS, interactivity setting.
# It enables to use all Geant4 graphics drivers and UIs,  
# however not all these options were tested with Geant4 VMC
# and even though a correct behaviour is expected,
# it is not guaranteed. 
# 
#include $(G4INSTALL)/config/G4UI_USE.gmk
#include $(G4INSTALL)/config/G4VIS_USE.gmk
#include $(G4INSTALL)/config/interactivity.gmk

CXXFLAGS += $(CPPFLAGS)

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

include ../config/MakeMacros

############################### Dependencies ##################################

-include $(BINDIR)/Make-depend
