# Copyright 2025-2026 Nana Sakisaka
# Copyright 2026 The Iris Project Contributors
#
# Distributed under the Boost Software License, Version 1.0.
# https://www.boost.org/LICENSE_1_0.txt

if(PROJECT_IS_TOP_LEVEL)
    option(IRIS_TEST_ALLOY "test alloy" ON)
    if(IRIS_TEST_ALLOY)
        add_subdirectory(alloy)
    endif()
    option(IRIS_TEST_X4 "test x4" ON)
    if(IRIS_TEST_X4)
        add_subdirectory(x4)
    endif()
endif()
