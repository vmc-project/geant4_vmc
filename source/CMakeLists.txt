#------------------------------------------------
# The Geant4 Virtual Monte Carlo package
# Copyright (C) 2014 - 2018 Ivana Hrivnacova
# All rights reserved.
#
# For the licensing terms see geant4_vmc/LICENSE.
# Contact: root-vmc@cern.ch
#-------------------------------------------------

# CMake Configuration file for geant4_vmc
# I. Hrivnacova, 24/04/2014

#---CMake required version -----------------------------------------------------
cmake_minimum_required(VERSION 3.3 FATAL_ERROR)

#---Project Geant4VMC  ---------------------------------------------------------
#
project(Geant4VMC)

#----------------------------------------------------------------------------
# Define installed names
#
set(library_name geant4vmc)

#-------------------------------------------------------------------------------
# CMake Module Path
#
set(CMAKE_MODULE_PATH
    ${Geant4VMC_DIR}/cmake
    ${PROJECT_SOURCE_DIR}/../cmake
    ${CMAKE_MODULE_PATH})

#--- Project version -----------------------------------------------------------
include(Geant4VMCPackagesVersion)

#--- Default project build mode ------------------------------------------------
include(VMCBuildMode)

#--- Options -------------------------------------------------------------------
option(Geant4VMC_USE_G4Root        "Build with G4Root" ON)
option(Geant4VMC_USE_VGM           "Build with VGM" OFF)
option(Geant4VMC_USE_GEANT4_UI     "Build with Geant4 UI drivers" ON)
option(Geant4VMC_USE_GEANT4_VIS    "Build with Geant4 Vis drivers" ON)
option(Geant4VMC_USE_GEANT4_G3TOG4 "Build with Geant4 G3toG4 library" OFF)
option(BUILD_SHARED_LIBS "Build the dynamic libraries" ON)

# Derived option
set(Geant4VMC_USE_EXTERN_G4Root OFF)
if ((NOT Geant4VMC_BUILD_G4Root) AND Geant4VMC_USE_G4Root)
  set(Geant4VMC_USE_EXTERN_G4Root ON)
endif()
mark_as_advanced(Geant4VMC_USE_EXTERN_G4Root)

#-- Includes --------------------------------------------------------------------
include_directories(${ROOT_INCLUDE_DIRS})
include_directories(${VMC_INCLUDE_DIRS})

#-- Geant4 (required) ----------------------------------------------------------
if(NOT Geant4_FOUND)
  #  find_package(Geant4 REQUIRED ui_all vis_all )
  set(_components)
  if(Geant4VMC_USE_GEANT4_UI)
    list(APPEND _components ui_all)
  endif()
  if(Geant4VMC_USE_GEANT4_VIS)
    list(APPEND _components vis_all)
  endif()
  if(Geant4VMC_USE_GEANT4_G3TOG4)
    list(APPEND _components g3tog4)
  endif()
  find_package(Geant4 CONFIG REQUIRED ${_components})
endif(NOT Geant4_FOUND)
# Workaround for upstream bug: http://bugzilla-geant4.kek.jp/show_bug.cgi?id=1663
#include(${Geant4_USE_FILE})
include(UseGeant4)
if (Geant4VMC_USE_GEANT4_G3TOG4)
  add_definitions(-DUSE_G3TOG4)
endif()

#-- G4Root ---------------------------------------------------------------------
if (Geant4VMC_USE_G4Root)
  if (Geant4VMC_BUILD_G4Root)
    include_directories(${PROJECT_SOURCE_DIR}/../g4root/include)
    set(G4Root_LIBRARIES g4root)
  else()
    if(NOT G4Root_FOUND)
      find_package(G4Root CONFIG REQUIRED)
    endif()
    include_directories(${G4Root_INCLUDE_DIRS})
  endif()
  add_definitions(-DUSE_G4ROOT)
endif(Geant4VMC_USE_G4Root)

#-- VGM (optional) -------------------------------------------------------------
if (Geant4VMC_USE_VGM)
  if(NOT VGM_FOUND)
    find_package(VGM CONFIG REQUIRED)
  endif(NOT VGM_FOUND)
  include_directories(${VGM_INCLUDE_DIRS})
  add_definitions(-DUSE_VGM)
