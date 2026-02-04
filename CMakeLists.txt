# Copyright 2020 Peter Dimov
# Copyright 2025-2026 Nana Sakisaka
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

add_subdirectory(modules/iris)


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

# Everything required by `fusion` (manually confirmed)
list(APPEND iris_x4_boost_deps fusion)
list(APPEND iris_x4_boost_deps config)
list(APPEND iris_x4_boost_deps container_hash)
list(APPEND iris_x4_boost_deps describe)
list(APPEND iris_x4_boost_deps mp11)
list(APPEND iris_x4_boost_deps core)
list(APPEND iris_x4_boost_deps assert)
list(APPEND iris_x4_boost_deps static_assert)
list(APPEND iris_x4_boost_deps throw_exception)
list(APPEND iris_x4_boost_deps function_types)
list(APPEND iris_x4_boost_deps detail)
list(APPEND iris_x4_boost_deps mpl)
list(APPEND iris_x4_boost_deps preprocessor)
list(APPEND iris_x4_boost_deps type_traits)
list(APPEND iris_x4_boost_deps predef)
list(APPEND iris_x4_boost_deps utility)
list(APPEND iris_x4_boost_deps io)
list(APPEND iris_x4_boost_deps tuple)
list(APPEND iris_x4_boost_deps typeof)
list(APPEND iris_x4_boost_deps functional)
list(APPEND iris_x4_boost_deps function)
list(APPEND iris_x4_boost_deps bind)

# Everything required by `variant` (manually confirmed)
list(APPEND iris_x4_boost_deps variant)
list(APPEND iris_x4_boost_deps integer)
list(APPEND iris_x4_boost_deps type_index)
list(APPEND iris_x4_boost_deps assert)
list(APPEND iris_x4_boost_deps config)
list(APPEND iris_x4_boost_deps container_hash)
list(APPEND iris_x4_boost_deps core)
list(APPEND iris_x4_boost_deps detail)
list(APPEND iris_x4_boost_deps mpl)
list(APPEND iris_x4_boost_deps preprocessor)
list(APPEND iris_x4_boost_deps static_assert)
list(APPEND iris_x4_boost_deps throw_exception)
list(APPEND iris_x4_boost_deps type_traits)
list(APPEND iris_x4_boost_deps utility)
list(APPEND iris_x4_boost_deps describe)
list(APPEND iris_x4_boost_deps mp11)
list(APPEND iris_x4_boost_deps predef)
list(APPEND iris_x4_boost_deps io)

# Everything required by `preprocessor` (manually confirmed)
list(APPEND iris_x4_boost_deps preprocessor)

# Everything required by `core` (manually confirmed)
list(APPEND iris_x4_boost_deps core)
list(APPEND iris_x4_boost_deps assert)
list(APPEND iris_x4_boost_deps config)
list(APPEND iris_x4_boost_deps static_assert)
list(APPEND iris_x4_boost_deps throw_exception)

# ---------------------------------------------
list(REMOVE_DUPLICATES iris_x4_boost_deps)

foreach(dep IN LISTS iris_x4_boost_deps)
    add_subdirectory(../${dep} build_deps/${dep})
endforeach()

foreach(dep IN LISTS iris_x4_boost_deps)
    target_link_libraries(iris_x4 INTERFACE Boost::${dep})
endforeach()


# -----------------------------------------------------------------
# Configure X4 main target

file(
    GLOB_RECURSE IRIS_X4_HEADERS
    ${PROJECT_SOURCE_DIR}/include/iris/*.hpp
    ${PROJECT_SOURCE_DIR}/include/iris/*.ipp
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
