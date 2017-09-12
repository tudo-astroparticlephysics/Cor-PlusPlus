
# Specify the minimum version for CMake
cmake_minimum_required(VERSION 2.6)

# Project's name
project(COR-PP C CXX)

set (CORPP_VERSION_MAJOR 2)
set (CORPP_VERSION_MINOR 0)
set (CORPP_VERSION_PATCH 0)

option(BUILD_DYNSTACK "Build the DynStack module" ON)
option(BUILD_REMOTECONTROL "Build the Remote-Control module" ON)
option(BUILD_BASIC "Build the Basic module" ON)
option(BUILD_TESTS "Build test programs" OFF)

option(NOCOCO "Deactivate CORSIKA dependents" OFF)


# set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

set(PROJECT_DIR ${PROJECT_SOURCE_DIR})
set(PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(PROJECT_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)

set(USER_CODE_INCLUDE_DIR "${PROJECT_INCLUDE_DIR}")

#Set settings to release
set(CMAKE_BUILD_TYPE Release)

# Compiler-specific C++XY activation.
if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
    execute_process(
    COMMAND bash "-c" "echo -n $(expr $(${CMAKE_CXX_COMPILER} -dumpversion | cut -f1 -d.) \\* 10000 + $(${CMAKE_CXX_COMPILER} -dumpversion | cut -f2 -d.) \\* 100 + 0$(${CMAKE_CXX_COMPILER} -dumpversion | cut -f3 -d.))" OUTPUT_VARIABLE GCC_VERSION)
    message("GCC Version to number: ${GCC_VERSION}" )
    IF ( GCC_VERSION LESS 40600 ) # OR GCC_VERSION EQUAL 40600
        message(FATAL_ERROR "${PROJECT_NAME} requires g++ 4.6 or greater.")
    elseif( GCC_VERSION LESS 40800 OR GCC_VERSION EQUAL 40800)
        message("${PROJECT_NAME} uses a version below g++ 4.8.1 and does not support the full feature level. Read the manual to check which functionality is available!")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
    elseif( GCC_VERSION LESS 50000 OR GCC_VERSION EQUAL 50000)
        message("${PROJECT_NAME} uses a version above g++ 4.8.1 and does support the full feature level.")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    elseif( GCC_VERSION LESS 70200 OR GCC_VERSION EQUAL 70200)
        message("${PROJECT_NAME} uses a g++ version above 5.0.0 and support all features including additional performance and usability tweaks!")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z")
    endif ()
elseif ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
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

# When Main is moved into Cor++
if(BUILD_BAACK)

endif(BUILD_BAACK)


# configure a header file to pass CMake settings and checks
# to the source code
configure_file (
    "${CMAKE_SOURCE_DIR}/config/corpp_config.h.in"
    "${CMAKE_BINARY_DIR}/corpp_config.h"
)
# include corpp_config.h
include_directories(${CMAKE_BINARY_DIR})


# Check and build tests
if(BUILD_TESTS)
    #Set settings to debug
    set(CMAKE_BUILD_TYPE Debug)

    # Find gtest and add it to directory
    enable_testing()
    find_package(GTest REQUIRED)
    include_directories(${GTEST_INCLUDE_DIRS})

    # Add DynstackTest
    set(DYNSTACK_TEST_SRCS
        ${PROJECT_SOURCE_DIR}/test/dynstack/unit_test.cpp
    )
    add_executable(dynstack_test ${DYNSTACK_TEST_SRCS})

    target_link_libraries(dynstack_test network_lib)

    target_link_libraries(dynstack_test Dynstack)
    target_link_libraries(dynstack_test REMOTE_CONTROL)
    target_link_libraries(dynstack_test Basic)

    target_link_libraries(dynstack_test ${GTEST_BOTH_LIBRARIES})

    find_package (Threads)
    target_link_libraries(dynstack_test ${CMAKE_THREAD_LIBS_INIT})


    # Add RCTest
    set(REMOTECONTROL_TEST_SRCS
        ${PROJECT_SOURCE_DIR}/test/remote_control/unit_test.cpp    
    )
    add_executable(rc_test ${REMOTECONTROL_TEST_SRCS})

    target_link_libraries(rc_test network_lib)

    target_link_libraries(rc_test Dynstack)
    target_link_libraries(rc_test REMOTE_CONTROL)
    target_link_libraries(rc_test Basic)

    target_link_libraries(rc_test ${GTEST_BOTH_LIBRARIES})

    find_package (Threads)
    target_link_libraries(rc_test ${CMAKE_THREAD_LIBS_INIT})



endif(BUILD_TESTS)
