#------------------------------------------------
# The Geant4 Virtual Monte Carlo package
# Copyright (C) 2014 Ivana Hrivnacova
# All rights reserved.
#
# For the licensing terms see geant4_vmc/LICENSE.
# Contact: vmc@pcroot.cern.ch
#-------------------------------------------------

# Configuration file for CMake build for Geant4 VMC packages
# which finds all required and optional packages.
#
# I. Hrivnacova, 13/06/2014

#-- ROOT (required) ------------------------------------------------------------
find_package(ROOT REQUIRED)

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
find_package(Geant4 REQUIRED ${_components})

#-- VGM (optional) -------------------------------------------------------------
if (Geant4VMC_USE_VGM)
  find_package(VGM REQUIRED)      
endif()

