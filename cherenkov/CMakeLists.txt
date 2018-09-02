
# Specify the minimum version for CMake
cmake_minimum_required(VERSION 3.1)

# Project's name
project(Cherenkov CXX)

set (CORPP_VERSION_MAJOR 2)
set (CORPP_VERSION_MINOR 0)
set (CORPP_VERSION_PATCH 0)


# Each subdirectory CMakeList.txt defines booth variable with
set (PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set (PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set (PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(CHERENKOV_SRCS
${PROJECT_SOURCE_DIR}/export.cpp
)

find_package(OpenCL REQUIRED)
include_directories(${OpenCL_INCLUDE_DIRS})
link_directories(${OpenCL_LIBRARY})

# include corpp_config.h
include_directories("${CMAKE_BINARY_DIR}")

# Include project files
include_directories("${PROJECT_INCLUDE_DIR}")

# Include user_code
include_directories("${USER_CODE_INCLUDE_DIR}")



add_library(${PROJECT_NAME} STATIC ${CHERENKOV_SRCS})


# Check and build tests
if(BUILD_TESTS)
    # Find gtest and add it to directory
    enable_testing()
    find_package(GTest REQUIRED)
    include_directories(${GTEST_INCLUDE_DIRS})

    # Include Test directory
    include_directories(${PROJECT_DIR}/test)

    # Add CherenkovTest
    set(CHERENKOV_TEST_SRCS
        ${PROJECT_DIR}/test/unit_test.cpp
    )
    add_executable(cherenkov_unittest ${CHERENKOV_TEST_SRCS})

    #target_link_libraries(cherenkov_test Cerenkov)

    target_link_libraries(cherenkov_unittest ${GTEST_BOTH_LIBRARIES})
    target_link_libraries (cherenkov_unittest ${OpenCL_LIBRARY})

    find_package (Threads)
    target_link_libraries(cherenkov_unittest ${CMAKE_THREAD_LIBS_INIT})

    add_test(cherenkov_gtest ../bin/cherenkov_unittest)


    ##################################################################
    # OpenCL function and execution test
    # Add OpenclTests
    set(OPENCL_TEST_SRCS
        ${PROJECT_DIR}/test/opencl_test.cpp
        ${PROJECT_DIR}/src/ocl/OpenCL_Hardware.cpp
        ${PROJECT_DIR}/src/ocl/OpenCL_Error.cpp
        ${PROJECT_DIR}/src/ocl/OpenCL_Kernel.cpp
        ${PROJECT_DIR}/src/ocl/OpenCL_Program.cpp
        ${PROJECT_DIR}/src/ocl/OpenCL_Queue.cpp
    )
    add_executable(opencl_test ${OPENCL_TEST_SRCS})
    target_link_libraries (opencl_test Cherenkov)

    target_link_libraries (opencl_test ${OpenCL_LIBRARY})



     ##################################################################
    # Random generator Tests

    set(OPENCL_RANDOM_TEST_SRCS
        ${PROJECT_DIR}/test/opencl_random.cpp
        ${PROJECT_DIR}/src/ocl/OpenCL_Hardware.cpp
        ${PROJECT_DIR}/src/ocl/OpenCL_Error.cpp
        ${PROJECT_DIR}/src/ocl/OpenCL_Kernel.cpp
        ${PROJECT_DIR}/src/ocl/OpenCL_Program.cpp
        ${PROJECT_DIR}/src/ocl/OpenCL_Queue.cpp
    )
    add_executable(random_test ${OPENCL_RANDOM_TEST_SRCS})
    target_link_libraries (random_test Cherenkov)

    target_link_libraries (random_test ${OpenCL_LIBRARY})


endif(BUILD_TESTS)


if(BUILD_BENCHMARK)
    ##################################################################
    # OpenCL function and execution test
    # Add OpenclBenchmark
    set(OPENCL_BENCHMARK_SRCS
        ${PROJECT_DIR}/benchmark/bench.cpp
        ${PROJECT_DIR}/src/ocl/OpenCL_Hardware.cpp
        ${PROJECT_DIR}/src/ocl/OpenCL_Error.cpp
        ${PROJECT_DIR}/src/ocl/OpenCL_Kernel.cpp
        ${PROJECT_DIR}/src/ocl/OpenCL_Program.cpp
        ${PROJECT_DIR}/src/ocl/OpenCL_Queue.cpp
    )
    add_executable(cerenk_bench ${OPENCL_BENCHMARK_SRCS})
    target_link_libraries (cerenk_bench Cherenkov)

    target_link_libraries (cerenk_bench ${OpenCL_LIBRARY})
endif(BUILD_BENCHMARK)
