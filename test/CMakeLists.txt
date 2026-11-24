# CMake configuration for Geant4 VMC tests

add_executable(test_material_media test_material_media.cxx)
target_include_directories(test_material_media PRIVATE
  ${PROJECT_SOURCE_DIR}/source/global/include
  ${PROJECT_SOURCE_DIR}/source/geometry/include
  ${PROJECT_SOURCE_DIR}/source/physics/include
  ${PROJECT_SOURCE_DIR}/source/run/include
  ${ROOT_INCLUDE_DIRS}
  ${VMC_INCLUDE_DIRS}
  ${Geant4_INCLUDE_DIRS})
target_link_libraries(test_material_media PRIVATE geant4vmc)

add_test(NAME material_media COMMAND test_material_media)

# Test the libraries from this build even when the shell environment points
# to an older installation. Keep the existing path for external dependencies.
set(test_library_path "$<TARGET_FILE_DIR:geant4vmc>")
if(TARGET g4root)
  string(APPEND test_library_path ":$<TARGET_FILE_DIR:g4root>")
endif()
if(APPLE)
  set_tests_properties(material_media PROPERTIES ENVIRONMENT
    "DYLD_LIBRARY_PATH=${test_library_path}:$ENV{DYLD_LIBRARY_PATH}")
elseif(UNIX)
  set_tests_properties(material_media PROPERTIES ENVIRONMENT
    "LD_LIBRARY_PATH=${test_library_path}:$ENV{LD_LIBRARY_PATH}")
endif()
