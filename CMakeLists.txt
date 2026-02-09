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


# -----------------------------------------------------------------
# Load Iris
# This sets many basic settings and common targets automatically.

if(PROJECT_IS_TOP_LEVEL)
    include(CTest) # sets BUILD_TESTING
endif()

if(NOT DEFINED IRIS_ROOT)
    set(IRIS_ROOT "${CMAKE_CURRENT_LIST_DIR}/modules/iris")
endif()

add_subdirectory("${IRIS_ROOT}")


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
            # "${PROJECT_SOURCE_DIR}/iris_x4.natvis" # TODO
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

set(iris_x4_boost_deps "")

# Everything required by `preprocessor` (manually confirmed)
list(APPEND iris_x4_boost_deps preprocessor)

# Everything required by `core` (manually confirmed)
list(APPEND iris_x4_boost_deps core)
list(APPEND iris_x4_boost_deps assert)
list(APPEND iris_x4_boost_deps config)
list(APPEND iris_x4_boost_deps static_assert)
list(APPEND iris_x4_boost_deps throw_exception)

# Everything required by `mpl`
list(APPEND iris_x4_boost_deps mpl)
list(APPEND iris_x4_boost_deps io)
list(APPEND iris_x4_boost_deps predef)
list(APPEND iris_x4_boost_deps type_traits)
list(APPEND iris_x4_boost_deps utility)

# ---------------------------------------------
list(REMOVE_DUPLICATES iris_x4_boost_deps)

foreach(dep IN LISTS iris_x4_boost_deps)
    add_subdirectory(../${dep} build_deps/${dep})
endforeach()

set_target_properties(boost_assert PROPERTIES FOLDER "_deps")
set_target_properties(boost_core PROPERTIES FOLDER "_deps")

foreach(dep IN LISTS iris_x4_boost_deps)
    target_link_libraries(iris_x4 INTERFACE Boost::${dep})
endforeach()


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
# Create the alloy target

if(MSVC)
    # This needs to be `OBJECT` target to set correct `/std:` flags on IDE
    add_library(iris_alloy OBJECT EXCLUDE_FROM_ALL)
    set_target_properties(iris_alloy PROPERTIES LINKER_LANGUAGE CXX)

    target_link_libraries(iris_alloy PUBLIC Iris::Iris)

else()
    add_library(iris_alloy INTERFACE)
    target_link_libraries(iris_alloy INTERFACE Iris::Iris)
endif()

add_library(Iris::Alloy ALIAS iris_alloy)
set_target_properties(iris_alloy PROPERTIES CXX_EXTENSIONS OFF)

# -----------------------------------------------------------------
# Configure alloy target

file(
    GLOB_RECURSE IRIS_ALLOY_HEADERS
    ${PROJECT_SOURCE_DIR}/include/iris/alloy/*.hpp
    ${PROJECT_SOURCE_DIR}/include/iris/alloy/*.ipp
)

target_sources(
    iris_alloy
    PRIVATE FILE_SET HEADERS TYPE HEADERS FILES ${IRIS_ALLOY_HEADERS}
)
source_group(
    TREE ${PROJECT_SOURCE_DIR}/include/iris PREFIX iris FILES ${IRIS_ALLOY_HEADERS}
)
target_include_directories(
    iris_alloy
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
