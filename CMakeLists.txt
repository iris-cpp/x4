# Copyright 2020 Peter Dimov
# Copyright 2025 Nana Sakisaka
#
# Distributed under the Boost Software License, Version 1.0.
# https://www.boost.org/LICENSE_1_0.txt

cmake_minimum_required(VERSION 3.30)

if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE
        "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
        CACHE STRING ""
    )
endif()

project(iris_x4 VERSION 1.0.0 LANGUAGES CXX)


# -----------------------------------------------------------------
# Global settings
# Handle with care, keep these to the ones that can't be
# accomplished by target-specific settings.

set(CMAKE_COLOR_DIAGNOSTICS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

option(IRIS_X4_REMOVE_MINSIZEREL_CONFIG "Remove rarely used MinSizeRel config" ON)

if(MSVC)
    if(IRIS_X4_REMOVE_MINSIZEREL_CONFIG OR PROJECT_IS_TOP_LEVEL)
        list(REMOVE_ITEM CMAKE_CONFIGURATION_TYPES MinSizeRel)
    endif()
endif()


# -----------------------------------------------------------------
# Create common base targets

# Private component; do not depend on this.
add_library(_iris_x4_cxx_common INTERFACE)
set_target_properties(_iris_x4_cxx_common PROPERTIES CXX_EXTENSIONS OFF)

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

else()
    add_library(iris_x4 INTERFACE)
endif()

add_library(Iris::X4 ALIAS iris_x4)
set_target_properties(iris_x4 PROPERTIES CXX_EXTENSIONS OFF)
target_link_libraries(iris_x4 INTERFACE _iris_x4_cxx_common)


# -----------------------------------------------------------------
# Configure C++ version

# Set minimal C++ version explicitly; avoid "/std:c++latest"
set(IRIS_X4_CXX_FEATURE cxx_std_23)
if(MSVC)
    if(CMAKE_CXX_STANDARD EQUAL 23)
        set(CMAKE_CXX23_STANDARD_COMPILE_OPTION "/std:c++23preview")
        set(CMAKE_CXX23_EXTENSION_COMPILE_OPTION "/std:c++23preview")
        target_compile_options(_iris_x4_cxx_common INTERFACE /std:c++23preview)
        target_compile_options(iris_x4 PRIVATE /std:c++23preview) # Set strict version for dummy.cpp
    else()
        # may become /std:c++latest only on this case
        target_compile_features(_iris_x4_cxx_common INTERFACE ${IRIS_X4_CXX_FEATURE})
        target_link_libraries(iris_x4 PRIVATE _iris_x4_cxx_common) # Virtually sets `/std:c++latest` for dummy.cpp
    endif()
else() # Non-MSVC
    target_compile_features(_iris_x4_cxx_common INTERFACE ${IRIS_X4_CXX_FEATURE}) # Sets appropriate `-std=c++XX`
endif()
unset(IRIS_X4_CXX_FEATURE)


# -----------------------------------------------------------------
# Advanced compile/link settings

if(MSVC)
    # Don't set too strict flags for this! They must go to `iris_x4_cxx_test`.
    # ABI-dependent configurations MUST be set here.
    target_compile_definitions(
        _iris_x4_cxx_common
        INTERFACE UNICODE _UNICODE
    )
    target_compile_options(
        _iris_x4_cxx_common
        INTERFACE
            /EHsc /MP /utf-8 /Zc:preprocessor /permissive-
            # $<$<CONFIG:Debug,RelWithDebInfo>:/fsanitize=address> # TODO
    )
    target_link_options(
        _iris_x4_cxx_common
        INTERFACE
            # $<$<CONFIG:Debug,RelWithDebInfo>:/INCREMENTAL:NO> # TODO
    )

else()
    target_compile_options(
        _iris_x4_cxx_common
        INTERFACE
            # $<$<CONFIG:Debug>:-fsanitize=undefined,address> # TODO
    )
    target_link_options(
        _iris_x4_cxx_common
        INTERFACE
            # $<$<CONFIG:Debug>:-fsanitize=undefined,address> # TODO
    )
endif()


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

# find_package(...)
# target_link_libraries(iris_x4 INTERFACE ...)

# TODO: link Iris::iris

foreach(dep IN LISTS iris_x4_boost_deps)
    target_link_libraries(
        iris_x4
        INTERFACE
            Boost::${dep}
    )
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

if(PROJECT_IS_TOP_LEVEL)
    include(CTest)
endif()

if(BUILD_TESTING)
    add_subdirectory(test)

    if(MSVC)
        set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Iris::X4)
    endif()
endif()
