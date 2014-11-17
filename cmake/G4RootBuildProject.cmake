#------------------------------------------------
# The Geant4 Virtual Monte Carlo package
# Copyright (C) 2014 Ivana Hrivnacova
# All rights reserved.
#
# For the licensing terms see geant4_vmc/LICENSE.
# Contact: root-vmc@cern.ch
#-------------------------------------------------

# Configuration file for CMake build for G4Root VMC package
# which defines customised installation.
#
# I. Hrivnacova, 13/06/2014

# Set needed variables for the install tree
set(G4Root_CMAKE_DIR ${CMAKE_INSTALL_PREFIX}/cmake)

#
# Install the G4RootConfig, G4RootConfigVersion
#
configure_file(
  "${PROJECT_SOURCE_DIR}/../cmake/G4RootConfig.cmake.in"
  "${PROJECT_BINARY_DIR}/G4RootConfig.cmake" @ONLY)

configure_file(
  "${PROJECT_SOURCE_DIR}/../cmake/Geant4VMCPackagesConfigVersion.cmake.in"
  "${PROJECT_BINARY_DIR}/G4RootConfigVersion.cmake" @ONLY)

install(FILES
  "${PROJECT_BINARY_DIR}/G4RootConfig.cmake"
  "${PROJECT_BINARY_DIR}/G4RootConfigVersion.cmake"
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/G4Root-${Geant4VMCPackages_VERSION})

install(EXPORT G4RootTargets
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/G4Root-${Geant4VMCPackages_VERSION})
