# Copyright 2026 The Iris Project Contributors
#
# Distributed under the Boost Software License, Version 1.0.
# https://www.boost.org/LICENSE_1_0.txt

function(alloy_define_test test_name)
    iris_define_test(alloy_${test_name} ${ARGN})
    target_link_libraries(alloy_${test_name}_test PRIVATE Iris::Alloy)
    set_target_properties(alloy_${test_name}_test PROPERTIES FOLDER "test/alloy")
endfunction()

function(alloy_define_tests)
    foreach(test_name IN LISTS ARGV)
        alloy_define_test(${test_name} ${test_name}.cpp)
    endforeach()
endfunction()

alloy_define_tests(
  alloy
)
