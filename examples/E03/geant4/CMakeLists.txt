#------------------------------------------------
# The Virtual Monte Carlo examples
# Copyright (C) 2014 Ivana Hrivnacova
# All rights reserved.
#
# For the licensing terms see geant4_vmc/LICENSE.
# Contact: vmc@pcroot.cern.ch
#-------------------------------------------------

# CMake Configuration file for the VMC E03 example Geant4 based code
# I. Hrivnacova, 28/04/2014

#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

#----------------------------------------------------------------------------
# Project g4E03
#
project(g4E03)

#----------------------------------------------------------------------------
# CMake Module Path
#
set(CMAKE_MODULE_PATH 
    ${Geant4VMC_DIR}/Modules
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
  geant4e03Dict
  ${CMAKE_CURRENT_SOURCE_DIR}/include/Ex03RunConfiguration1.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/Ex03RunConfiguration2.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/Ex03RunConfiguration3.h
  LINKDEF ${CMAKE_CURRENT_SOURCE_DIR}/include/geant4e03LinkDef.h)

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
file(GLOB sources geant4e03Dict.cxx ${PROJECT_SOURCE_DIR}/src/*.cxx)
file(GLOB headers geant4e03Dict.h ${PROJECT_SOURCE_DIR}/include/*.h)

#----------------------------------------------------------------------------
# Add the example library
#
add_library(geant4e03 ${sources} geant4e03Dict.cxx ${headers})
target_link_libraries(geant4e03 ${VMC_LIBRARIES})

#----------------------------------------------------------------------------
# Add program to the project targets
# (this avoids the need of typing the program name after make)
#
add_custom_target(g4E03 DEPENDS geant4e03)

#----------------------------------------------------------------------------
# Install the library to 'lib' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS geant4e03 DESTINATION ${CMAKE_INSTALL_LIBDIR})
