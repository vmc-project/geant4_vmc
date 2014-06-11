#------------------------------------------------
# The Virtual Monte Carlo examples
# Copyright (C) 2014 Ivana Hrivnacova
# All rights reserved.
#
# For the licensing terms see geant4_vmc/LICENSE.
# Contact: vmc@pcroot.cern.ch
#-------------------------------------------------

# Configuration file for CMake build for VMC applications.
# It defines include directories, compile definitions and link libraries
# (VMC_LIBRARIES) for all required and optional packages.
#
# I. Hrivnacova, 26/02/2014

#message(STATUS "Processing UseVMC.cmake")

if (NOT VMC_FOUND)
  find_package(VMC REQUIRED)
endif(NOT VMC_FOUND)

set(VMC_LIBRARIES)

if(ROOT_FOUND)
  include_directories(${ROOT_INCLUDE_DIRS})
endif(ROOT_FOUND)

if(Geant4_FOUND)
  add_definitions(-DUSE_GEANT4) 
  include(${Geant4_USE_FILE})
  
  if(Geant4VMC_FOUND)
    include_directories(${Geant4VMC_INCLUDE_DIRS})
    set(VMC_LIBRARIES ${VMC_LIBRARIES} ${Geant4VMC_LIBRARIES})
    # currently G4ROOT is not optional in Geant4 VMC
    set(VMC_WITH_G4ROOT ON)
    # set use VGM according to USE_VGM environment variable
    if(VGM_FOUND}) 
      set(VMC_WITH_VGM ON)
    endif()
  endif(Geant4VMC_FOUND)

  if(G4ROOT_FOUND)
    set(VMC_LIBRARIES ${VMC_LIBRARIES} ${G4ROOT_LIBRARIES})
  endif(G4ROOT_FOUND)  

  if(VGM_FOUND)
    set(VMC_LIBRARIES ${VMC_LIBRARIES} ${VGM_LIBRARIES})
  endif(VGM_FOUND)

  set(VMC_LIBRARIES ${VMC_LIBRARIES} ${Geant4_LIBRARIES})

endif(Geant4_FOUND)

if(Geant3VMC_FOUND)
  add_definitions(-DUSE_GEANT3) 
  include_directories(${Geant3VMC_INCLUDE_DIR})
  
  #Pythia6
  if(Pythia6_FOUND)
    set(VMC_LIBRARIES ${VMC_LIBRARIES} ${Pythia6_LIBRARIES} ${Geant3VMC_LIBRARIES})
  else()
    set(VMC_LIBRARIES ${Geant3VMC_LIBRARIES} ${VMC_LIBRARIES})
  endif(Pythia6_FOUND)
    
endif(Geant3VMC_FOUND)

# MTROOT
if (MTROOT_FOUND)
  include_directories(${MTROOT_INCLUDE_DIRS})
  set(VMC_LIBRARIES ${MTROOT_LIBRARIES} ${VMC_LIBRARIES})
endif(MTROOT_FOUND)

# Finally add Root libraries
set(VMC_LIBRARIES ${VMC_LIBRARIES} ${ROOT_LIBRARIES} -lVMC -lEG)

#message(STATUS "VMC_LIBRARIES ${VMC_LIBRARIES}")
