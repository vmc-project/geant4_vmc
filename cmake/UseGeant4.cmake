#------------------------------------------------
# The Virtual Monte Carlo packages
# Copyright (C) 2014 Ivana Hrivnacova
# All rights reserved.
#
# For the licensing terms see geant4_vmc/LICENSE.
# Contact: vmc@pcroot.cern.ch
#-------------------------------------------------

# A temporary replacement of Geant4 UseGeant4.cmake 
# to avoid overriding the compiler flags.
# See: http://bugzilla-geant4.kek.jp/show_bug.cgi?id=1663
# From:  Oliver Freyermuth, Institute of Physics University of Bonn
# 01/09/2014
#

# Do not use the next line! See http://bugzilla-geant4.kek.jp/show_bug.cgi?id=1663
# include(${Geant4_USE_FILE})

add_definitions(${Geant4_DEFINITIONS})
set(CMAKE_CXX_FLAGS                "${Geant4_CXX_FLAGS} ${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG          "${Geant4_CXX_FLAGS_DEBUG} ${CMAKE_CXX_FLAGS_DEBUG}")
set(CMAKE_CXX_FLAGS_MINSIZEREL     "${Geant4_CXX_FLAGS_MINSIZEREL} ${CMAKE_CXX_FLAGS_MINSIZEREL}")
set(CMAKE_CXX_FLAGS_RELEASE        "${Geant4_CXX_FLAGS_RELEASE} ${CMAKE_CXX_FLAGS_RELEASE}")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${Geant4_CXX_FLAGS_RELWITHDEBINFO} ${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
set(CMAKE_EXE_LINKER_FLAGS         "${Geant4_EXE_LINKER_FLAGS} ${CMAKE_EXE_LINKER_FLAGS}")
include_directories(${Geant4_INCLUDE_DIRS})
list(APPEND CMAKE_MODULE_PATH ${Geant4_DIR}/Modules)
