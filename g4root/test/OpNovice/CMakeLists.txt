#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.6.4 FATAL_ERROR)
project(OpNovice)

#----------------------------------------------------------------------------
# Define unique names of libraries and executables based on project name
#
set(program_name g4root_${PROJECT_NAME})

#----------------------------------------------------------------------------
# Add path to Find modules in Geant4 VMC installation
set(CMAKE_MODULE_PATH 
    ${Geant4VMC_DIR}/Modules
    ${CMAKE_MODULE_PATH}) 

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

#----------------------------------------------------------------------------
# Find ROOT (required)
find_package(ROOT REQUIRED)

#----------------------------------------------------------------------------
# Find G4Root(required)
if (NOT G4Root_BUILD_TEST)
  # build outside G4Root
  find_package(G4Root REQUIRED)
else()
  # build inside G4Root
  include_directories(${G4Root_SOURCE_DIR}/include)
  set(G4Root_LIBRARIES g4root)
endif()

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
#
include(${Geant4_USE_FILE})

#----------------------------------------------------------------------------
# Locate sources and headers for this project
#
include_directories(${PROJECT_SOURCE_DIR}/include 
                    ${Geant4_INCLUDE_DIR}
                    ${ROOT_INCLUDE_DIRS}
                    ${G4Root_INCLUDE_DIRS})
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(${program_name} OpNovice.cc ${sources} ${headers})
target_link_libraries(${program_name} ${Geant4_LIBRARIES} ${G4Root_LIBRARIES} ${ROOT_LIBRARIES} )

#----------------------------------------------------------------------------
# Copy all scripts and data files to the build directory, i.e. the directory in which we
# build OpNovice. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
set(OpNovice_SCRIPTS
    OpNoviceGeom.root
    OpNoviceGeom.C
    drawTracks.C 
    OpNovice.out
    OpNovice.in
    optPhoton.mac
    gui.mac
    icons.mac
    run.png
    vis.mac
  )

foreach(_script ${OpNovice_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS ${program_name} DESTINATION bin)

