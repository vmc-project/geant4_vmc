#------------------------------------------------
# The Virtual Monte Carlo examples
# Copyright (C) 2014 Ivana Hrivnacova
# All rights reserved.
#
# For the licensing terms see geant4_vmc/LICENSE.
# Contact: vmc@pcroot.cern.ch
#-------------------------------------------------

# - Try to find VGM
# Once done this will define
#
#  VGM_FOUND         - If VGM is found
#  VGM_INCLUDE_DIRS  - VGM include directoris
#  VGM_LIBRARIES      - The libraries needed to use VGM

#message(STATUS "Looking for VGM ...")

# Search for VGMConfig.cmake on the path defined via user setting
# VGM_DIR

if(EXISTS ${VGM_DIR}/VGMConfig.cmake)
  include(${VGM_DIR}/VGMConfig.cmake)
  # VGM_INCLUDE_DIRS, VGM_LIBRARIES
  set(VGM_FOUND TRUE)
  message(STATUS "Found VGM CMake configuration in ${VGM_DIR}")
  return()
endif()
