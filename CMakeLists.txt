
# Specify the minimum version for CMake
cmake_minimum_required(VERSION 3.0)

# Project's name
project(COR-PP C CXX)

set (CORPP_VERSION_MAJOR 2)
set (CORPP_VERSION_MINOR 0)
set (CORPP_VERSION_PATCH 0)

option(BUILD_DYNSTACK "Build the DynStack module" ON)
option(BUILD_REMOTECONTROL "Build the Remote-Control module" ON)
option(BUILD_BASIC "Build the Basic module" ON)
option(BUILD_TESTS "Build test programs" OFF)


set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

set(PROJECT_DIR ${PROJECT_SOURCE_DIR})
set(PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(PROJECT_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)

set(USER_CODE_INCLUDE_DIR "${PROJECT_INCLUDE_DIR}")

#Set settings to release
set(CMAKE_BUILD_TYPE Release)

# Set Compiler Version
set (CMAKE_CXX_STANDARD 14)

# Compiler-specific C++XY activation.
if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
    execute_process(
        COMMAND ${CMAKE_CXX_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
        message("Compiler version: ${GCC_VERSION}")
    if (NOT (GCC_VERSION VERSION_GREATER 4.6 OR GCC_VERSION VERSION_EQUAL 4.6))
        message(FATAL_ERROR "${PROJECT_NAME} requires g++ 4.6 or greater.")
    elseif(NOT (GCC_VERSION VERSION_GREATER 4.8 OR GCC_VERSION VERSION_EQUAL 4.8))
        message("${PROJECT_NAME} uses a version below g++ 4.8.1 and does not support the full feature level. Read the manual to check which functionality is available!")
    elseif(NOT (GCC_VERSION VERSION_GREATER 5.0 OR GCC_VERSION VERSION_EQUAL 5.0))
        message("${PROJECT_NAME} uses a version above g++ 4.8.1 and does support the full feature level.")
    elseif(NOT (GCC_VERSION VERSION_GREATER 7.0.0 OR GCC_VERSION VERSION_EQUAL 7.0.0))
        message("${PROJECT_NAME} uses a g++ version above 5.0.0 and support all features including additional performance and usability tweaks!")
    endif ()
elseif ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
else ()
    message(FATAL_ERROR "Your C++ compiler does not support the necessary feature to compile this module. For best results use gcc 4.6 or above.")
endif ()

# Set shared compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wextra -pedantic -pedantic-errors")


# Include user_code
include_directories("${PROJECT_INCLUDE_DIR}")
message("Code to modify baack plugins is located in: ${PROJECT_INCLUDE_DIR}")
message("")

# Add generic library
if(BUILD_DYNSTACK OR BUILD_REMOTECONTROL OR BUILD_BASIC)
    include_directories("${PROJECT_DIR}/lib/DTL/include")
    add_subdirectory(lib/DTL)
endif(BUILD_DYNSTACK OR BUILD_REMOTECONTROL OR BUILD_BASIC)

# Add each corsika module developed from this author
if(BUILD_DYNSTACK)
    include_directories("${PROJECT_DIR}/dynstack/include")
    add_subdirectory(dynstack)
endif(BUILD_DYNSTACK)

if(BUILD_REMOTECONTROL)
    include_directories("${PROJECT_DIR}/remote_control/include")
    add_subdirectory(remote_control)
endif(BUILD_REMOTECONTROL)

if(BUILD_BASIC)
    include_directories("${PROJECT_DIR}/basic/include")
    add_subdirectory(basic)
endif(BUILD_BASIC)


# configure a header file to pass CMake settings and checks
# to the source code
configure_file (
    "${CMAKE_SOURCE_DIR}/config/config.h.in"
    "${CMAKE_BINARY_DIR}/config.h"
)
# include config.h
include_directories("${CMAKE_BINARY_DIR}")
