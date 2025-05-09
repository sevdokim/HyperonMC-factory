#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)

#----------------------------------------------------------------------------
# Project Hyp_py8
#
project(Hyp_py8)

#----------------------------------------------------------------------------
# Define unique names of libraries and executables based on project name
#
set(library_name vmc_${PROJECT_NAME})
set(g4library_name geant4_${PROJECT_NAME})
set(program_name ${PROJECT_NAME})
set(test_name test${PROJECT_NAME})

#----------------------------------------------------------------------------
# CMake Module Path
#
set(CMAKE_MODULE_PATH
    ${G4Root_DIR}/Modules
    ${Geant4VMC_DIR}/Modules
    ${Geant3_DIR}/Modules
    ${CMAKE_MODULE_PATH})

#----------------------------------------------------------------------------
# VMC Configuration file
# (for building MC independent code)
#
include(UseVMC)

#----------------------------------------------------------------------------
# Setup project include directories; compile definitions; link libraries
#
include_directories(
  ${PROJECT_SOURCE_DIR}/include
  ${PYTHIA8}/include
  ${CMAKE_CURRENT_BINARY_DIR})

#----------------------------------------------------------------------------
# Generate Root dictionaries
#
ROOT_GENERATE_DICTIONARY(
  ${library_name}_dict
  HyMCApplication.h
  HyMCStack.h
  HyDetectorConstruction.h
  HyParticle.h
  MODULE ${library_name}
  LINKDEF include/HyMCLinkDef.h)

# Files produced by the dictionary generation
set(root_dict
  ${library_name}_dict.cxx)
set(root_dict_lib
  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}${library_name}_rdict.pcm)
set(root_map
  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}${library_name}.rootmap)

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cxx)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.h)

#----------------------------------------------------------------------------
# Add the example library
#
add_library(${library_name} ${sources} ${root_dict} ${headers})
target_link_libraries(${library_name} ${VMCPackages_LIBRARIES} ${PYTHIA8}/lib/libpythia8.so)

#----------------------------------------------------------------------------
# Suppress the .rootmap generated by  ROOT_GENERATE_DICTIONARY.
# The ROOT_GENERATE_DICTIONARY function does not provide an option to disable rootmap generation.
# The presence of rootmaps from all examples in one directory causes ROOT warnings
# when executing examples from this directory.
#
add_custom_command(
    TARGET ${library_name} POST_BUILD COMMAND rm ${root_map}
)

#----------------------------------------------------------------------------
# Install the library and dictionary map
# to CMAKE_INSTALL_LIBDIR directory
#
if (VMC_INSTALL_EXAMPLES)
  install(TARGETS ${library_name} DESTINATION ${CMAKE_INSTALL_LIBDIR})
  install(FILES ${root_dict_lib} DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif()

#----------------------------------------------------------------------------
# MC Configuration file
# (for building MC dependent code)
#
include(UseMC)
message(STATUS "MCPackages_FOUND ${MCPackages_FOUND}")

#----------------------------------------------------------------------------
# Do not build executables if no MC is selected
#
if (NOT MCPackages_FOUND)
  return()
endif(NOT MCPackages_FOUND)

#----------------------------------------------------------------------------
# Add the executables, and link them to all libraries
#
#add_executable(${MC_PREFIX}vmc_${program_name} ${program_name}.cxx)
#add_executable(${MC_PREFIX}vmc_${test_name} ${test_name}.cxx)
#target_link_libraries(${MC_PREFIX}vmc_${program_name} ${library_name} ${MCPackages_LIBRARIES})
#target_link_libraries(${MC_PREFIX}vmc_${test_name} ${library_name} ${MCPackages_LIBRARIES})

#----------------------------------------------------------------------------
# Add program to the project targets
# (this avoids the need of typing the program name after make)
#
add_custom_target(${PROJECT_NAME} DEPENDS)
#                  ${MC_PREFIX}vmc_${program_name} ${MC_PREFIX}vmc_${test_name})

#----------------------------------------------------------------------------
# Install the executables to 'bin'
#
if (VMC_INSTALL_EXAMPLES)
  #  install(TARGETS ${MC_PREFIX}vmc_${program_name} ${MC_PREFIX}vmc_${test_name}
  install(TARGETS
    DESTINATION bin)
endif()
