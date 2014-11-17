#------------------------------------------------
# The Virtual Monte Carlo examples
# Copyright (C) 2014 Ivana Hrivnacova
# All rights reserved.
#
# For the licensing terms see geant4_vmc/LICENSE.
# Contact: root-vmc@cern.ch
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
# Define unique names of libraries and executables based on project name
#
set(library_name vmc_${PROJECT_NAME})
set(g4library_name geant4_${PROJECT_NAME})
set(program_name example${PROJECT_NAME})
set(test_name test${PROJECT_NAME})

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
  ${CMAKE_SHARED_LIBRARY_PREFIX}${library_name}
  no_rootmap
  ${CMAKE_CURRENT_SOURCE_DIR}/include/Ex02MCApplication.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/Ex02MCStack.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/Ex02Particle.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/Ex02ChamberParameterisation.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/Ex02DetectorConstruction.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/Ex02DetectorConstructionOld.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/Ex02MagField.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/Ex02TrackerHit.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/Ex02TrackerSD.h
  LINKDEF ${CMAKE_CURRENT_SOURCE_DIR}/include/${program_name}LinkDef.h)

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cxx)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.h)

#----------------------------------------------------------------------------
# Add the example library
#
add_library(${library_name} ${sources} ${CMAKE_SHARED_LIBRARY_PREFIX}${library_name}_dict.cxx ${headers})
target_link_libraries(${library_name} ${VMC_LIBRARIES})

#----------------------------------------------------------------------------
# Install the library and dictionary map (if Root 6.x)
# to CMAKE_INSTALL_LIBDIR directory
#
if (VMC_INSTALL_EXAMPLES)
  install(TARGETS ${library_name} DESTINATION ${CMAKE_INSTALL_LIBDIR})
  # dictionary map (only if ROOT 6.x)
  if (${ROOT_FOUND_VERSION} GREATER 59999)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}${library_name}_dict_rdict.pcm
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
# Add the executables, and link them to all libraries
#
add_executable(${MC_PREFIX}vmc_${program_name} ${program_name}.cxx)
add_executable(${MC_PREFIX}vmc_${test_name} ${test_name}.cxx)
target_link_libraries(${MC_PREFIX}vmc_${program_name} ${library_name} ${MC_LIBRARIES})
target_link_libraries(${MC_PREFIX}vmc_${test_name} ${library_name} ${MC_LIBRARIES})

#----------------------------------------------------------------------------
# Add program to the project targets
# (this avoids the need of typing the program name after make)
#
add_custom_target(${PROJECT_NAME} DEPENDS
                  ${MC_PREFIX}vmc_${program_name} ${MC_PREFIX}vmc_${test_name})

#----------------------------------------------------------------------------
# Install the executables to 'bin'
#
if (VMC_INSTALL_EXAMPLES)
  install(TARGETS ${MC_PREFIX}vmc_${program_name} ${MC_PREFIX}vmc_${test_name}
          DESTINATION bin)
endif()
