# Copyright 2020 Peter Dimov
# Copyright 2025-2026 Nana Sakisaka
# Copyright 2026 The Iris Project Contributors
#
# Distributed under the Boost Software License, Version 1.0.
# https://www.boost.org/LICENSE_1_0.txt

cmake_minimum_required(VERSION 3.30)

if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()

project(iris_x4 VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

# -----------------------------------------------------------------
# Load Iris
# This sets many basic settings and common targets automatically.

if(PROJECT_IS_TOP_LEVEL)
    include(CTest) # sets BUILD_TESTING
endif()

if(NOT TARGET Iris::Iris)
    if(DEFINED IRIS_DIR)
        cmake_path(ABSOLUTE_PATH IRIS_DIR BASE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" NORMALIZE OUTPUT_VARIABLE _iris_src)
    else()
        set(_iris_src "${CMAKE_CURRENT_SOURCE_DIR}/modules/iris")
    endif()

    if(NOT EXISTS "${_iris_src}/CMakeLists.txt")
        message(FATAL_ERROR "Iris was not found at '${_iris_src}'. Set IRIS_DIR to the Iris source directory.")
    endif()

    add_subdirectory("${_iris_src}" "${CMAKE_CURRENT_BINARY_DIR}/iris")
endif()


# -----------------------------------------------------------------
# Create the main X4 target

if(MSVC)
    # This needs to be `OBJECT` target to set correct `/std:` flags on IDE
    add_library(iris_x4 OBJECT EXCLUDE_FROM_ALL)
    set_target_properties(iris_x4 PROPERTIES LINKER_LANGUAGE CXX)

    target_sources(
        iris_x4
        PRIVATE
            # "${PROJECT_SOURCE_DIR}/cpp.hint" # TODO
            "${PROJECT_SOURCE_DIR}/iris_x4.natvis"
    )

    target_link_libraries(iris_x4 PUBLIC Iris::Iris)

else()
    add_library(iris_x4 INTERFACE)
    target_link_libraries(iris_x4 INTERFACE Iris::Iris)
endif()

add_library(Iris::X4 ALIAS iris_x4)
set_target_properties(iris_x4 PROPERTIES CXX_EXTENSIONS OFF)


# -----------------------------------------------------------------
# External libraries

# None!


# -----------------------------------------------------------------
# Configure X4 main target

file(
    GLOB_RECURSE IRIS_X4_HEADERS
    ${PROJECT_SOURCE_DIR}/include/iris/x4/*.hpp
    ${PROJECT_SOURCE_DIR}/include/iris/x4/*.ipp
    ${PROJECT_SOURCE_DIR}/include/iris/x4.hpp
)

target_sources(
    iris_x4
    PRIVATE FILE_SET HEADERS TYPE HEADERS FILES ${IRIS_X4_HEADERS}
)
source_group(
    TREE ${PROJECT_SOURCE_DIR}/include/iris PREFIX iris FILES ${IRIS_X4_HEADERS}
)
target_include_directories(
    iris_x4
    INTERFACE ${PROJECT_SOURCE_DIR}/include
)


# -----------------------------------------------------------------
# Test

if(BUILD_TESTING)
    add_subdirectory(test)

    if(MSVC AND PROJECT_IS_TOP_LEVEL)
        set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Iris::X4)
    endif()
endif()
