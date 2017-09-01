
# Specify the minimum version for CMake
cmake_minimum_required(VERSION 2.6)

# Project's name
project(Dynstack CXX)

set (CORPP_VERSION_MAJOR 2)
set (CORPP_VERSION_MINOR 0)
set (CORPP_VERSION_PATCH 0)


option(__DYNDEFAULT__ "Set stack to default implementation" ON)


# Each subdirectory CMakeList.txt defines booth variable with
set (PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set (PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(DYNSTACK_SRCS
${PROJECT_SOURCE_DIR}/export.cpp
)

# include corpp_config.h
include_directories("${CMAKE_BINARY_DIR}")

# Include project files
include_directories("${PROJECT_INCLUDE_DIR}")

# Include user_code
include_directories("${USER_CODE_INCLUDE_DIR}")



add_library(${PROJECT_NAME} STATIC ${DYNSTACK_SRCS})
