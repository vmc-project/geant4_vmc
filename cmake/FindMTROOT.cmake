#------------------------------------------------
# The Virtual Monte Carlo examples
# Copyright (C) 2014 Ivana Hrivnacova
# All rights reserved.
#
# For the licensing terms see geant4_vmc/LICENSE.
# Contact: vmc@pcroot.cern.ch
#-------------------------------------------------

# Adopted from Virtual Geometry Model.
# (http://ivana.home.cern.ch/ivana/VGM.html)
# I. Hrivnacova, 31/01/2014
#
# The CMake build for Virtual Geometry Model is a result of a merge 
# of the CMake configuration files kindly provided 
# by Florian Uhlig, GSI and Pere Mato, CERN.
# (http://ivana.home.cern.ch/ivana/VGM.html)

# - Try to find MTROOT
# Once done this will define
#
#  MTROOT_FOUND       - If MTROOT is found
#  MTROOT_INCLUDE_DIR - the MTROOT include directory
#  MTROOT_LIBRARIES   - The libraries needed to use MTROOT
#  MTROOT_LIBRARY_DIR - PATH to the library directory 

#message(STATUS "Looking for MTROOT ...")

# Alternative paths which can be defined by user
set(MTROOT_DIR "" CACHE PATH "Directory where MTROOT is installed")
set(MTROOT_INC_DIR "" CACHE PATH "Alternative directory for MTROOT includes")
set(MTROOT_LIB_DIR "" CACHE PATH "Alternative directory for MTROOT libraries")
set(MTROOT_ARCH "" CACHE PATH "MTROOT platform specification")

find_path(MTROOT_INCLUDE_DIR NAMES TVirtualMCRootManager.h PATHS
  ${MTROOT_INC_DIR}
  ${MTROOT_DIR}/include/mtroot
  ${Geant4VMC_DIR}/include/mtroot
)

if (NOT MTROOT_ARCH)
  if(ROOT_CONFIG_EXECUTABLE)
    execute_process(
      COMMAND ${ROOT_CONFIG_EXECUTABLE} --arch 
      OUTPUT_VARIABLE MTROOT_ARCH 
      OUTPUT_STRIP_TRAILING_WHITESPACE)
   endif()
endif()      

find_path(MTROOT_LIBRARY_DIR NAMES libmtroot.so libmtroot.a libmtroot.dylib PATHS
  ${MTROOT_LIB_DIR}
  ${MTROOT_DIR}/lib/tgt_${MTROOT_ARCH}
  ${Geant4VMC_DIR}/lib
  ${Geant4VMC_DIR}/lib/tgt_${MTROOT_ARCH}
)

if (MTROOT_INCLUDE_DIR AND MTROOT_LIBRARY_DIR)
  set(MTROOT_FOUND TRUE)
endif (MTROOT_INCLUDE_DIR AND MTROOT_LIBRARY_DIR)

if (MTROOT_FOUND)
  set(LD_LIBRARY_PATH ${LD_LIBRARY_PATH} ${MTROOT_LIBRARY_DIR})
  set(MTROOT_LIBRARIES "-L${MTROOT_LIBRARY_DIR} -lmtroot")
  if (NOT MTROOT_FIND_QUIETLY)
    message(STATUS "Found MTROOT in ${MTROOT_LIBRARY_DIR}")
  endif (NOT MTROOT_FIND_QUIETLY)
else (MTROOT_FOUND)
  if (MTROOT_FIND_REQUIRED)
    message(FATAL_ERROR "Looking for MTROOT... - Not found")
  endif (MTROOT_FIND_REQUIRED)
endif (MTROOT_FOUND)

# Make variables changeble to the advanced user
mark_as_advanced(MTROOT_INCLUDE_DIR MTROOT_LIBRARY_DIR MTROOT_LIBRARIES)

