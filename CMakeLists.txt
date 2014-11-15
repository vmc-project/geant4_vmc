#------------------------------------------------
# The Geant4 Virtual Monte Carlo package
# Copyright (C) 2014 Ivana Hrivnacova
# All rights reserved.
#
# For the licensing terms see geant4_vmc/LICENSE.
# Contact: vmc@pcroot.cern.ch
#-------------------------------------------------

# Configuration file for CMake build for Geant4 VMC packages.
# Inspired by Geant4 and VGM projects.
#
# I. Hrivnacova, 24/04/2014

#---  Enforce an out-of-source builds before anything else ---------------------
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(STATUS "Geant4 VMC requires an out-of-source build.")
    message(STATUS "Please remove these files from ${CMAKE_BINARY_DIR} first:")
    message(STATUS "CMakeCache.txt")
    message(STATUS "CMakeFiles")
    message(STATUS "Once these files are removed, create a separate directory")
    message(STATUS "and run CMake from there")
    message(FATAL_ERROR "in-source build detected")
endif()

#--- Define CMake requirements -------------------------------------------------
cmake_minimum_required(VERSION 2.6.4 FATAL_ERROR)

#--- Prepend our own CMake Modules to the search path --------------------------
set(CMAKE_MODULE_PATH 
    ${CMAKE_SOURCE_DIR}/cmake
    ${CMAKE_MODULE_PATH}) 

#--- Project definitions -------------------------------------------------------
project(Geant4VMCPackages)
include(Geant4VMCPackagesVersion)

#--- Options -------------------------------------------------------------------
option(Geant4VMC_BUILD_G4Root      "Build G4Root" ON)
option(Geant4VMC_BUILD_G4Root_TEST "Build G4Root test" OFF)
option(Geant4VMC_BUILD_MTRoot      "Build MTRoot" ON)
option(Geant4VMC_BUILD_Geant4VMC   "Build Geant4VMC" ON)
option(Geant4VMC_BUILD_EXAMPLES    "Build VMC examples" ON)
option(Geant4VMC_USE_G4Root        "Build with G4Root" ON)
option(Geant4VMC_USE_VGM           "Build with VGM" OFF)
option(Geant4VMC_USE_GEANT4_UI     "Build with Geant4 UI drivers" ON)
option(Geant4VMC_USE_GEANT4_VIS    "Build with Geant4 Vis drivers" ON)
option(Geant4VMC_USE_GEANT4_G3TOG4 "Build with Geant4 G3toG4 library" OFF)
option(Geant4VMC_INSTALL_EXAMPLES  "Install examples" ON)
option(BUILD_SHARED_LIBS "Build the dynamic libraries" ON)

#--- Find required packages ----------------------------------------------------
include(Geant4VMCRequiredPackages)

#--- Utility to defined installation lib directory -----------------------------
include(VMCInstallLibDir)

#--- Add the packages sources --------------------------------------------------
#
if(Geant4VMC_BUILD_G4Root)
  add_subdirectory(g4root)
  set(G4Root_BUILD_TEST ${Geant4VMC_BUILD_G4Root_TEST})
endif(Geant4VMC_BUILD_G4Root)

if(Geant4VMC_BUILD_MTRoot)
  add_subdirectory(mtroot)
endif(Geant4VMC_BUILD_MTRoot)

if(Geant4VMC_BUILD_Geant4VMC)
  add_subdirectory(source)
endif(Geant4VMC_BUILD_Geant4VMC)

if(Geant4VMC_BUILD_EXAMPLES)
  # examples require mtroot
  if(NOT Geant4VMC_BUILD_MTRoot)
    message(STATUS "Building examples inactivated")
    message(STATUS "as it requires Geant4VMC built with MTRoot.")
    message(STATUS "Please set Geant4VMC_BUILD_MTRoot option ON.")
  else()
    set(VMC_WITH_Geant4 ON)
    set(VMC_WITH_MTRoot ON)
    set(VMC_INSTALL_EXAMPLES ${Geant4VMC_INSTALL_EXAMPLES})
    set(VMC_FOUND ON)
    set(MC_FOUND ON)
    set(Geant4VMC_SOURCE_DIR ${CMAKE_SOURCE_DIR})
    add_subdirectory(examples)
  endif()
endif(Geant4VMC_BUILD_EXAMPLES)

#--- CPack ----------------- --------------------------------------------------
include(Geant4VMCPackagesCPack)
