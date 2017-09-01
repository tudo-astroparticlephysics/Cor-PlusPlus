
# Specify the minimum version for CMake
cmake_minimum_required(VERSION 2.6)

# Project's name
project(REMOTE_CONTROL CXX)

set (RC_VERSION_MAJOR 2)
set (RC_VERSION_MINOR 0)
set (RC_VERSION_PATCH 0)



# Each subdirectory CMakeList.txt defines booth variable with
set (PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set (PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(REMOTECONTROL_SRCS
${PROJECT_SOURCE_DIR}/export.cpp
${PROJECT_SOURCE_DIR}/communication/message_queue.cpp
${PROJECT_SOURCE_DIR}/communication/packet.cpp
${PROJECT_SOURCE_DIR}/control/main_control.cpp
${PROJECT_SOURCE_DIR}/control/periodic_task.cpp
${PROJECT_SOURCE_DIR}/control/recv_cache.cpp
)



# include config.h
include_directories("${CMAKE_BINARY_DIR}")

# Include project files
include_directories("${PROJECT_INCLUDE_DIR}")

# Include user_code
include_directories("${USER_CODE_INCLUDE_DIR}")

add_library(${PROJECT_NAME} STATIC ${REMOTECONTROL_SRCS})
