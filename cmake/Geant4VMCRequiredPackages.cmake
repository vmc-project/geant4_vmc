#------------------------------------------------
# The Virtual Monte Carlo examples
# Copyright (C) 2014 Ivana Hrivnacova
# All rights reserved.
#
# For the licensing terms see geant4_vmc/LICENSE.
# Contact: vmc@pcroot.cern.ch
#-------------------------------------------------

# Configuration file for CMake build for Geant4 VMC packages
# which finds all required and optional packages.
#
# I. Hrivnacova, 24/04/2014

#-- ROOT (required) ------------------------------------------------------------
find_package(ROOT REQUIRED)

#-- Geant4 (required) ----------------------------------------------------------
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

#-- G4Root from external installation (optional) -------------------------------
if (Geant4VMC_USE_SYSTEM_G4ROOT)
  find_package(G4ROOT REQUIRED)      
endif()

#-- VGM (optional) -------------------------------------------------------------
if (Geant4VMC_USE_VGM)
  find_package(VGM REQUIRED)      
endif()

