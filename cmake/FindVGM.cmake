#------------------------------------------------
# The Virtual Monte Carlo examples
# Copyright (C) 2014 Ivana Hrivnacova
# All rights reserved.
#
# For the licensing terms see geant4_vmc/LICENSE.
# Contact: vmc@pcroot.cern.ch
#-------------------------------------------------

# Adopted from Virtual Geometry Model and ROOT (ROOT_GENERATE_DICTIONARY)
# (http://ivana.home.cern.ch/ivana/VGM.html)
# I. Hrivnacova, 31/01/2014
#
# The CMake build for Virtual Geometry Model is a result of a merge 
# of the CMake configuration files kindly provided 
# by Florian Uhlig, GSI and Pere Mato, CERN.
# (http://ivana.home.cern.ch/ivana/VGM.html)

# - Try to find VGM
# Once done this will define
#
#  VGM_FOUND       - If VGM is found
#  VGM_LIBRARIES   - The libraries needed to use VGM
#  VGM_LIBRARY_DIR - PATH to the library directory 
#  VGM_INCLUDE_DIR     - VGM include directory
#  VGM_TOP_INCLUDE_DIR - VGM top include directories (if not one)

#message(STATUS "Looking for VGM ...")

# Alternative paths which can be defined by user
set(VGM_DIR "" CACHE PATH "Directory where VGM is installed")
set(VGM_LIB_DIR "" CACHE PATH "Alternative directory for VGM libraries")
set(VGM_INCLUDE_DIR "" CACHE PATH "Alternative directory for VGM includes")
set(VGM_TOP_INCLUDE_DIR ""
    CACHE PATH "Alternative directory for VGM includes (if not one directory)")
set(VGM_SYSTEM "" CACHE PATH "VGM platform specification")

find_path(VGM_LIBRARY_DIR NAMES libBaseVGM.so libBaseVGM.a libBaseVGM.dylib PATHS
  ${VGM_LIB_DIR}
  ${VGM_DIR}/lib/${VGM_SYSTEM}
  $ENV{VGM_INSTALL}/lib/$ENV{VGM_SYSTEM}
)

# find VGM includes
find_path(IFACTORY_INCLUDE_DIR NAMES IFactory.h PATHS
  # find includes in VGM installed via CMake
  ${VGM_DIR}/include/VGM/volumes
  $ENV{VGM_INSTALL}/include/VGM/volumes
)
if (IFACTORY_INCLUDE_DIR)
  STRING(REGEX REPLACE "/VGM/volumes" ""
         VGM_INCLUDE_DIR ${IFACTORY_INCLUDE_DIR})
else()
  # find includes in VGM installed within source (via GNUmake)
  find_path(IFACTORY_INCLUDE_DIR2 IFactory.h PATHS
    ${VGM_DIR}/packages/VGM/include/VGM/volumes
    $ENV{VGM_INSTALL}/packages/VGM/include/VGM/volumes
  )
  if (IFACTORY_INCLUDE_DIR2)
    STRING(REGEX REPLACE "/packages/VGM/include/VGM/volumes" ""
           VGM_TOP_INCLUDE_DIR ${IFACTORY_INCLUDE_DIR2})
  endif()
endif()

if (VGM_LIBRARY_DIR)
  if (VGM_INCLUDE_DIR OR VGM_TOP_INCLUDE_DIR)
    set(VGM_FOUND TRUE)
  endif (VGM_INCLUDE_DIR OR VGM_TOP_INCLUDE_DIR)
endif (VGM_LIBRARY_DIR)

if (VGM_FOUND)
  set(LD_LIBRARY_PATH ${LD_LIBRARY_PATH} ${VGM_LIBRARY_DIR})
  set(VGM_LIBRARIES 
      "-L${VGM_LIBRARY_DIR} -lGeant4GM -lRootGM -lBaseVGM -lClhepVGM -lXmlVGM")
  #message(STATUS VGM_INCLUDE_DIR ${VGM_INCLUDE_DIR})
  #message(STATUS VGM_TOP_INCLUDE_DIR ${VGM_TOP_INCLUDE_DIR})
  if (NOT VGM_FIND_QUIETLY)
    message(STATUS "Found VGM in ${VGM_LIBRARY_DIR}")
  endif (NOT VGM_FIND_QUIETLY)
else (VGM_FOUND)
  if (VGM_FIND_REQUIRED)
    message(FATAL_ERROR "Looking for VGM... - Not found")
  endif (VGM_FIND_REQUIRED)
endif (VGM_FOUND)

# Make variables changeble to the advanced user
mark_as_advanced(IFACTORY_INCLUDE_DIR IFACTORY_INCLUDE_DIR2)
mark_as_advanced(VGM_LIBRARY_DIR VGM_LIBRARIES VGM_INCLUDE_DIR VGM_TOP_INCLUDE_DIR)
