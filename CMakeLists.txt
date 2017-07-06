
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

set(CMAKE_BUILD_TYPE Release)


# Include user_code
include_directories("${PROJECT_INCLUDE_DIR}")
message("Code to modify baack plugins is located in: ${PROJECT_INCLUDE_DIR}")

if(NOCOCO)
 add_definitions(-D_NOCOCO_)
endif(NOCOCO)

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


file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/obj")
file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

# Set the output folder where your program will be created
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin")
set(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib")

set(SOURCES ${CMAKE_SOURCE_DIR}/src/)

# The following folder will be included
include_directories("${CMAKE_SOURCE_DIR}/include")
include_directories("${CMAKE_SOURCE_DIR}/test")


# Set Compiler Version
set (CMAKE_CXX_STANDARD 14)
# Set shared compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wextra -pedantic -pedantic-errors")
