#------------------------------------------------
# The Virtual Monte Carlo examples
# Copyright (C) 2014 Ivana Hrivnacova
# All rights reserved.
#
# For the licensing terms see geant4_vmc/LICENSE.
# Contact: root-vmc@cern.ch
#-------------------------------------------------

# CMake Configuration file for the VMC A01 example
# I. Hrivnacova, 31/01/2014

#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.6.4 FATAL_ERROR)

#----------------------------------------------------------------------------
# Project A01
#
project(A01)

#----------------------------------------------------------------------------
# Define unique names of libraries and executables based on project name
#
set(library_name vmc_${PROJECT_NAME})
set(g4library_name geant4_${PROJECT_NAME})
set(program_name example${PROJECT_NAME})
set(test_name test${PROJECT_NAME})

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
  ${library_name}_dict
  ${CMAKE_CURRENT_SOURCE_DIR}/include/A01MCApplication.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/Ex03MCStack.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/A01MagField.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/A01LocalMagField.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/A01DriftChamberHit.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/A01EmCalorHit.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/A01HadCalorHit.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/A01HodoscopeHit.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/A01DriftChamberSD.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/A01EmCalorimeterSD.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/A01HadCalorimeterSD.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/A01HodoscopeSD.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/A01PrimaryGenerator.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/A01RootDetectorConstruction.h
  MODULE ${library_name}
  LINKDEF ${CMAKE_CURRENT_SOURCE_DIR}/include/${program_name}LinkDef.h)

# Files produced by the dictionary generation
set(root_dict
  ${library_name}_dict.cxx)
set(root_dict_lib
  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}${library_name}_rdict.pcm)
#  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}${library_name}.rootmap)

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cxx)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.h)

#----------------------------------------------------------------------------
# Add the example library
#
add_library(${library_name} ${sources} ${root_dict} ${headers})
target_link_libraries(${library_name} ${VMCPackages_LIBRARIES})

#----------------------------------------------------------------------------
# Install the library and dictionary map
# to CMAKE_INSTALL_LIBDIR directory
#
if (VMC_INSTALL_EXAMPLES)
  install(TARGETS ${library_name} DESTINATION ${CMAKE_INSTALL_LIBDIR})
  install(FILES ${root_dict_lib} DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif()

#----------------------------------------------------------------------------
# MC Configuration file
# (for building MC dependent code)
#
include(UseMC)

#----------------------------------------------------------------------------
# Do not build executables if no MC is selected
#
if (NOT MCPackages_FOUND)
  return()
endif(NOT MCPackages_FOUND)

#----------------------------------------------------------------------------
# Process the example geant4 dependent library
#
if (Geant4_FOUND)
  add_subdirectory(geant4)
  include_directories(
    ${PROJECT_SOURCE_DIR}/geant4/include)
endif()

#----------------------------------------------------------------------------
# Add the executables, and link them to all libraries
#
add_executable(${MC_PREFIX}vmc_${program_name} ${program_name}.cxx)
add_executable(${MC_PREFIX}vmc_${test_name} ${test_name}.cxx)
if (Geant4_FOUND)
  target_link_libraries(${MC_PREFIX}vmc_${program_name} ${library_name} ${g4library_name} ${MCPackages_LIBRARIES})
  target_link_libraries(${MC_PREFIX}vmc_${test_name} ${library_name} ${g4library_name} ${MCPackages_LIBRARIES})
else()
  target_link_libraries(${MC_PREFIX}vmc_${program_name} ${library_name} ${MCPackages_LIBRARIES})
  target_link_libraries(${MC_PREFIX}vmc_${test_name} ${library_name} ${MCPackages_LIBRARIES})
endif()

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
