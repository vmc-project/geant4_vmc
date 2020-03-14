#------------------------------------------------
# The Virtual Monte Carlo examples
# Copyright (C) 2015 - 2016 Ivana Hrivnacova
# All rights reserved.
#
# For the licensing terms see geant4_vmc/LICENSE.
# Contact: root-vmc@cern.ch
#-------------------------------------------------

# CMake Configuration file for the VMC ExGarfield example Geant4 based code
# I. Hrivnacova, 28/04/2015

#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.6.4 FATAL_ERROR)

#----------------------------------------------------------------------------
# Project geant4ExGarfield
#
project(geant4ExGarfield)

#----------------------------------------------------------------------------
# CMake Module Path
#
set(CMAKE_MODULE_PATH
    ${Geant4VMC_DIR}/Modules
    ${CMAKE_MODULE_PATH})

#----------------------------------------------------------------------------
# MC Configuration file
# (for building MC dependent code)
#
include(UseMC)

#----------------------------------------------------------------------------
# Garfield package
#
find_package(Garfield)


#----------------------------------------------------------------------------
# Setup project include directories; compile definitions; link libraries
#
include_directories(
  ${PROJECT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_BINARY_DIR}
  ${Garfield_INCLUDE_DIRS})

#----------------------------------------------------------------------------
# Generate Root dictionaries
#
ROOT_GENERATE_DICTIONARY(
  ${g4library_name}_dict
  ${CMAKE_CURRENT_SOURCE_DIR}/include/RunConfiguration.h
  MODULE ${g4library_name}
  LINKDEF ${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}LinkDef.h)

# Files produced by the dictionary generation
set(root_dict
  ${g4library_name}_dict.cxx)
set(root_dict_lib
  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}${g4library_name}_rdict.pcm)
#  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}${g4library_name}.rootmap)

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cxx)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.h)

#----------------------------------------------------------------------------
# Add the example library
#
add_library(${g4library_name} ${sources} ${root_dict} ${headers})
target_link_libraries(${g4library_name} ${library_name} ${VMCPackages_LIBRARIES} ${MCPackages_LIBRARIES} ${Garfield_LIBRARIES})

#----------------------------------------------------------------------------
# Add program to the project targets
# (this avoids the need of typing the program name after make)
#
add_custom_target(${PROJECT_NAME} DEPENDS ${g4library_name})

#----------------------------------------------------------------------------
# Install the library and dictionary map
# to CMAKE_INSTALL_LIBDIR directory
#
if (VMC_INSTALL_EXAMPLES)
  install(TARGETS ${g4library_name} DESTINATION ${CMAKE_INSTALL_LIBDIR})
  install(FILES ${root_dict_lib} DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif()
