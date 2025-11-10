# EOOS Library (LIB)
#
# @author    Sergey Baigudin, sergey@baigudin.software
# @copyright 2024-2025, Sergey Baigudin, Baigudin Software

cmake_minimum_required(VERSION 3.22)

project(eoos-lib
    VERSION 0.14.0
    LANGUAGES CXX
)

include(FindPkgConfig)

if ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_SOURCE_DIR}")
    find_package(eoos-api 0.14.0 REQUIRED)
    find_package(eoos-sys 0.14.0 REQUIRED)
endif()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(eoos-lib INTERFACE)

target_include_directories(eoos-lib
    INTERFACE
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include/public>"
)

target_link_libraries(eoos-lib
    INTERFACE
    eoos::api
    eoos::sys
)

add_library(eoos::lib ALIAS eoos-lib)
