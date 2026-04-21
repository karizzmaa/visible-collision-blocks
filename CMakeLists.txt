cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")

project(VisibleCollisionBlocks VERSION 1.0.0)

if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "Unable to find Geode SDK! Please define GEODE_SDK.")
else()
    message(STATUS "Found Geode SDK: $ENV{GEODE_SDK}")
endif()

add_library(${PROJECT_NAME} SHARED src/main.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE src)

set(GEODE_LINK_NIGHTLY 0)
include($ENV{GEODE_SDK}/cmake/GeodeFile.cmake)

setup_geode_mod(${PROJECT_NAME}
SOURCE_DIR ${CMAKE_SOURCE_DIR})
