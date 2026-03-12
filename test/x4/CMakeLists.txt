# Copyright 2025-2026 Nana Sakisaka
# Copyright 2026 The Iris Project Contributors
#
# Distributed under the Boost Software License, Version 1.0.
# https://www.boost.org/LICENSE_1_0.txt

add_library(x4_test_pch OBJECT pch.cpp)
set_target_properties(x4_test_pch PROPERTIES CXX_EXTENSIONS OFF)
target_link_libraries(x4_test_pch PRIVATE Iris::X4 iris_cxx_test)
target_precompile_headers(x4_test_pch PUBLIC pch.hpp)

function(x4_define_test_headers test_name)
    iris_define_test_headers(x4_${test_name} PRIVATE FILE_SET HEADERS FILES ${ARGN})
endfunction()

function(x4_define_test test_name)
    iris_define_test(x4_${test_name} ${ARGN})
    iris_define_test_headers(x4_${test_name} iris_x4_test.hpp)
    target_precompile_headers(x4_${test_name}_test REUSE_FROM x4_test_pch)
    target_link_libraries(x4_${test_name}_test PRIVATE Iris::X4)
    set_target_properties(x4_${test_name}_test PROPERTIES FOLDER "test/x4")

    if(MSVC)
        # Prevent "Warning: Conflicting <Type> entries detected" error
        # set_source_files_properties(
        #     "${PROJECT_SOURCE_DIR}/iris_x4.natvis"
        #     TARGET_DIRECTORY x4_${test_name}_test
        #     PROPERTIES VS_SETTINGS "ExcludedFromBuild=true"
        # )

        # target_sources(x4_${test_name}_test PRIVATE cpp.hint)
    endif()
endfunction()

function(x4_define_tests)
    foreach(test_name IN LISTS ARGV)
        x4_define_test(${test_name} ${test_name}.cpp)
    endforeach()
endfunction()

x4_define_tests(
    actions
    alternative
    and_predicate
    as
    attr
    attribute_type_check
    bool
    char
    char_class
    container_support
    context
    debug
    difference
    eoi
    eol
    eps
    error_handler
    default_error_handler
    expect
    extract_int
    int
    iterator
    kleene
    lexeme
    list
    lit
    matches
    move_to
    not_predicate
    no_case
    no_skip
    omit
    optional
    parser
    partial_success
    plus
    real1
    real2
    real3
    recursive
    repeat
    rule1
    rule2
    rule3
    rule4
    seek
    sequence
    single_element_tuple_like
    skip
    substitution
    symbols1
    symbols2
    symbols3
    tst
    uint
    uint_radix
    unused
    with
    with_local
    without
    x3_rule_problem
    alloy_wrong_substitute_test_case
)

x4_define_test(rule_separate_tu rule_separate_tu.cpp rule_separate_tu_grammar.cpp)
x4_define_test_headers(rule_separate_tu rule_separate_tu_grammar.hpp)

x4_define_test(grammar grammar.cpp grammar_linker.cpp)
x4_define_test_headers(grammar grammar.hpp)

x4_define_test_headers(real1 real.hpp)
x4_define_test_headers(real2 real.hpp)
x4_define_test_headers(real3 real.hpp)
