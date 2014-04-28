#------------------------------------------------
# The Virtual Monte Carlo examples
# Copyright (C) 2014 Ivana Hrivnacova
# All rights reserved.
#
# For the licensing terms see geant4_vmc/LICENSE.
# Contact: vmc@pcroot.cern.ch
#-------------------------------------------------

# CMake Configuration file for the VMC A01 test
# I. Hrivnacova, 31/01/2014

#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

#----------------------------------------------------------------------------
# Project A01
#
project(A01)

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
  ${VMC_INCLUDE_DIR}
  ${PROJECT_SOURCE_DIR}/include 
  ${CMAKE_CURRENT_BINARY_DIR})

#----------------------------------------------------------------------------
# Generate Root dictionaries
#
ROOT_GENERATE_DICTIONARY(
  exampleA01Dict
  ${CMAKE_CURRENT_SOURCE_DIR}/include/A01MCApplication.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/Ex03MCStack.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/A01MagField.h
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
  LINKDEF ${CMAKE_CURRENT_SOURCE_DIR}/include/exampleA01LinkDef.h)

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cxx)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.h)
file(GLOB g4sources ${PROJECT_SOURCE_DIR}/geant4/src/*.cxx)
file(GLOB g4headers ${PROJECT_SOURCE_DIR}/geant4/include/*.h)

#----------------------------------------------------------------------------
# Add the example library
#
add_library(examplea01 ${sources} exampleA01Dict.cxx ${headers})
target_link_libraries(examplea01 ${VMC_LIBRARIES})

#----------------------------------------------------------------------------
# Add the example geant4 dependent library
#
if (GEANT4_FOUND)
  include_directories(
    ${PROJECT_SOURCE_DIR}/geant4/include)
  add_library(geant4a01 ${g4sources} ${g4headers})
  target_link_libraries(geant4a01 ${VMC_LIBRARIES})
endif(GEANT4_FOUND)

#----------------------------------------------------------------------------
# Add the executable, and link it to all libraries
#
add_executable(exeA01 exampleA01.cxx)
add_executable(testA01 testA01.cxx)
if (GEANT4_FOUND)
  target_link_libraries(exeA01 examplea01 geant4a01 ${VMC_LIBRARIES})
  target_link_libraries(testA01 examplea01 geant4a01  ${VMC_LIBRARIES})
else()
  target_link_libraries(exeA01 examplea01 ${VMC_LIBRARIES})
  target_link_libraries(testA01 examplea01 ${VMC_LIBRARIES})
endif()

#----------------------------------------------------------------------------
# Add program to the project targets
# (this avoids the need of typing the program name after make)
#
add_custom_target(A01 DEPENDS exeA01 testA01)

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS exeA01 testA01 DESTINATION bin)
install(TARGETS examplea01 DESTINATION lib)
if (GEANT4_FOUND)
  install(TARGETS geant4a01 DESTINATION lib)
endif(GEANT4_FOUND)
