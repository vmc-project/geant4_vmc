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
project(Geant4VMC)
set(${PROJECT_NAME}_VERSION_MAJOR "3")
set(${PROJECT_NAME}_VERSION_MINOR "0")
set(${PROJECT_NAME}_VERSION_PATCH "0")
set(${PROJECT_NAME}_VERSION
    ${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}.${${PROJECT_NAME}_VERSION_PATCH})

#--- Options -------------------------------------------------------------------
option(Geant4VMC_USE_VGM           "Build with VGM" ON)
option(Geant4VMC_USE_G4ROOT        "Build with G4Root" ON)
option(Geant4VMC_USE_SYSTEM_G4ROOT "Build with external G4Root" OFF)
option(BUILD_SHARED_LIBS "Build the dynamic libraries" ON)

#--- CMake module path ---------------------------------------------------------
set(CMAKE_MODULE_PATH 
    ${CMAKE_SOURCE_DIR}/cmake
    ${CMAKE_MODULE_PATH}) 

#--- Find required packages ----------------------------------------------------
include(Geant4VMCRequiredPackages)

#--- Add the packages sources --------------------------------------------------
add_subdirectory(mtroot)
add_subdirectory(g4root)
add_subdirectory(source)

#--- Build project customization =----------------------------------------------
include(Geant4VMCBuildProject)

