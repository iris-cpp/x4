REM Copyright 2026 The Iris Project Contributors
REM
REM Distributed under the Boost Software License, Version 1.0.
REM https://www.boost.org/LICENSE_1_0.txt
@echo off
cl /TP /std:c++23preview /Iinclude /I..\preprocessor\include /Imodules\iris\include /P /EP /C /DIRIS_ALLOY_GENERATE_PREPROCESSED /Fiinclude\iris\alloy\detail\preprocessed\temp.hpp include\iris\alloy\detail\tuple_impl.hpp
pushd include\iris\alloy\detail\preprocessed
type tuple_impl.hpp.pre.in temp.hpp tuple_impl.hpp.post.in > tuple_impl.hpp
clang-format -i tuple_impl.hpp
del temp.hpp
popd
