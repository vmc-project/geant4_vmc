#------------------------------------------------
# The Virtual Monte Carlo examples
# Copyright (C) 2014 Ivana Hrivnacova
# All rights reserved.
#
# For the licensing terms see geant4_vmc/LICENSE.
# Contact: vmc@pcroot.cern.ch
#-------------------------------------------------

# Configuration file for CMake build for Geant4 VMC packages
# which defines customised installation.
#
# I. Hrivnacova, 24/04/2014

# Copy the custom cmake modules into the build tree
foreach(_mod CMakeMacroParseArguments UseVMC)
  configure_file(
    ${PROJECT_SOURCE_DIR}/cmake/${_mod}.cmake
    ${PROJECT_BINARY_DIR}/Modules/${_mod}.cmake
    COPYONLY
  )
endforeach()

# Find modules
foreach(_find_mod Geant3VMC Geant4 Pythia6 ROOT VGM VMC)
  configure_file(
    ${PROJECT_SOURCE_DIR}/cmake/Find${_find_mod}.cmake
    ${PROJECT_BINARY_DIR}/Modules/Find${_find_mod}.cmake
    COPYONLY
  )
endforeach()

# Set needed variables for the install tree
set(GEANT4VMC_CMAKE_DIR ${CMAKE_INSTALL_PREFIX}/cmake)

# Install the custom modules for the examples
install(DIRECTORY
  ${PROJECT_BINARY_DIR}/Modules/
  DESTINATION lib/Geant4VMC-${Geant4VMC_VERSION}/Modules
  COMPONENT Development
)

#
# Install the Geant4VMCConfig, Geant4VMCConfigVersion
#
configure_file(
  "${PROJECT_SOURCE_DIR}/cmake/Geant4VMCConfig.cmake.in"
  "${PROJECT_BINARY_DIR}/Geant4VMCConfig.cmake" @ONLY)

configure_file(
  "${PROJECT_SOURCE_DIR}/cmake/Geant4VMCConfigVersion.cmake.in"
  "${PROJECT_BINARY_DIR}/Geant4VMCConfigVersion.cmake" @ONLY)

install(FILES
  "${PROJECT_BINARY_DIR}/Geant4VMCConfig.cmake"
  "${PROJECT_BINARY_DIR}/Geant4VMCConfigVersion.cmake"
  DESTINATION lib/Geant4VMC-${Geant4VMC_VERSION})

install(EXPORT Geant4VMCTargets
  DESTINATION lib/Geant4VMC-${Geant4VMC_VERSION})
