# Specify the minimum version for CMake
cmake_minimum_required(VERSION 2.6)


if(BUILD_REMOTECONTROL)
    include_directories("${PROJECT_DIR}/plugins/remote_control/include")
    add_subdirectory(remote_control)
endif(BUILD_REMOTECONTROL)

