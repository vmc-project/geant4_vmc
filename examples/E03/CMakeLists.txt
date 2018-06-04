#------------------------------------------------
# The Virtual Monte Carlo examples
# Copyright (C) 2018 Ivana Hrivnacova
# All rights reserved.
#
# For the licensing terms see geant4_vmc/LICENSE.
# Contact: root-vmc@cern.ch
#-------------------------------------------------

# CMake Configuration file for the VMC E03 examples
# I. Hrivnacova, 04/06/2018

#---Adding examples subdirectories explicitly 
#   and a custom target to for building all VMC examples -------------

cmake_minimum_required(VERSION 2.6.4 FATAL_ERROR)

add_subdirectory(E03a)
add_subdirectory(E03b)

#----------------------------------------------------------------------------
# E03b example requires ROOT version >= 6.13/04
#
#find_package(ROOT QUIET)
#message(STATUS "Root version ${ROOT_FOUND_VERSION}")
#if (${ROOT_FOUND_VERSION} GREATER "61302" )
#  add_subdirectory(E03b)
#else()
#  message(STATUS "VMC Examples: E03b example disabled")
#endif()
