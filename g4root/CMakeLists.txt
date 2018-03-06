#------------------------------------------------
# The Geant4 Virtual Monte Carlo package
# Copyright (C) 2014 - 2018 Ivana Hrivnacova
# All rights reserved.
#
# For the licensing terms see geant4_vmc/LICENSE.
# Contact: root-vmc@cern.ch
#-------------------------------------------------

# CMake Configuration file for g4root
# I. Hrivnacova, 24/04/2014

#---CMake required version -----------------------------------------------------
cmake_minimum_required(VERSION 3.3 FATAL_ERROR)

#---Project G4Root -------------------------------------------------------------
#
project(G4Root)

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
option(G4Root_BUILD_TEST           "Build G4Root test" ON)
option(Geant4VMC_USE_GEANT4_UI     "Build with Geant4 UI drivers" ON)
option(Geant4VMC_USE_GEANT4_VIS    "Build with Geant4 Vis drivers" ON)
option(BUILD_SHARED_LIBS "Build the dynamic libraries" ON)

#-- ROOT (required) ------------------------------------------------------------
if(NOT ROOT_FOUND)
  find_package(ROOT REQUIRED)
endif()  
include_directories(${ROOT_INCLUDE_DIRS})

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
  find_package(Geant4 REQUIRED ${_components})
endif()
# Workaround for upstream bug: http://bugzilla-geant4.kek.jp/show_bug.cgi?id=1663
#include(${Geant4_USE_FILE})
include(UseGeant4)

#--- Utility to defined installation lib directory -----------------------------
if("${CMAKE_INSTALL_LIBDIR}" MATCHES "")
  include(VMCInstallLibDir)
endif()

#----------------------------------------------------------------------------
# Setup project include directories; compile definitions; link libraries
#
include_directories(
  ${PROJECT_SOURCE_DIR}/include 
  ${CMAKE_CURRENT_BINARY_DIR})

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cxx)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.h)

#---Add library-----------------------------------------------------------------
add_library(g4root ${sources} ${headers})
target_link_libraries(g4root ${Geant4_LIBRARIES} ${ROOT_LIBRARIES} -lVMC -lEG)

#----Installation---------------------------------------------------------------
install(DIRECTORY include/ DESTINATION include/g4root)
install(TARGETS g4root EXPORT G4RootTargets DESTINATION ${CMAKE_INSTALL_LIBDIR})

#--- Build project configuration -----------------------------------------------
include(G4RootBuildProject)

#--- Build test ----------------------------------------------------------------
if (G4Root_BUILD_TEST)
  add_subdirectory(test)
endif(G4Root_BUILD_TEST)
