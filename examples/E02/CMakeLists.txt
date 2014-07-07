#------------------------------------------------
# The Virtual Monte Carlo examples
# Copyright (C) 2014 Ivana Hrivnacova
# All rights reserved.
#
# For the licensing terms see geant4_vmc/LICENSE.
# Contact: vmc@pcroot.cern.ch
#-------------------------------------------------

# CMake Configuration file for the VMC E02 example
# I. Hrivnacova, 31/01/2014

#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.6.4 FATAL_ERROR)

#----------------------------------------------------------------------------
# Project E02
#
project(E02)

#----------------------------------------------------------------------------
# CMake Module Path
# (for building MC independent code)
#
set(CMAKE_MODULE_PATH 
    ${Geant4VMC_DIR}/Modules
    ${Geant3_DIR}/Modules
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
  exampleE02Dict
  ${CMAKE_CURRENT_SOURCE_DIR}/include/Ex02MCApplication.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/Ex02MCStack.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/Ex02Particle.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/Ex02ChamberParameterisation.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/Ex02DetectorConstruction.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/Ex02DetectorConstructionOld.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/Ex02MagField.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/Ex02TrackerHit.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/Ex02TrackerSD.h
  LINKDEF ${CMAKE_CURRENT_SOURCE_DIR}/include/example02LinkDef.h)

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cxx)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.h)

#----------------------------------------------------------------------------
# Add the example library
#
add_library(example02 ${sources} exampleE02Dict.cxx ${headers})
target_link_libraries(example02 ${VMC_LIBRARIES})

#----------------------------------------------------------------------------
# MC Configuration file
# (for building MC dependent code)
#
include(UseMC)

#----------------------------------------------------------------------------
# Add the executables, and link it to all libraries
#
add_executable(exampleE02 exampleE02.cxx)
add_executable(testE02 testE02.cxx)
target_link_libraries(exampleE02 example02 ${MC_LIBRARIES})
target_link_libraries(testE02 example02 ${MC_LIBRARIES})

#----------------------------------------------------------------------------
# Add program to the project targets
# (this avoids the need of typing the program name after make)
#
add_custom_target(E02 DEPENDS exampleE02 testE02)

#----------------------------------------------------------------------------
# Install the executable to 'bin' and library to 'lib' directories
# under CMAKE_INSTALL_PREFIX
#
install(TARGETS exampleE02 testE02 DESTINATION bin)
install(TARGETS example02 DESTINATION ${CMAKE_INSTALL_LIBDIR})
