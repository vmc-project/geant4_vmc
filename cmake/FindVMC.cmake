#------------------------------------------------
# The Virtual Monte Carlo examples
# Copyright (C) 2014 Ivana Hrivnacova
# All rights reserved.
#
# For the licensing terms see geant4_vmc/LICENSE.
# Contact: vmc@pcroot.cern.ch
#-------------------------------------------------

# Configuration file for CMake build for VMC applications.
# It defines include directories, compile definitions and link libraries
# for all required and optional packages.
# It defines (updates) PACKAGE_FOUND variables for all required/optional
# VMC packages according to selected options
# - VMC_FOUND
# - GEANT4_FOUND
# - Geant4VMC_FOUND
# - G4ROOT_FOUND
# - VGM_FOUND
# - Geant3VMC_FOUND
# - PYTHIA6_FOUND
# - MTROOT_FOUND
#
# I. Hrivnacova, 26/02/2014

#---Options---------------------------------------------------------------------
option(VMC_WITH_GEANT4   "Build with Geant4" OFF)
option(VMC_WITH_GEANT3   "Build with Geant3" OFF)
option(BUILD_SHARED_LIBS "Build the dynamic libraries" ON)

#---Find required packages------------------------------------------------------

set(VMC_FOUND FALSE)

# ROOT (required)
find_package(ROOT REQUIRED)

# Geant4 
if(VMC_WITH_GEANT4)
  option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
  if(WITH_GEANT4_UIVIS)
    find_package(Geant4 REQUIRED ui_all vis_all)
  else()
    find_package(Geant4 REQUIRED)
  endif()
  add_definitions(-DUSE_GEANT4) 

  # Geant4VMC  
  set(Geant4VMC_DIR "" CACHE PATH "Directory where Geant4VMC is installed")
  find_package(Geant4VMC REQUIRED)      

  # VGM
  if (Geant4VMC_USE_VGM)
    find_package(VGM REQUIRED)      
  endif()
endif()

# Geant3
if(VMC_WITH_GEANT3)
  find_package(Geant3VMC REQUIRED)
  #PYTHIA6
  find_package(Pythia6 REQUIRED)
endif()

# If all required packages above were found we can update VMC_FOUND
set(VMC_FOUND TRUE)
