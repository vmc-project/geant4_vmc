#------------------------------------------------
# The Geant4 Virtual Monte Carlo package
# Copyright (C) 2007, 2008 Ivana Hrivnacova
# All rights reserved.
#
# For the licensing terms see geant4_vmc/LICENSE.
# Contact: vmc@pcroot.cern.ch
#-------------------------------------------------

# CMake Configuration file for geant4_vmc
# I. Hrivnacova, 24/04/2014

#---CMake required version -----------------------------------------------------
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

#---Project Geant4VMC/source ---------------------------------------------------  
#
project(Geant4VMCsource)

#-------------------------------------------------------------------------------
# CMake Module Path
#
set(CMAKE_MODULE_PATH 
    ${Geant4VMC_DIR}/cmake
    ${CMAKE_MODULE_PATH}) 

#--- Options -------------------------------------------------------------------
option(Geant4VMC_USE_G4ROOT        "Build with G4Root" ON)
option(Geant4VMC_USE_VGM           "Build with VGM" OFF)
option(Geant4VMC_USE_GEANT4_UI     "Build with Geant4 UI drivers" ON)
option(Geant4VMC_USE_GEANT4_VIS    "Build with Geant4 Vis drivers" ON)
option(Geant4VMC_USE_GEANT4_G3TOG4 "Build with Geant4 G3toG4 library" OFF)
option(BUILD_SHARED_LIBS "Build the dynamic libraries" ON)

#-- ROOT (required) ------------------------------------------------------------
if(NOT ROOT_FOUND)
  find_package(ROOT REQUIRED)
endif(NOT ROOT_FOUND)  
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
  if(Geant4VMC_USE_GEANT4_G3TOG4)
    list(APPEND _components g3tog4)
  endif()
  find_package(Geant4 REQUIRED ${_components})
endif(NOT Geant4_FOUND)
include(${Geant4_USE_FILE})
if (Geant4VMC_USE_GEANT4_G3TOG4)
  add_definitions(-DUSE_G3TOG4)
endif()

#-- G4Root (now required, to be made optional) ---------------------------------
if (Geant4VMC_USE_G4ROOT)
  include_directories(${PROJECT_SOURCE_DIR}/../g4root/include)
  add_definitions(-DUSE_G4ROOT)
endif(Geant4VMC_USE_G4ROOT)

#-- VGM (optional) -------------------------------------------------------------
if (Geant4VMC_USE_VGM)
  if(NOT VGM_FOUND)
    find_package(VGM REQUIRED)      
  endif(NOT VGM_FOUND)  
  include_directories(${VGM_INCLUDE_DIRS})
  add_definitions(-DUSE_VGM)
else(Geant4VMC_USE_VGM)
  set(VGM_LIBRARIES)
endif(Geant4VMC_USE_VGM)

#----------------------------------------------------------------------------
# Setup project include directories; compile definitions; link libraries
#
include_directories(
  ${PROJECT_SOURCE_DIR}/global/include 
  ${PROJECT_SOURCE_DIR}/geometry/include 
  ${PROJECT_SOURCE_DIR}/digits+hits/include 
  ${PROJECT_SOURCE_DIR}/physics/include 
  ${PROJECT_SOURCE_DIR}/physics_list/include 
  ${PROJECT_SOURCE_DIR}/event/include 
  ${PROJECT_SOURCE_DIR}/run/include 
  ${PROJECT_SOURCE_DIR}/visualization/include 
  ${CMAKE_CURRENT_BINARY_DIR})

#----------------------------------------------------------------------------
# Generate Root dictionaries
#
ROOT_GENERATE_DICTIONARY(
  geant4vmcDict
  ${CMAKE_CURRENT_SOURCE_DIR}/run/include/TG4RunConfiguration.h
  ${CMAKE_CURRENT_SOURCE_DIR}/run/include/TGeant4.h
  LINKDEF ${CMAKE_CURRENT_SOURCE_DIR}/geant4vmcLinkDef.h)

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
file(GLOB sources 
  ${PROJECT_SOURCE_DIR}/global/src/*.cxx 
  ${PROJECT_SOURCE_DIR}/geometry/src/*.cxx 
  ${PROJECT_SOURCE_DIR}/digits+hits/src/*.cxx 
  ${PROJECT_SOURCE_DIR}/physics/src/*.cxx 
  ${PROJECT_SOURCE_DIR}/physics_list/src/*.cxx 
  ${PROJECT_SOURCE_DIR}/event/src/*.cxx 
  ${PROJECT_SOURCE_DIR}/run/src/*.cxx 
  ${PROJECT_SOURCE_DIR}/visualization/src/*.cxx)
file(GLOB headers ${PROJECT_SOURCE_DIR}/*/include/*.h ${PROJECT_SOURCE_DIR}/include/*.h)

#---Add library-----------------------------------------------------------------
add_library(geant4vmc ${sources} geant4vmcDict.cxx ${headers})
# Make g4root optional
target_link_libraries(geant4vmc g4root ${VGM_LIBRARIES} ${Geant4_LIBRARIES} ${ROOT_LIBRARIES} -lVMC -lEG)

#----Installation---------------------------------------------------------------
install(DIRECTORY 
  ${PROJECT_SOURCE_DIR}/global/include/
  ${PROJECT_SOURCE_DIR}/geometry/include/ 
  ${PROJECT_SOURCE_DIR}/digits+hits/include/ 
  ${PROJECT_SOURCE_DIR}/physics/include/
  ${PROJECT_SOURCE_DIR}/physics_list/include/ 
  ${PROJECT_SOURCE_DIR}/event/include/
  ${PROJECT_SOURCE_DIR}/run/include/
  ${PROJECT_SOURCE_DIR}/visualization/include/ 
  DESTINATION include/geant4vmc)
install(TARGETS geant4vmc EXPORT Geant4VMCTargets DESTINATION ${CMAKE_INSTALL_LIBDIR})
