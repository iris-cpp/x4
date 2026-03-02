#include "iris_x4_test.hpp"

#include <iris/x4.hpp>

#include <string>
#include <vector>

TEST_CASE("SIMPLE")
{
    constexpr auto parser = x4::char_ >> *(x4::char_ >> x4::char_);
    std::string attr;
    REQUIRE(parse("abc", parser, attr));
}
