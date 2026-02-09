cl /I:include -I:..\preprocessor\include /I:modules\iris\include /P /EP /C include\iris\alloy\detail\tuple_impl.hpp /OUT:include\iris\alloy\detail\preprocessed\temp.hpp
cd include\iris\alloy\detail\preprocessed
type tuple_impl.hpp.pre.in temp.hpp tuple_impl.hpp.post.in
del temp.hpp
