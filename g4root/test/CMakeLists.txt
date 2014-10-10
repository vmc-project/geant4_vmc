# CMake Configuration file for G4Root test

#---Adding the OpNovice subdirectory explicitly 

cmake_minimum_required(VERSION 2.6.4 FATAL_ERROR)

set(CMAKE_MODULE_PATH 
    ${Geant4VMC_DIR}/Modules
    ${Geant3_DIR}/Modules
    ${CMAKE_MODULE_PATH}) 

add_subdirectory(OpNovice)

#add_custom_target(all DEPENDS OpNovice)
