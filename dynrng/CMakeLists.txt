
# Specify the minimum version for CMake
cmake_minimum_required(VERSION 2.6)

# Project's name
project(DYNRNG CXX)

set (CORPP_VERSION_MAJOR 2)
set (CORPP_VERSION_MINOR 0)
set (CORPP_VERSION_PATCH 0)


option(__RNG_MERSENNE__ "Set RNG to 64Bit Mersenne Twister" ON)


# Each subdirectory CMakeList.txt defines booth variable with
set (PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set (PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set (PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(DYNRNG_SRCS
${PROJECT_SOURCE_DIR}/export.cpp
)

# include corpp_config.h
include_directories("${CMAKE_BINARY_DIR}")

# Include project files
include_directories("${PROJECT_INCLUDE_DIR}")

# Include user_code
include_directories("${USER_CODE_INCLUDE_DIR}")



add_library(${PROJECT_NAME} STATIC ${DYNRNG_SRCS})


# Check and build tests
if(BUILD_TESTS)
    # Find gtest and add it to directory
    enable_testing()
    find_package(GTest REQUIRED)
    include_directories(${GTEST_INCLUDE_DIRS})

    # Include Test directory
    include_directories(${PROJECT_DIR}/test)

    # Add DynrngTest
    set(DYNRNG_TEST_SRCS
        ${PROJECT_DIR}/test/unit_test.cpp
    )
    add_executable(dynrng_test ${DYNRNG_TEST_SRCS})

    target_link_libraries(dynrng_test Dynrng)
    target_link_libraries(dynrng_test Basic)

    target_link_libraries(dynrng_test ${GTEST_BOTH_LIBRARIES})

    find_package (Threads)
    target_link_libraries(dynrng_test ${CMAKE_THREAD_LIBS_INIT})



endif(BUILD_TESTS)
