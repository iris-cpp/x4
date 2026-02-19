# Copyright 2025-2026 Nana Sakisaka
# Copyright 2026 The Iris Project Contributors
#
# Distributed under the Boost Software License, Version 1.0.
# https://www.boost.org/LICENSE_1_0.txt

if(PROJECT_IS_TOP_LEVEL)
    if(NOT DEFINED IRIS_CI_COMPONENT OR IRIS_CI_COMPONENT STREQUAL alloy)
        add_subdirectory(alloy)
    endif()
    if(NOT DEFINED IRIS_CI_COMPONENT OR IRIS_CI_COMPONENT STREQUAL x4)
        add_subdirectory(x4)
    endif()
endif()
