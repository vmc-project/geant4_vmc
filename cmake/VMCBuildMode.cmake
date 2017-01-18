#------------------------------------------------
# The Geant4 Virtual Monte Carlo package
# Copyright (C) 2016 Ivana Hrivnacova
# All rights reserved.
#
# For the licensing terms see geant4_vmc/LICENSE.
# Contact: root-vmc@cern.ch
#-------------------------------------------------

# Configuration file for CMake build for Geant4 VMC packages
# which defines the default build mode if it has not been set
#
# I. Hrivnacova, 11/05/2016

#-------------------------------------------------------------------------------
# Make sure a default build type is used
#
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING
      "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel."
       FORCE)
endif(NOT CMAKE_BUILD_TYPE)

#-------------------------------------------------------------------------------
# Always use '@rpath' in install names of libraries.
#
set(CMAKE_MACOSX_RPATH 1)
