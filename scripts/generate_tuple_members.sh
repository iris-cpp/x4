#!/usr/bin/sh
g++ -Iinclude -Imodules/iris/include -E -P -DIRIS_ALLOY_GENERATE_PREPROCESSED include/iris/alloy/detail/tuple_impl.hpp > include/iris/alloy/detail/preprocessed/temp.hpp
cd include/iris/alloy/detail/preprocessed
sed -e '/\/\/ replace me/{r temp.hpp' -e 'd}' tuple_impl.hpp.in > tuple_impl.hpp
clang-format -i tuple_impl.hpp
rm temp.hpp
