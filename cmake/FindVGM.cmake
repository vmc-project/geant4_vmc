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

#message(STATUS "Looking for VGM ...")

find_path(VGM_LIBRARY_DIR NAMES libBaseVGM.so libBaseVGM.a libBaseVGM.dylib PATHS
  ${VGM_LIB_DIR}
  ${VGM_DIR}/lib/${VGM_SYSTEM}
  $ENV{VGM_INSTALL}/lib/$ENV{VGM_SYSTEM}
)

if (VGM_LIBRARY_DIR)
  set(VGM_FOUND TRUE)
endif (VGM_LIBRARY_DIR)

if (VGM_FOUND)
  set(LD_LIBRARY_PATH ${LD_LIBRARY_PATH} ${VGM_LIBRARY_DIR})
  set(VGM_LIBRARIES 
      "-L${VGM_LIBRARY_DIR} -lGeant4GM -lRootGM -lBaseVGM -lClhepVGM -lXmlVGM")
  if (NOT VGM_FIND_QUIETLY)
    message(STATUS "Found VGM in ${VGM_LIBRARY_DIR}")
  endif (NOT VGM_FIND_QUIETLY)
else (VGM_FOUND)
  if (VGM_FIND_REQUIRED)
    message(FATAL_ERROR "Looking for VGM... - Not found")
  endif (VGM_FIND_REQUIRED)
endif (VGM_FOUND)

# Make variables changeble to the advanced user
mark_as_advanced(VGM_LIBRARY_DIR VGM_LIBRARIES)
