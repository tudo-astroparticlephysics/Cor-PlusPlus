
# Specify the minimum version for CMake
cmake_minimum_required(VERSION 2.6)

# Project's name
project(REMOTE_CONTROL CXX)

set (RC_VERSION_MAJOR 2)
set (RC_VERSION_MINOR 0)
set (RC_VERSION_PATCH 0)



# Each subdirectory CMakeList.txt defines booth variable with
set (PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set (PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set (PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(REMOTECONTROL_SRCS
${PROJECT_SOURCE_DIR}/export.cpp
${PROJECT_SOURCE_DIR}/control/main_control.cpp
)


# include config.h
include_directories("${CMAKE_BINARY_DIR}")

# Include project files
include_directories("${PROJECT_INCLUDE_DIR}")

# Include user_code
include_directories("${USER_CODE_INCLUDE_DIR}")

add_library(${PROJECT_NAME} STATIC ${REMOTECONTROL_SRCS})

# Check and build tests
if(BUILD_TESTS)
    # Find gtest and add it to directory
    enable_testing()
    find_package(GTest REQUIRED)
    include_directories(${GTEST_INCLUDE_DIRS})

    # Add RCTest
    set(REMOTECONTROL_TEST_SRCS
        ${PROJECT_DIR}/test/unit_test.cpp
    )
    add_executable(rc_test ${REMOTECONTROL_TEST_SRCS})

    target_link_libraries(rc_test REMOTE_CONTROL)

    target_link_libraries(rc_test ${GTEST_BOTH_LIBRARIES})

    find_package (Threads)
    target_link_libraries(rc_test ${CMAKE_THREAD_LIBS_INIT})

    list(APPEND CMAKE_MODULE_PATH "${PROJECT_DIR}/CMake")
    find_package(ZeroMQ)
    include_directories(${ZeroMQ_INCLUDE_DIR})
    target_link_libraries(rc_test ${ZeroMQ_LIBRARY} )



endif(BUILD_TESTS)
