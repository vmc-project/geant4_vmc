#------------------------------------------------
# The Geant4 Virtual Monte Carlo package
# Copyright (C) 2014 - 2018 Ivana Hrivnacova
# All rights reserved.
#
# For the licensing terms see geant4_vmc/LICENSE.
# Contact: root-vmc@cern.ch
#-------------------------------------------------

# CMake Configuration file for mtroot
# I. Hrivnacova, 24/04/2014

#---CMake required version -----------------------------------------------------
cmake_minimum_required(VERSION 3.3 FATAL_ERROR)

#---Project MTRoot -------------------------------------------------------------
#
project(MTRoot)

#-------------------------------------------------------------------------------
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
option(BUILD_SHARED_LIBS "Build the dynamic libraries" ON)  

#-- ROOT (required) ------------------------------------------------------------
if(NOT ROOT_FOUND)
  find_package(ROOT REQUIRED)
endif()  
include_directories(${ROOT_INCLUDE_DIRS})

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
# Set sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
set(sources
    ${PROJECT_SOURCE_DIR}/src/TMCRootManager.cxx
    )

set(headers
    ${PROJECT_SOURCE_DIR}/include/TMCRootManager.h
    )

#---Add library-----------------------------------------------------------------
add_library(mtroot ${sources} ${headers})
target_link_libraries(mtroot ${ROOT_LIBRARIES})

#----Installation---------------------------------------------------------------
#install(DIRECTORY include/ DESTINATION include/mtroot)
install(FILES
        include/TMCRootManager.h
        DESTINATION include/mtroot)

install(TARGETS mtroot EXPORT MTRootTargets DESTINATION ${CMAKE_INSTALL_LIBDIR})

#--- Build project configuration -----------------------------------------------
include(MTRootBuildProject)
