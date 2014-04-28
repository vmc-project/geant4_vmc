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

# - Try to find Geant4VMC
# Once done this will define
#
#  Geant4VMC_FOUND       - If Geant4VMC is found
#  Geant4VMC_INCLUDE_DIR - the Geant4VMC include directory
#  Geant4VMC_LIBRARIES   - The libraries needed to use Geant4VMC
#  Geant4VMC_LIBRARY_DIR - PATH to the library directory 

#message(STATUS "Looking for Geant4VMC ...")

# Alternative paths which can be defined by user
set(Geant4VMC_DIR "" CACHE PATH "Directory where Geant4VMC is installed")
set(Geant4VMC_INC_DIR "" CACHE PATH "Alternative directory for Geant4VMC includes")
set(Geant4VMC_LIB_DIR "" CACHE PATH "Alternative directory for Geant4VMC libraries")
set(Geant4VMC_ARCH "" CACHE PATH "Geant4VMC platform specification")

find_path(Geant4VMC_INCLUDE_DIR NAMES TGeant4.h PATHS
  ${Geant4VMC_INC_DIR}
  ${Geant4VMC_DIR}/include/geant4vmc
)

if (NOT Geant4VMC_ARCH)
  if(ROOT_CONFIG_EXECUTABLE)
    execute_process(
      COMMAND ${ROOT_CONFIG_EXECUTABLE} --arch 
      OUTPUT_VARIABLE Geant4VMC_ARCH 
      OUTPUT_STRIP_TRAILING_WHITESPACE)
   endif()
endif()      

find_path(Geant4VMC_LIBRARY_DIR NAMES libgeant4vmc.so libgeant4vmc.a libgeant4vmc.dylib PATHS
  ${Geant4VMC_LIB_DIR}
  ${Geant4VMC_DIR}/lib
  ${Geant4VMC_DIR}/lib/tgt_${Geant4VMC_ARCH}
)

if (Geant4VMC_INCLUDE_DIR AND Geant4VMC_LIBRARY_DIR)
  set(Geant4VMC_FOUND TRUE)
endif (Geant4VMC_INCLUDE_DIR AND Geant4VMC_LIBRARY_DIR)

if (Geant4VMC_FOUND)
  set(LD_LIBRARY_PATH ${LD_LIBRARY_PATH} ${Geant4VMC_LIBRARY_DIR})
  set(Geant4VMC_LIBRARIES "-L${Geant4VMC_LIBRARY_DIR} -lgeant4vmc")
  if (NOT Geant4VMC_FIND_QUIETLY)
    message(STATUS "Found Geant4VMC in ${Geant4VMC_LIBRARY_DIR}")
  endif (NOT Geant4VMC_FIND_QUIETLY)
else (Geant4VMC_FOUND)
  if (Geant4VMC_FIND_REQUIRED)
    message(FATAL_ERROR "Looking for Geant4VMC... - Not found")
  endif (Geant4VMC_FIND_REQUIRED)
endif (Geant4VMC_FOUND)

# Make variables changeble to the advanced user
mark_as_advanced(Geant4VMC_INCLUDE_DIR Geant4VMC_LIBRARY_DIR Geant4VMC_LIBRARIES)

