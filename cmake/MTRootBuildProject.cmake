#------------------------------------------------
# The Geant4 Virtual Monte Carlo package
# Copyright (C) 2014 Ivana Hrivnacova
# All rights reserved.
#
# For the licensing terms see geant4_vmc/LICENSE.
# Contact: vmc@pcroot.cern.ch
#-------------------------------------------------

# Configuration file for CMake build for MTRoot VMC package
# which defines customised installation.
#
# I. Hrivnacova, 13/06/2014

# Set needed variables for the install tree
set(MTRoot_CMAKE_DIR ${CMAKE_INSTALL_PREFIX}/cmake)

#
# Install the MTRootConfig, MTRootConfigVersion
#
configure_file(
  "${PROJECT_SOURCE_DIR}/../cmake/MTRootConfig.cmake.in"
  "${PROJECT_BINARY_DIR}/MTRootConfig.cmake" @ONLY)

configure_file(
  "${PROJECT_SOURCE_DIR}/../cmake/Geant4VMCPackagesConfigVersion.cmake.in"
  "${PROJECT_BINARY_DIR}/MTRootConfigVersion.cmake" @ONLY)

install(FILES
  "${PROJECT_BINARY_DIR}/MTRootConfig.cmake"
  "${PROJECT_BINARY_DIR}/MTRootConfigVersion.cmake"
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/MTRoot-${Geant4VMCPackages_VERSION})

install(EXPORT MTRootTargets
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/MTRoot-${Geant4VMCPackages_VERSION})
