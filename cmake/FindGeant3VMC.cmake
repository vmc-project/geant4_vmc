##------------------------------------------------
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

# - Try to find Geant3VMC
# Once done this will define
#
#  Geant3VMC_FOUND       - If Geant3VMC is found
#  Geant3VMC_INCLUDE_DIR - the Geant3VMC include directory
#  Geant3VMC_LIBRARIES   - The libraries needed to use Geant3VMC
#  Geant3VMC_LIBRARY_DIR - PATH to the library directory 

#message(STATUS "Looking for Geant3VMC ...")

# Alternative paths which can be defined by user
set(Geant3VMC_DIR "" CACHE PATH "Directory where Geant3VMC is installed")
set(Geant3VMC_INC_DIR "" CACHE PATH "Alternative directory for Geant3VMC includes")
set(Geant3VMC_LIB_DIR "" CACHE PATH "Alternative directory for Geant3VMC libraries")
set(Geant3VMC_ARCH "" CACHE PATH "Geant3VMC platform specification")

find_path(Geant3VMC_INCLUDE_DIR NAMES TGeant3.h PATHS
  ${Geant3VMC_INC_DIR}
  ${Geant3VMC_DIR}/TGeant3
)

set(Pythia6_LIBRARIES ${Pythia6_LIBRARY}) 

if (NOT Geant3VMC_ARCH)
  if(ROOT_CONFIG_EXECUTABLE)
    execute_process(
      COMMAND ${ROOT_CONFIG_EXECUTABLE} --arch 
      OUTPUT_VARIABLE Geant3VMC_ARCH 
      OUTPUT_STRIP_TRAILING_WHITESPACE)
   endif()
endif()      

find_path(Geant3VMC_LIBRARY_DIR NAMES libgeant321.so libgeant321.a libgeant321.dylib PATHS
  ${Geant3VMC_LIB_DIR}
  ${Geant3VMC_DIR}/lib/tgt_${Geant3VMC_ARCH}
)

if (Geant3VMC_INCLUDE_DIR AND Geant3VMC_LIBRARY_DIR)
  set(Geant3VMC_FOUND TRUE)
endif (Geant3VMC_INCLUDE_DIR AND Geant3VMC_LIBRARY_DIR)

if (Geant3VMC_FOUND)
  set(LD_LIBRARY_PATH ${LD_LIBRARY_PATH} ${Geant3VMC_LIBRARY_DIR})
  set(Geant3VMC_LIBRARIES "-L${Geant3VMC_LIBRARY_DIR} -lgeant321")
  if (NOT Geant3VMC_FIND_QUIETLY)
    message(STATUS "Found Geant3VMC in ${Geant3VMC_LIBRARY_DIR}")
  endif (NOT Geant3VMC_FIND_QUIETLY)
else (Geant3VMC_FOUND)
  if (Geant3VMC_FIND_REQUIRED)
    message(FATAL_ERROR "Looking for Geant3VMC... - Not found")
  endif (Geant3VMC_FIND_REQUIRED)
endif (Geant3VMC_FOUND)

# Make variables changeble to the advanced user
mark_as_advanced(Geant3VMC_INCLUDE_DIR Geant3VMC_LIBRARY_DIR Geant3VMC_LIBRARIES)

