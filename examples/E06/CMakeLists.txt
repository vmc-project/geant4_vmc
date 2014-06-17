#------------------------------------------------
# The Virtual Monte Carlo examples
# Copyright (C) 2014 Ivana Hrivnacova
# All rights reserved.
#
# For the licensing terms see geant4_vmc/LICENSE.
# Contact: vmc@pcroot.cern.ch
#-------------------------------------------------

# CMake Configuration file for the VMC E06 example
# I. Hrivnacova, 31/01/2014

#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.6.4 FATAL_ERROR)

#----------------------------------------------------------------------------
# Project E06
#
project(E06)

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
  exampleE06Dict
  ${CMAKE_CURRENT_SOURCE_DIR}/include/Ex06MCApplication.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/Ex03MCStack.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/Ex06DetectorConstruction.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/Ex06DetectorConstructionOld.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/Ex06PrimaryGenerator.h
  LINKDEF ${CMAKE_CURRENT_SOURCE_DIR}/include/example06LinkDef.h)

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cxx)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.h)

#----------------------------------------------------------------------------
# Add the example library
#
add_library(example06 ${sources} exampleE06Dict.cxx ${headers})
target_link_libraries(example06 ${VMC_LIBRARIES})

#----------------------------------------------------------------------------
# Add the executable, and link it to all libraries
#
add_executable(exampleE06 exampleE06.cxx)
add_executable(testE06 testE06.cxx)
target_link_libraries(exampleE06 example06 ${VMC_LIBRARIES})
target_link_libraries(testE06 example06 ${VMC_LIBRARIES})

#----------------------------------------------------------------------------
# Add program to the project targets
# (this avoids the need of typing the program name after make)
#
add_custom_target(E06 DEPENDS exampleE06 testE06)

#----------------------------------------------------------------------------
# Install the executable to 'bin' and library to 'lib' directories
# under CMAKE_INSTALL_PREFIX
#
install(TARGETS exampleE06 testE06 DESTINATION bin)
install(TARGETS example06 DESTINATION ${CMAKE_INSTALL_LIBDIR})
