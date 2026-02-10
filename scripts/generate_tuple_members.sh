#!/usr/bin/sh
# Copyright 2026 The Iris Project Contributors
#
# Distributed under the Boost Software License, Version 1.0.
# https://www.boost.org/LICENSE_1_0.txt
g++ -Iinclude -Imodules/boost_preprocessor/include -Imodules/iris/include -E -P -DIRIS_ALLOY_GENERATE_PREPROCESSED include/iris/alloy/detail/tuple_impl.hpp > include/iris/alloy/detail/preprocessed/temp.hpp
cd include/iris/alloy/detail/preprocessed
cat tuple_impl.hpp.pre.in temp.hpp tuple_impl.hpp.post.in > tuple_impl.hpp
clang-format -i tuple_impl.hpp
rm temp.hpp
