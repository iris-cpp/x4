#!/usr/bin/sh
g++ -Iinclude -I../preprocessor/include -Imodules/iris/include -E -P -DIRIS_ALLOY_GENERATE_PREPROCESSED include/iris/alloy/detail/tuple_impl.hpp > include/iris/alloy/detail/preprocessed/temp.hpp
cd include/iris/alloy/detail/preprocessed
cat tuple_impl.hpp.pre.in temp.hpp tuple_impl.hpp.post.in > tuple_impl.hpp
clang-format -i tuple_impl.hpp
rm temp.hpp
