#------------------------------------------------
# The Virtual Monte Carlo examples
# Copyright (C) 2014 Ivana Hrivnacova
# All rights reserved.
#
# For the licensing terms see geant4_vmc/LICENSE.
# Contact: vmc@pcroot.cern.ch
#-------------------------------------------------

# CMake Configuration file for the VMC E03 example
# I. Hrivnacova, 31/01/2014

#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.6.4 FATAL_ERROR)

#----------------------------------------------------------------------------
# Project E03
#
project(E03)

#----------------------------------------------------------------------------
# CMake Module Path
#
set(CMAKE_MODULE_PATH 
    ${Geant4VMC_DIR}/Modules
    ${Geant3_DIR}/Modules
    ${CMAKE_MODULE_PATH}) 

#----------------------------------------------------------------------------
# VMC Configuration file
# (for building MC independent code)
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
# Add the example library
#
add_library(example03 ${sources} exampleE03Dict.cxx ${headers})
target_link_libraries(example03 ${VMC_LIBRARIES})

#----------------------------------------------------------------------------
# Install the library to CMAKE_INSTALL_LIBDIR directory
#
if (VMC_INSTALL_EXAMPLES)
  install(TARGETS example03 DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif()

#----------------------------------------------------------------------------
# MC Configuration file
# (for building MC dependent code)
#
include(UseMC)

#----------------------------------------------------------------------------
# Do not build executables if no MC is selected
#
if (NOT MC_FOUND)
  return()
endif( NOT MC_FOUND)

#----------------------------------------------------------------------------
# Process the example geant4 dependent library
#
if (Geant4_FOUND)
  add_subdirectory(geant4)
  include_directories(
    ${PROJECT_SOURCE_DIR}/geant4/include)
endif()

#----------------------------------------------------------------------------
# Add the executable, and link it to all libraries
#
add_executable(exampleE03 exampleE03.cxx)
add_executable(testE03 testE03.cxx)
if (Geant4_FOUND)
  target_link_libraries(exampleE03 example03 geant4e03 ${MC_LIBRARIES})
  target_link_libraries(testE03 example03 geant4e03 ${MC_LIBRARIES})
else()
  target_link_libraries(exampleE03 example03 ${MC_LIBRARIES})
  target_link_libraries(testE03 example03 ${MC_LIBRARIES})
endif()

#----------------------------------------------------------------------------
# Add program to the project targets
# (this avoids the need of typing the program name after make)
#
add_custom_target(E03 DEPENDS exampleE03 testE03)

#----------------------------------------------------------------------------
# Install the executables to 'bin'
#
if (VMC_INSTALL_EXAMPLES)
  install(TARGETS exampleE03 testE03 DESTINATION bin)
endif()

