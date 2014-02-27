#------------------------------------------------
# The Virtual Monte Carlo examples
# Copyright (C) 2014 Ivana Hrivnacova
# All rights reserved.
#
# For the licensing terms see geant4_vmc/LICENSE.
# Contact: vmc@pcroot.cern.ch
#-------------------------------------------------

# CMake Configuration file for the VMC E03 test
# I. Hrivnacova, 31/01/2014

#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

#----------------------------------------------------------------------------
# Project E03
#
project(E03)

#----------------------------------------------------------------------------
# CMake Module Path
#
set(CMAKE_MODULE_PATH 
    ${Geant4VMC_DIR}/cmake
    ${CMAKE_MODULE_PATH}) 

#----------------------------------------------------------------------------
# VMC Configuration file
#
include(UseVMC)

#----------------------------------------------------------------------------
# Setup project include directories; compile definitions; link libraries
#
include_directories(
  ${PROJECT_SOURCE_DIR}/include 
  ${CMAKE_CURRENT_BINARY_DIR})

#----------------------------------------------------------------------------
# Generate Root dictionaries
#
ROOT_GENERATE_DICTIONARY(
  exampleE03Dict
  ${CMAKE_CURRENT_SOURCE_DIR}/include/Ex03MCApplication.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/Ex03MCStack.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/Ex03DetectorConstruction.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/Ex03DetectorConstructionOld.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/Ex03CalorHit.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/Ex03CalorimeterSD.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/Ex03PrimaryGenerator.h
  LINKDEF ${CMAKE_CURRENT_SOURCE_DIR}/include/example03LinkDef.h)

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cxx)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.h)

#----------------------------------------------------------------------------
# Add sources from geant4
#
if (GEANT4_FOUND)
  include_directories(
    ${PROJECT_SOURCE_DIR}/geant4/include)
  list(APPEND sources
    ${PROJECT_SOURCE_DIR}/geant4/src/B4DetectorConstruction.cxx
    ${PROJECT_SOURCE_DIR}/geant4/src/Ex03RegionConstruction.cxx
    ${PROJECT_SOURCE_DIR}/geant4/src/Ex03RunConfiguration1.cxx
    ${PROJECT_SOURCE_DIR}/geant4/src/Ex03RunConfiguration2.cxx
    ${PROJECT_SOURCE_DIR}/geant4/src/Ex03RunConfiguration3.cxx)
endif(GEANT4_FOUND)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(exampleE03 exampleE03.cxx exampleE03Dict.cxx ${sources} ${headers})
target_link_libraries(exampleE03 ${VMC_LIBRARIES})
add_executable(testE03 testE03.cxx exampleE03Dict.cxx ${sources} ${headers})
target_link_libraries(testE03 ${VMC_LIBRARIES})

#----------------------------------------------------------------------------
# Add program to the project targets
# (this avoids the need of typing the program name after make)
#
add_custom_target(E03 DEPENDS exampleE03 testE03)

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS exampleE03 testE03 DESTINATION bin)
