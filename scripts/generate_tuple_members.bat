cl /TP /std:c++23preview /Iinclude /I..\preprocessor\include /Imodules\iris\include /P /EP /C /DIRIS_ALLOY_GENERATE_PREPROCESSED /Fiinclude\iris\alloy\detail\preprocessed\temp.hpp include\iris\alloy\detail\tuple_impl.hpp
pushd include\iris\alloy\detail\preprocessed
type tuple_impl.hpp.pre.in temp.hpp tuple_impl.hpp.post.in > tuple_impl.hppp
del temp.hpp
popd
