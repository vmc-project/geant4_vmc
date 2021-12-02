#------------------------------------------------
# The Geant4 Virtual Monte Carlo package
# Copyright (C) 2014 Ivana Hrivnacova
# All rights reserved.
#
# For the licensing terms see geant4_vmc/LICENSE.
# Contact: root-vmc@cern.ch
#-------------------------------------------------

# Configuration file for CMake build for Geant4 VMC packages
# which finds all required and optional packages.
#
# I. Hrivnacova, 13/06/2014

#message(STATUS Processing Geant4VMCRequiredPackages)

#-- ROOT (required) ------------------------------------------------------------
find_package(ROOT CONFIG REQUIRED COMPONENTS EG Geom REQUIRED)
set(ROOT_DEPS ROOT::Core ROOT::RIO ROOT::Tree ROOT::Rint ROOT::Physics
    ROOT::MathCore ROOT::Thread ROOT::Geom ROOT::EG)
include(${ROOT_USE_FILE})

#-- VMC (required) ------------------------------------------------------------
find_package(VMC CONFIG)
# first try VMC standalone (default)
if(VMC_FOUND)
  if(NOT VMC_FIND_QUIETLY)
    message(STATUS "Found VMC ${VMC_VERSION} in ${VMC_DIR}")
  endif()
else()
  # otherwise fallback to ROOT's internal if possible (deprecated)
  if(ROOT_vmc_FOUND)
    message(WARNING "Using VMC built with ROOT - Deprecated")
    set(VMC_LIBRARIES ROOT::VMC)
    add_definitions(-DUSE_ROOT_VMC)
  else()
    message(FATAL_ERROR
            "Could not find VMC package. "
            "VMC package is not provided with ROOT since v6.26/00. "
            "Please, install it from https://github.com/vmc-project/vmc. ")
  endif()
endif()

#-- Geant4 (required) ----------------------------------------------------------
set(_components)
if(Geant4VMC_USE_GEANT4_UI)
  list(APPEND _components ui_all)
endif()
if(Geant4VMC_USE_GEANT4_VIS)
  list(APPEND _components vis_all)
endif()
if(Geant4VMC_USE_GEANT4_G3TOG4)
  list(APPEND _components g3tog4)
endif()
find_package(Geant4 CONFIG REQUIRED ${_components})

#-- VGM (optional) -------------------------------------------------------------
if (Geant4VMC_USE_VGM)
  find_package(VGM CONFIG REQUIRED)
endif()
