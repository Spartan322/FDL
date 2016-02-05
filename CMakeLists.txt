#TODO: actually create a real cmake file

cmake_minimum_required(VERSION 2.8.12)

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING
    "Choose the type of build, options are: None(CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel.")
endif ()

project(FDL)

option(FDL_DOC "Generates Documentation Target." ${MASTER_PROJECT})
option(FLD_TEST "Generates Testing Target." ${MASTER_PROJECT})

add_subdirectory(include/FDL)

if(FDL_TEST)
	enable_testing()
	add_subdirectory(tests)
endif()