else(Geant4VMC_USE_VGM)
  set(VGM_LIBRARIES)
endif(Geant4VMC_USE_VGM)

#--- Utility to defined installation lib directory -----------------------------
if("${CMAKE_INSTALL_LIBDIR}" MATCHES "")
  include(VMCInstallLibDir)
endif()

#----------------------------------------------------------------------------
# Setup project include directories; compile definitions; link libraries
#
include_directories(
  ${PROJECT_SOURCE_DIR}/biasing/include
  ${PROJECT_SOURCE_DIR}/global/include
  ${PROJECT_SOURCE_DIR}/geometry/include
  ${PROJECT_SOURCE_DIR}/digits+hits/include
  ${PROJECT_SOURCE_DIR}/physics/include
  ${PROJECT_SOURCE_DIR}/physics_list/include
  ${PROJECT_SOURCE_DIR}/physics_monopole/include
  ${PROJECT_SOURCE_DIR}/event/include
  ${PROJECT_SOURCE_DIR}/run/include
  ${PROJECT_SOURCE_DIR}/visualization/include
  ${CMAKE_CURRENT_BINARY_DIR})

#----------------------------------------------------------------------------
# Generate Root dictionaries
#
ROOT_GENERATE_DICTIONARY(
  ${library_name}_dict
  TG4RunConfiguration.h
  TGeant4.h
  MODULE ${library_name}
  OPTIONS "-I${CMAKE_INSTALL_PREFIX}/include/${library_name}"
    -excludePath "${CMAKE_CURRENT_BINARY_DIR}"
    -excludePath "${PROJECT_SOURCE_DIR}"
  LINKDEF ${library_name}LinkDef.h)

# Files produced by the dictionary generation
SET(root_dict
  ${library_name}_dict.cxx)
SET(root_dict_libs
  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}${library_name}_rdict.pcm
  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}${library_name}.rootmap)

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
file(GLOB sources
  ${PROJECT_SOURCE_DIR}/biasing/src/*.cxx
  ${PROJECT_SOURCE_DIR}/global/src/*.cxx
  ${PROJECT_SOURCE_DIR}/geometry/src/*.cxx
  ${PROJECT_SOURCE_DIR}/digits+hits/src/*.cxx
  ${PROJECT_SOURCE_DIR}/physics/src/*.cxx
  ${PROJECT_SOURCE_DIR}/physics_list/src/*.cxx
  ${PROJECT_SOURCE_DIR}/physics_monopole/src/*.cxx
  ${PROJECT_SOURCE_DIR}/event/src/*.cxx
  ${PROJECT_SOURCE_DIR}/run/src/*.cxx
  ${PROJECT_SOURCE_DIR}/visualization/src/*.cxx)
file(GLOB headers ${PROJECT_SOURCE_DIR}/*/include/*.h ${PROJECT_SOURCE_DIR}/include/*.h)

#---Add library-----------------------------------------------------------------
add_library(${library_name} ${sources} ${root_dict} ${headers})
# Make g4root optional
target_link_libraries(${library_name} ${G4Root_LIBRARIES}
  ${VGM_LIBRARIES} ${VMC_LIBRARIES} ${Geant4_LIBRARIES} ${ROOT_DEPS})
target_include_directories(${library_name} INTERFACE $<INSTALL_INTERFACE:include/${library_name}>)

#----Installation---------------------------------------------------------------
install(DIRECTORY
  ${PROJECT_SOURCE_DIR}/global/include/
  ${PROJECT_SOURCE_DIR}/geometry/include/
  ${PROJECT_SOURCE_DIR}/digits+hits/include/
  ${PROJECT_SOURCE_DIR}/physics/include/
  ${PROJECT_SOURCE_DIR}/physics_list/include/
  ${PROJECT_SOURCE_DIR}/physics_monopole/include/
  ${PROJECT_SOURCE_DIR}/event/include/
  ${PROJECT_SOURCE_DIR}/run/include/
  ${PROJECT_SOURCE_DIR}/visualization/include/
  DESTINATION include/${library_name})
install(TARGETS ${library_name} EXPORT Geant4VMCTargets 
  DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(FILES ${root_dict_libs} 
  DESTINATION ${CMAKE_INSTALL_LIBDIR})

#--- Build project configuration -----------------------------------------------
include(Geant4VMCBuildProject)
