#------------------------------------------------
# The Virtual Monte Carlo packages
# Copyright (C) 2014 Ivana Hrivnacova
# All rights reserved.
#
# For the licensing terms see geant4_vmc/LICENSE.
# Contact: root-vmc@cern.ch
#-------------------------------------------------

# Adopted from Virtual Geometry Model.
# (http://ivana.home.cern.ch/ivana/VGM.html)
# I. Hrivnacova, 31/01/2014
#
# The CMake build for Virtual Geometry Model is a result of a merge 
# of the CMake configuration files kindly provided 
# by Florian Uhlig, GSI and Pere Mato, CERN.
# (http://ivana.home.cern.ch/ivana/VGM.html)

# - Try to find Geant4 instalation
# This module sets up Geant4 information: 
# - either from Geant4 CMake configuration file (Geant4Config.cmake), if available
# - or it defines:
# Geant4_FOUND          If Geant4 is found
# Geant4_INCLUDE_DIRS   include directories for Geant4
# Geant4_LIBRARIES      Geant4 libraries to link against
# GEANT4_LIBRARY_DIR    PATH to the library directory (used to find CLHEP in old Geant4 installation)

#message(STATUS "Looking for GEANT4 ...")

# Alternative paths which can be defined by user
set(Geant4_DIR "" CACHE PATH "Directory where Geant4 is installed")
set(GEANT4_INC_DIR "" CACHE PATH "Alternative directory for Geant4 includes")
set(GEANT4_LIB_DIR "" CACHE PATH "Alternative directory for Geant4 libraries")
set(GEANT4_SYSTEM "" CACHE PATH "Geant4 platform specification")

# First search for Geant4Config.cmake on the path defined via user setting 
# Geant4_DIR

if(EXISTS ${Geant4_DIR}/Geant4Config.cmake)
  include(${Geant4_DIR}/Geant4Config.cmake)
  message(STATUS "Found Geant4 CMake configuration in ${Geant4_DIR}")
  # Geant4_INCLUDE_DIRS, Geant4_LIBRARIES are defined in Geant4Config
  set(Geant4_FOUND TRUE)
  return()
endif()

# If Geant4Config.cmake was not found in Geant4_DIR
# search for geant4-config executable on system path to get Geant4 installation directory 

find_program(GEANT4_CONFIG_EXECUTABLE geant4-config PATHS
  ${Geant4_DIR}/bin
  )

if(GEANT4_CONFIG_EXECUTABLE)
  execute_process(
    COMMAND ${GEANT4_CONFIG_EXECUTABLE} --prefix 
    OUTPUT_VARIABLE G4PREFIX 
    OUTPUT_STRIP_TRAILING_WHITESPACE)

  execute_process(
    COMMAND ${GEANT4_CONFIG_EXECUTABLE} --version 
    OUTPUT_VARIABLE GEANT4_VERSION
    OUTPUT_STRIP_TRAILING_WHITESPACE)

  if(EXISTS ${G4PREFIX}/lib/Geant4-${GEANT4_VERSION}/Geant4Config.cmake)
    set(Geant4_DIR ${G4PREFIX}/lib/Geant4-${GEANT4_VERSION})
    include(${Geant4_DIR}/Geant4Config.cmake)
    # Geant4_INCLUDE_DIRS, Geant4_LIBRARIES are defined in Geant4Config
    set(Geant4_FOUND TRUE)
    message(STATUS "Found Geant4 CMake configuration in ${Geant4_DIR}")
    return()
  endif()

  if(EXISTS ${G4PREFIX}/lib64/Geant4-${GEANT4_VERSION}/Geant4Config.cmake)
    set(Geant4_DIR ${G4PREFIX}/lib64/Geant4-${GEANT4_VERSION})
    include(${Geant4_DIR}/Geant4Config.cmake)
    # Geant4_INCLUDE_DIRS, Geant4_LIBRARIES are defined in Geant4Config
    set(Geant4_FOUND TRUE)
    message(STATUS "Found Geant4 CMake configuration in ${Geant4_DIR}")
    return()
  endif()

endif()

# If search for Geant4Config.cmake via geant4-config failed try to use directly 
# user paths if set or environment variables 
#
if (NOT Geant4_FOUND)
  find_path(Geant4_INCLUDE_DIRS NAMES globals.hh PATHS
    ${GEANT4_INC_DIR}
    ${Geant4_DIR}/include
    $ENV{G4INSTALL}/include
    $ENV{G4INCLUDE}
  )
  find_path(GEANT4_LIBRARY_DIR NAMES libname.map PATHS
    ${GEANT4_LIB_DIR}
    ${Geant4_DIR}/lib/${GEANT4_SYSTEM}
    $ENV{G4INSTALL}/lib/$ENV{G4SYSTEM}
    $ENV{G4LIB}
  )

  if (Geant4_INCLUDE_DIRS AND GEANT4_LIBRARY_DIR)
    execute_process(
      COMMAND ${GEANT4_LIBRARY_DIR}/liblist -m ${GEANT4_LIBRARY_DIR}                  
      INPUT_FILE ${GEANT4_LIBRARY_DIR}/libname.map 
      OUTPUT_VARIABLE Geant4_LIBRARIES
      OUTPUT_STRIP_TRAILING_WHITESPACE
      TIMEOUT 2)
  endif()
  set(Geant4_LIBRARIES "-L${GEANT4_LIBRARY_DIR} ${Geant4_LIBRARIES}")
endif()      

if (Geant4_INCLUDE_DIRS AND GEANT4_LIBRARY_DIR AND Geant4_LIBRARIES)
  set (Geant4_FOUND TRUE)
endif()  

if (Geant4_FOUND)
  if (NOT GEANT4_FIND_QUIETLY)
    if (G4PREFIX)
      message(STATUS "Found GEANT4 ${GEANT4_VERSION} in ${G4PREFIX}")
    else()  
      message(STATUS "Found GEANT4 includes in ${Geant4_INCLUDE_DIRS}")
      message(STATUS "Found GEANT4 libraries in ${GEANT4_LIBRARY_DIR}")
      #message(STATUS "Found GEANT4 libraries ${Geant4_LIBRARIES}")
    endif()  
  endif (NOT GEANT4_FIND_QUIETLY)  
else()
  if (Geant4_FIND_REQUIRED)
    message(FATAL_ERROR "Geant4 required, but not found")
  endif (Geant4_FIND_REQUIRED)   
endif()

# Make variables changeble to the advanced user
mark_as_advanced(Geant4_INCLUDE_DIRS)
mark_as_advanced(Geant4_LIBRARIES)
mark_as_advanced(GEANT4_LIBRARY_DIR)
mark_as_advanced(GEANT4_CONFIG_EXECUTABLE)
