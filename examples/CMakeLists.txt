#------------------------------------------------
# The Virtual Monte Carlo examples
# Copyright (C) 2014 - 2015 Ivana Hrivnacova
# All rights reserved.
#
# For the licensing terms see geant4_vmc/LICENSE.
# Contact: root-vmc@cern.ch
#-------------------------------------------------

# CMake Configuration file for the VMC examples
# I. Hrivnacova, 31/01/2014

#---Adding examples subdirectories explicitly
#   and a custom target to for building all VMC examples -------------

cmake_minimum_required(VERSION 2.6.4 FATAL_ERROR)

set(CMAKE_MODULE_PATH
    ${Geant4VMC_DIR}/Modules
    ${Geant3_DIR}/Modules
    ${CMAKE_MODULE_PATH})

if(VMC_WITH_Multi)
add_subdirectory(E03)
else()
add_subdirectory(E01)
add_subdirectory(E02)
add_subdirectory(E03)
add_subdirectory(E06)
add_subdirectory(A01)
add_subdirectory(Gflash)
add_subdirectory(Monopole)
add_subdirectory(TR)

#----------------------------------------------------------------------------
# Garfield example requires Garfield
#
find_package(Garfield QUIET)
if(Garfield_FOUND)
  add_subdirectory(ExGarfield)
else()
  message(STATUS "VMC Examples: Garfield package not found. ")
  message(STATUS "ExGarfield example disabled")
endif()
endif()
