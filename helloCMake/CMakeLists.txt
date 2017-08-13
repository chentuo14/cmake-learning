project(helloCMake)

if(${CMAKE_VERSION} VERSION_LESS 3.1.0)
    message("Not a valid CMake version")
    message("On Ubuntu >=16.04, install or upgrade via:")
    message("   sudo apt-get install cmake")
    message("")
    message("Official website: https://cmake.org/download/")
    message(FATAL_ERROR "Update CMake and try again")
endif()

message(STATUS "This is BINARY dir " ${helloCMake_BINARY_DIR})
message(STATUS "This is SOURCE dir " ${helloCMake_SOURCE_DIR})

cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
aux_source_directory(. SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})

