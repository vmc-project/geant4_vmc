#------------------------------------------------
# The Virtual Monte Carlo tests
# Copyright (C) 2014 Ivana Hrivnacova
# All rights reserved.
#
# For the licensing terms see geant4_vmc/LICENSE.
# Contact: vmc@pcroot.cern.ch
#-------------------------------------------------

# CMake Configuration file for the VMC E01 example
# I. Hrivnacova, 31/01/2014

#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.6.4 FATAL_ERROR)

#----------------------------------------------------------------------------
# Project E01
#
project(E01)

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
  exampleE01Dict
  ${CMAKE_CURRENT_SOURCE_DIR}/include/Ex01MCApplication.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/Ex01MCStack.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/Ex01DetectorConstructionOld.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/Ex01Particle.h
  LINKDEF ${CMAKE_CURRENT_SOURCE_DIR}/include/example01LinkDef.h)

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cxx)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.h)

#----------------------------------------------------------------------------
# Add the example library
#
add_library(example01 ${sources} exampleE01Dict.cxx ${headers})
target_link_libraries(example01 ${VMC_LIBRARIES})

#----------------------------------------------------------------------------
# Add the executables, and link it to all libraries
#
add_executable(exampleE01 exampleE01.cxx)
add_executable(testE01 testE01.cxx)
target_link_libraries(exampleE01 example01 ${VMC_LIBRARIES})
target_link_libraries(testE01 example01 ${VMC_LIBRARIES})

#----------------------------------------------------------------------------
# Add program to the project targets
# (this avoids the need of typing the program name after make)
#
add_custom_target(E01 DEPENDS exampleE01 testE01)

#----------------------------------------------------------------------------
# Install the executable to 'bin' and library to 'lib' directories
# under CMAKE_INSTALL_PREFIX
#
install(TARGETS exampleE01 testE01  DESTINATION bin)
install(TARGETS example01 DESTINATION ${CMAKE_INSTALL_LIBDIR})
