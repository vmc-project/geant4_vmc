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
  ${CMAKE_SHARED_LIBRARY_PREFIX}example06
  exampleE06Dict
  no_rootmap
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
# Install the library and dictionary map (if Root 6.x)
# to CMAKE_INSTALL_LIBDIR directory
#
if (VMC_INSTALL_EXAMPLES)
  install(TARGETS example06 DESTINATION ${CMAKE_INSTALL_LIBDIR})
  # dictionary map (only if ROOT 6.x)
  if (${ROOT_FOUND_VERSION} GREATER 59999)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/exampleE06Dict_rdict.pcm
      DESTINATION ${CMAKE_INSTALL_LIBDIR})
  endif()
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
endif(NOT MC_FOUND)

#----------------------------------------------------------------------------
# Add the executable, and link it to all libraries
#
add_executable(exampleE06 exampleE06.cxx)
add_executable(testE06 testE06.cxx)
target_link_libraries(exampleE06 example06 ${MC_LIBRARIES})
target_link_libraries(testE06 example06 ${MC_LIBRARIES})

#----------------------------------------------------------------------------
# Add program to the project targets
# (this avoids the need of typing the program name after make)
#
add_custom_target(E06 DEPENDS exampleE06 testE06)

#----------------------------------------------------------------------------
# Install the executables to 'bin'
#
if (VMC_INSTALL_EXAMPLES)
  install(TARGETS exampleE06 testE06 DESTINATION bin)
endif()
