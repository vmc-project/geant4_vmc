#------------------------------------------------
# The Virtual Monte Carlo examples
# Copyright (C) 2014 Ivana Hrivnacova
# All rights reserved.
#
# For the licensing terms see geant4_vmc/LICENSE.
# Contact: vmc@pcroot.cern.ch
#-------------------------------------------------

# Configuration file for CMake build for Geant4 VMC packages.
# Ispired by Geant4 and VGM projects.
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
set(${PROJECT_NAME}_VERSION_MAJOR "3")
set(${PROJECT_NAME}_VERSION_MINOR "0")
set(${PROJECT_NAME}_VERSION_PATCH "0")
set(${PROJECT_NAME}_VERSION
    ${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}.${${PROJECT_NAME}_VERSION_PATCH})

#--- Options -------------------------------------------------------------------
option(Geant4VMC_BUILD_G4Root      "Build G4Root" ON)
option(Geant4VMC_BUILD_MTRoot      "Build MTRoot" ON)
option(Geant4VMC_BUILD_Geant4VMC   "Build Geant4VMC" ON)
option(Geant4VMC_USE_VGM           "Build with VGM" OFF)
option(Geant4VMC_USE_GEANT4_UI     "Build with Geant4 UI drivers" ON)
option(Geant4VMC_USE_GEANT4_VIS    "Build with Geant4 Vis drivers" ON)
option(Geant4VMC_USE_GEANT4_G3TOG4 "Build with Geant4 G3toG4 library" OFF)
option(BUILD_SHARED_LIBS "Build the dynamic libraries" ON)

#--- Find required packages ----------------------------------------------------
include(Geant4VMCRequiredPackages)

#--- Utility to defined installation lib directory -----------------------------
include(Geant4VMCInstallLibDir)

#--- Add the packages sources --------------------------------------------------
#
if(Geant4VMC_BUILD_G4Root)
  add_subdirectory(g4root)
  set(Geant4VMC_USE_G4Root ON)
endif(Geant4VMC_BUILD_G4Root)

if(Geant4VMC_BUILD_MTRoot)
  add_subdirectory(mtroot)
endif(Geant4VMC_BUILD_MTRoot)

if(Geant4VMC_BUILD_Geant4VMC)
  add_subdirectory(source)
endif(Geant4VMC_BUILD_Geant4VMC)

#--- Build project configuration -----------------------------------------------
#include(Geant4VMCBuildProject)

