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

#---Project G4Root -------------------------------------------------------------
#
project(Geant4VMC)

#----------------------------------------------------------------------------
# CMake Module Path
#
set(CMAKE_MODULE_PATH 
    ${Geant4VMC_DIR}/cmake
    ${CMAKE_MODULE_PATH}) 

#--- Options -------------------------------------------------------------------
option(GEANT4VMC_USE_VGM           "Build with VGM" ON)    
option(GEANT4VMC_USE_G4ROOT        "Build with G4Root" ON)    
option(GEANT4VMC_USE_SYSTEM_G4ROOT "Build with external G4Root" OFF)    
option(BUILD_SHARED_LIBS "Build the dynamic libraries" ON)  

#-- ROOT (required) ------------------------------------------------------------
if(NOT ROOT_FOUND)
  find_package(ROOT REQUIRED)
endif(NOT ROOT_FOUND)  
include_directories(${ROOT_INCLUDE_DIR})

#-- Geant4 (required) ----------------------------------------------------------
if(NOT GEANT4_FOUND)
  option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
  if(WITH_GEANT4_UIVIS)
    find_package(Geant4 REQUIRED ui_all vis_all)
  else()
    find_package(Geant4 REQUIRED)
  endif()
endif(NOT GEANT4_FOUND)  
include(${Geant4_USE_FILE})

#-- G4Root (required, to make optional) -------------------------------------------------------------
if (GEANT4VMC_USE_G4ROOT)
  if(GEANT4VMC_USE_SYSTEM_G4ROOT)
    if(NOT G4ROOT_FOUND)
      find_package(G4ROOT REQUIRED)      
    endif(NOT G4ROOT_FOUND)  
    include_directories(${G4ROOT_INCLUDE_DIR})
  else(GEANT4VMC_USE_SYSTEM_G4ROOT)
    include_directories(${PROJECT_SOURCE_DIR}/../g4root/include) 
  endif(GEANT4VMC_USE_SYSTEM_G4ROOT)  
else(GEANT4VMC_USE_G4ROOT)
  set(G4ROOT_LIBRARIES)
endif(GEANT4VMC_USE_G4ROOT)

#-- VGM (optional) -------------------------------------------------------------
if (GEANT4VMC_USE_VGM)
  if(NOT VGM_FOUND)
    find_package(VGM REQUIRED)      
  endif(NOT VGM_FOUND)  
  if (VGM_INCLUDE_DIR)
    include_directories(${VGM_INCLUDE_DIR})
  else(VGM_INCLUDE_DIR)
    include_directories(${VGM_TOP_INCLUDE_DIR}/packages/VGM/include)
    include_directories(${VGM_TOP_INCLUDE_DIR}/packages/BaseVGM/include)
    include_directories(${VGM_TOP_INCLUDE_DIR}/packages/ClhepVGM/include)
    include_directories(${VGM_TOP_INCLUDE_DIR}/packages/Geant4GM/include)
    include_directories(${VGM_TOP_INCLUDE_DIR}/packages/RootGM/include)
    include_directories(${VGM_TOP_INCLUDE_DIR}/packages/XmlVGM/include)
  endif(VGM_INCLUDE_DIR)  
  add_definitions(-DUSE_VGM)
else(GEANT4VMC_USE_VGM)
  set(VGM_LIBRARIES)
endif(GEANT4VMC_USE_VGM)

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
target_link_libraries(geant4vmc g4root ${VGM_LIBRARIES} ${Geant4_LIBRARIES} -L${GEANT4_LIBRARY_DIR} -lG3toG4 ${ROOT_LIBRARIES} -lVMC -lEG)

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
install(TARGETS geant4vmc DESTINATION lib)
