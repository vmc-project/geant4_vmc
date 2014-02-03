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
# It defines (updates) following variables:
# - include_directories
# - LINK_LIBRARIES
#
# I. Hrivnacova, 31/01/2014

#---Options---------------------------------------------------------------------
option(WITH_GEANT4       "Build with Geant4" ON)
option(WITH_GEANT3       "Build with Geant3" OFF)
option(WITH_G4ROOT       "Build with G4Root" OFF)    
option(WITH_MTROOT       "Build with MtRoot" OFF)    
option(WITH_VGM          "Build with VGM" OFF)    
option(BUILD_SHARED_LIBS "Build the dynamic libraries" ON)  

#---Find required packages------------------------------------------------------

set (LINK_LIBRARIES)

# ROOT (required)
set(ROOT_DIR "" CACHE PATH "Directory where ROOT is installed")
set(ROOT_INC_DIR "" CACHE PATH "Alternative directory for ROOT includes")
set(ROOT_LIB_DIR "" CACHE PATH "Alternative directory for ROOT libraries")
find_package(ROOT REQUIRED)
include_directories(${ROOT_INCLUDE_DIR})

# Geant4 
if(WITH_GEANT4)
  set(Geant4_DIR "" CACHE PATH "Directory where Geant4Config.cmake is installed")
  set(GEANT4_DIR "" CACHE PATH "Directory where Geant4 is installed")
  set(GEANT4_INC_DIR "" CACHE PATH "Alternative directory for Geant4 includes")
  set(GEANT4_LIB_DIR "" CACHE PATH "Alternative directory for Geant4 libraries")
  set(GEANT4_SYSTEM "" CACHE PATH "Geant4 platform specification")
  option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
  if(WITH_GEANT4_UIVIS)
    find_package(Geant4 REQUIRED ui_all vis_all)
  else()
    find_package(Geant4 REQUIRED)
  endif()
  add_definitions(-DUSE_GEANT4) 
  include(${Geant4_USE_FILE})
  include_directories(${Geant4_INCLUDE_DIR})
  message(STATUS Geant4 LIB DIR ${GEANT4_LIBRARY_DIR})

  # Geant4VMC  
  set(Geant4VMC_DIR "" CACHE PATH "Directory where Geant4VMC is installed")
  set(Geant4VMC_INC_DIR "" CACHE PATH "Alternative directory for Geant4VMC includes")
  set(Geant4VMC_LIB_DIR "" CACHE PATH "Alternative directory for Geant4VMC libraries")
  set(Geant4VMC_ARCH "" CACHE PATH "Geant4VMC platform specification")
  find_package(Geant4VMC REQUIRED)      
  include_directories(${Geant4VMC_INCLUDE_DIR})
  set (LINK_LIBRARIES ${Geant4VMC_LIBRARIES} ${LINK_LIBRARIES})
  # currently G4ROOT is not optional in Geant4 VMC
  set (WITH_G4ROOT ON)
  # set use VGM according to USE_VGM environment variable
  if ($ENV{USE_VGM}) 
    set(WITH_VGM ON) 
  endif()  

  # G4ROOT
  if (WITH_G4ROOT)
    set(G4ROOT_DIR "" CACHE PATH "Directory where G4ROOT is installed")
    set(G4ROOT_INC_DIR "" CACHE PATH "Alternative directory for G4ROOT includes")
    set(G4ROOT_LIB_DIR "" CACHE PATH "Alternative directory for G4ROOT libraries")
    set(G4ROOT_ARCH "" CACHE PATH "G4ROOT platform specification")
    find_package(G4ROOT REQUIRED)      
    set (LINK_LIBRARIES ${LINK_LIBRARIES} ${G4ROOT_LIBRARIES})
  endif()

  # VGM
  if (WITH_VGM)
    set(VGM_DIR "" CACHE PATH "Directory where VGM is installed")
    set(VGM_LIB_DIR "" CACHE PATH "Alternative directory for VGM libraries")
    set(VGM_SYSTEM "" CACHE PATH "VGM platform specification")
    find_package(VGM REQUIRED)      
    set (LINK_LIBRARIES ${LINK_LIBRARIES} ${VGM_LIBRARIES})
  endif()

  set (LINK_LIBRARIES ${LINK_LIBRARIES} ${Geant4_LIBRARIES} -L${GEANT4_LIBRARY_DIR} -lG3toG4)
endif()

# Geant3
if(WITH_GEANT3)
  set(Geant3VMC_DIR "" CACHE PATH "Directory where Geant3VMC is installed")
  set(Geant3VMC_INC_DIR "" CACHE PATH "Alternative directory for Geant3VMC includes")
  set(Geant3VMC_LIB_DIR "" CACHE PATH "Alternative directory for Geant3VMC libraries")
  set(Geant3VMC_ARCH "" CACHE PATH "Geant3VMC platform specification")
  find_package(Geant3VMC REQUIRED)
  add_definitions(-DUSE_GEANT3) 
  include_directories(${Geant3VMC_INCLUDE_DIR})
  set (LINK_LIBRARIES ${Geant3VMC_LIBRARIES} ${LINK_LIBRARIES})
  
  #PYTHIA6
  find_package(Pythia6 REQUIRED)
  set (LINK_LIBRARIES ${LINK_LIBRARIES} ${Pythia6_LIBRARIES} ${Geant3VMC_LIBRARIES} )
endif()

# MTROOT
if (WITH_MTROOT)
  set(MTROOT_DIR "" CACHE PATH "Directory where MTROOT is installed")
  set(MTROOT_INC_DIR "" CACHE PATH "Alternative directory for MTROOT includes")
  set(MTROOT_LIB_DIR "" CACHE PATH "Alternative directory for MTROOT libraries")
  set(MTROOT_ARCH "" CACHE PATH "MTROOT platform specification")
  find_package(MTROOT REQUIRED)      
  include_directories(${MTROOT_INCLUDE_DIR})
  set (LINK_LIBRARIES ${MTROOT_LIBRARIES} ${LINK_LIBRARIES})
endif()

# Finally add Root libraries
set(LINK_LIBRARIES ${LINK_LIBRARIES} ${ROOT_LIBRARIES} -lVMC -lEG)
