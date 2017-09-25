
# Specify the minimum version for CMake
cmake_minimum_required(VERSION 2.6)

# Project's name
project(CorsikaPP CXX Fortran)


option(__CORSIKA__ "Use corsika library routines" ON)


# Each subdirectory CMakeList.txt defines booth variable with
set (PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set (PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set (PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

# include corpp_config.h
include_directories("${CMAKE_BINARY_DIR}")

# Include project files
include_directories("${PROJECT_INCLUDE_DIR}")

# Include user_code
include_directories("${USER_CODE_INCLUDE_DIR}")

configure_file (
    "${PROJECT_DIR}/config/corsika_config.h.in"
    "${CMAKE_BINARY_DIR}/corsika_config.h"
)

#set(CMAKE_Fortran_FLAGS "${CMAKE_Fortran_FLAGS} -include ${CORSIKA_DIR}/include/config.h")

# FFLAGS depend on the compiler
get_filename_component (Fortran_COMPILER_NAME ${CMAKE_Fortran_COMPILER} NAME)

if (Fortran_COMPILER_NAME MATCHES "gfortran.*")
  # gfortran
  set (CMAKE_Fortran_FLAGS_RELEASE "-funroll-all-loops -fno-f2c -O3")
  set (CMAKE_Fortran_FLAGS_DEBUG   "-fno-f2c -O0 -g")
elseif (Fortran_COMPILER_NAME MATCHES "ifort.*")
  # ifort
  set (CMAKE_Fortran_FLAGS_RELEASE "-f77rtl -O3")
  set (CMAKE_Fortran_FLAGS_DEBUG   "-f77rtl -O0 -g")
elseif (Fortran_COMPILER_NAME MATCHES "g77")
  # g77
  set (CMAKE_Fortran_FLAGS_RELEASE "-funroll-all-loops -fno-f2c -O3 -m32")
  set (CMAKE_Fortran_FLAGS_DEBUG   "-fno-f2c -O0 -g -m32")
else (Fortran_COMPILER_NAME MATCHES "gfortran.*")
  message ("CMAKE_Fortran_COMPILER full path: " ${CMAKE_Fortran_COMPILER})
  message ("Fortran compiler: " ${Fortran_COMPILER_NAME})
  message ("No optimized Fortran compiler flags are known, we just try -O2...")
  set (CMAKE_Fortran_FLAGS_RELEASE "-O2")
  set (CMAKE_Fortran_FLAGS_DEBUG   "-O0 -g")
endif (Fortran_COMPILER_NAME MATCHES "gfortran.*")



include_directories("${CORSIKA_DIR}/urqmd")
include_directories("${CORSIKA_DIR}/bernlohr")
include_directories("${CORSIKA_DIR}/dpmjet")
include_directories("${CORSIKA_DIR}/src")
include_directories("${CORSIKA_DIR}/src/utils")

set(CORSIKA_SRCS
    ${CORSIKA_DIR}/src/corsikacompilefile.f
)

add_library(${PROJECT_NAME} STATIC ${CORSIKA_SRCS})



# Check and build tests
if(BUILD_TESTS)
    # Find gtest and add it to directory
    enable_testing()
    find_package(GTest REQUIRED)
    include_directories(${GTEST_INCLUDE_DIRS})

    # Include Test directory
    include_directories(${PROJECT_DIR}/test)

    # Add DynstackTest
    set(CORSIKA_TEST_SRCS
        ${PROJECT_DIR}/test/unit_test.cpp
    )
    add_executable(corsika_test ${CORSIKA_TEST_SRCS})

    #target_link_libraries(corsika_test Dynstack)
    #target_link_libraries(corsika_test Basic)

    target_link_libraries(corsika_test ${GTEST_BOTH_LIBRARIES})

endif(BUILD_TESTS)
