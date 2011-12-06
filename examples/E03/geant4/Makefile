# $Id$

############################### geant4_vmc Makefile ###########################

PACKAGE   = geant4e03

ifeq ($(PLATFORM),)
PLATFORM = $(shell root-config --arch)
endif

TOPDIR  = $(shell pwd)/../../..
BINDIR  = $(TOPDIR)/tmp/tgt_$(PLATFORM)/$(PACKAGE)
LIBDIR  = $(TOPDIR)/lib/tgt_$(PLATFORM)

ifeq ($(ROOTSYS),)
ROOT_INCDIR = $(shell root-config --incdir)
ROOT_BINDIR = $(shell root-config --prefix)/bin
ROOT_ETCDIR = $(shell root-config --etcdir)/vmc
else
ROOT_INCDIR = $(ROOTSYS)/include
ROOT_BINDIR = $(ROOTSYS)/bin
ROOT_ETCDIR = $(ROOTSYS)/etc/vmc
endif

include $(ROOT_ETCDIR)/Makefile.$(PLATFORM)

############################### Sources #######################################

# Rules
#
include $(ROOT_ETCDIR)/MakeRules

# Dictionary
#
GDICT    := $(BINDIR)/$(PACKAGE)Cint.cxx
GDICTH   := $(GDICT:.cxx=.h)
GDICTO   := $(patsubst %.cxx,%.o,$(GDICT))

# Sources
#
CXXSRC	:= $(wildcard src/*.cxx) 
SRCS	:= $(CXXSRC)

# C++ Headers processed by CINT
#
HDRS	:= include/Ex03RunConfiguration1.h \
	   include/Ex03RunConfiguration2.h \
	   include/Ex03RunConfiguration3.h \
	   include/$(PACKAGE)LinkDef.h

# Objects
#
CXXOBJ	:= $(patsubst %.cxx,$(BINDIR)/%.o,$(CXXSRC))
OBJS	:= $(CXXOBJ) $(GDICTO)

# Make include list
#
ifeq ($(G4INSTALL),)
  #G4INCLUDE := $(shell geant4-config --prefix)/include/Geant4
  G4CFLAGS := $(shell geant4-config --cflags)
  #additional flags - not handled via geant4-config
  ifeq ($(G4VIS_NONE),) 
    G4CFLAGS += -DG4VIS_USE 
  endif  
  ifeq ($(G4UI_NONE),) 
    G4CFLAGS += -DG4UI_USE 
  endif  
else   
  ifndef G4INCLUDE
    G4CFLAGS := -I$(G4INSTALL)/include
  else
    G4CFLAGS := -I$(G4INCLUDE)
  endif 
endif   

INCLUDES := -Iinclude \
	   -I$(CLHEP_BASE_DIR)/include \
	   -I$(TOPDIR)/include/geant4vmc \
	   -I$(TOPDIR)/include

ifneq ($(USE_VGM),)
INCLUDES  += -I$(VGM_INSTALL)/packages/VGM/include \
	   -I$(VGM_INSTALL)/packages/ClhepVGM/include \
	   -I$(VGM_INSTALL)/packages/BaseVGM/include \
	   -I$(VGM_INSTALL)/packages/Geant4GM/include \
	   -I$(VGM_INSTALL)/packages/RootGM/include \
	   -I$(VGM_INSTALL)/packages/XmlVGM/include
endif

# C++ compilation flags
#
CXXFLAGS := $(CXXOPTS) -I. $(INCLUDES) -I$(ROOT_INCDIR) $(G4CFLAGS) 

# Debug option 
#
ifdef G4DEBUG
  CXXFLAGS += -DMCDEBUG
endif

# Use VGM flag 
#
ifneq ($(USE_VGM),)
  CXXFLAGS += -DUSE_VGM
endif

######################## Supported Geant4 VIS, UI #############################
#
# Supported Geant4 VIS, UI setting: 
# OPENGLX, OPENGLXM, XM, TCSH
#
ifneq ($(G4INSTALL),)
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

DEPINC 		+= -I. -I$(ROOT_INCDIR) $(INCLUDES)

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

include $(ROOT_ETCDIR)/MakeMacros

############################### Dependencies ##################################

-include $(BINDIR)/Make-depend
