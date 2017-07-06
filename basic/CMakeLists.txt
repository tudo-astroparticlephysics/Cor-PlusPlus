
# Specify the minimum version for CMake
cmake_minimum_required(VERSION 3.0)

# Project's name
project(BASIC CXX)

set (BASIC_VERSION_MAJOR 2)
set (BASIC_VERSION_MINOR 0)
set (BASIC_VERSION_PATCH 0)



# Each subdirectory CMakeList.txt defines booth variable with
set (PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set (PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(BASIC_SRCS
${PROJECT_SOURCE_DIR}/header_set.cpp
${PROJECT_SOURCE_DIR}/header_manager.cpp
${PROJECT_SOURCE_DIR}/export.cpp
${PROJECT_SOURCE_DIR}/corsika/event_end.cpp
${PROJECT_SOURCE_DIR}/corsika/run_end.cpp
${PROJECT_SOURCE_DIR}/corsika/run_header.cpp
${PROJECT_SOURCE_DIR}/corsika/steering_card.cpp
${PROJECT_SOURCE_DIR}/corsika/event_header.cpp
${PROJECT_SOURCE_DIR}/basic.cpp
)


# include config.h
include_directories("${CMAKE_BINARY_DIR}")

# Include project files
include_directories("${PROJECT_INCLUDE_DIR}")

# Include user_code
include_directories("${USER_CODE_INCLUDE_DIR}")

add_library(${PROJECT_NAME} STATIC ${BASIC_SRCS})
