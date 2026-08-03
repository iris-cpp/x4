/*=============================================================================
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include "iris_x4_test.hpp"

#include <iris/x4/attribute/smart_ptr.hpp>
#include <iris/x4/auxiliary/eps.hpp>
#include <iris/x4/numeric/int.hpp>

#include <stdexcept>

template<class T>
struct custom_deleter
{
    static constexpr void operator()(T* ptr) noexcept
    {
        delete ptr;
    }
};

TEST_CASE("unique_ptr (std::default_delete<T>)")
{
    using x4::unique_ptr;
    using x4::int_;
    using x4::eps;

    // Initial ptr = nullptr
    {
        std::unique_ptr<int> result;
        REQUIRE(parse("1", unique_ptr(int_), result));
        REQUIRE(!!result);
        CHECK(*result == 1);
    }
    {
        std::unique_ptr<int> result;
        REQUIRE_FALSE(parse("1", unique_ptr<int>(eps(false)), result));
        REQUIRE(!result);
    }
    {
        std::unique_ptr<int> result;
        REQUIRE_THROWS_AS(
            parse("1", unique_ptr<int>(eps[([] { throw std::runtime_error{"failed"}; })]), result),
            std::runtime_error
        );
        REQUIRE(!result);
    }

    // Initial ptr = some valid value
    {
        std::unique_ptr<int> result = std::make_unique<int>(42);
        REQUIRE(parse("1", unique_ptr(int_), result));
        REQUIRE(!!result);
        CHECK(*result == 1);
    }
    {
        std::unique_ptr<int> result = std::make_unique<int>(42);
        REQUIRE_FALSE(parse("1", unique_ptr<int>(eps(false)), result));
        REQUIRE(!!result);
        CHECK(*result == 42);
    }
    {
        std::unique_ptr<int> result = std::make_unique<int>(42);
        REQUIRE_THROWS_AS(
            parse("1", unique_ptr<int>(eps[([] { throw std::runtime_error{"failed"}; })]), result),
            std::runtime_error
        );
        REQUIRE(!!result);
        CHECK(*result == 42);
    }
}

TEST_CASE("unique_ptr (custom deleter)")
{
    using x4::unique_ptr;
    using x4::int_;
    using x4::eps;

    // Initial ptr = nullptr
    {
        std::unique_ptr<int, custom_deleter<int>> result;
        REQUIRE(parse("1", unique_ptr<int, custom_deleter<int>>(int_), result));
        REQUIRE(!!result);
        CHECK(*result == 1);
    }
    {
        std::unique_ptr<int, custom_deleter<int>> result;
        REQUIRE_FALSE(parse("1", unique_ptr<int, custom_deleter<int>>(eps(false)), result));
        REQUIRE(!result);
    }
    {
        std::unique_ptr<int, custom_deleter<int>> result;
        REQUIRE_THROWS_AS(
            parse("1", unique_ptr<int, custom_deleter<int>>(eps[([] { throw std::runtime_error{"failed"}; })]), result),
            std::runtime_error
        );
        REQUIRE(!result);
    }

    // Initial ptr = some valid value
    {
        std::unique_ptr<int, custom_deleter<int>> result(new int(42), custom_deleter<int>{});
        REQUIRE(parse("1", unique_ptr<int, custom_deleter<int>>(int_), result));
        REQUIRE(!!result);
        CHECK(*result == 1);
    }
    {
        std::unique_ptr<int, custom_deleter<int>> result(new int(42), custom_deleter<int>{});
        REQUIRE_FALSE(parse("1", unique_ptr<int, custom_deleter<int>>(eps(false)), result));
        REQUIRE(!!result);
        CHECK(*result == 42);
    }
    {
        std::unique_ptr<int, custom_deleter<int>> result(new int(42), custom_deleter<int>{});
        REQUIRE_THROWS_AS(
            parse("1", unique_ptr<int, custom_deleter<int>>(eps[([] { throw std::runtime_error{"failed"}; })]), result),
            std::runtime_error
        );
        REQUIRE(!!result);
        CHECK(*result == 42);
    }
}

TEST_CASE("shared_ptr (std::default_delete<T>)")
{
    using x4::shared_ptr;
    using x4::int_;
    using x4::eps;

    // Initial ptr = nullptr
    {
        std::shared_ptr<int> result;
        REQUIRE(parse("1", shared_ptr(int_), result));
        REQUIRE(!!result);
        CHECK(*result == 1);
    }
    {
        std::shared_ptr<int> result;
        REQUIRE_FALSE(parse("1", shared_ptr<int>(eps(false)), result));
        REQUIRE(!result);
    }
    {
        std::shared_ptr<int> result;
        REQUIRE_THROWS_AS(
            parse("1", shared_ptr<int>(eps[([] { throw std::runtime_error{"failed"}; })]), result),
            std::runtime_error
        );
        REQUIRE(!result);
    }

    // Initial ptr = some valid value
    {
        std::shared_ptr<int> result = std::make_shared<int>(42);
        REQUIRE(parse("1", shared_ptr(int_), result));
        REQUIRE(!!result);
        CHECK(*result == 1);
    }
    {
        std::shared_ptr<int> result = std::make_shared<int>(42);
        REQUIRE_FALSE(parse("1", shared_ptr<int>(eps(false)), result));
        REQUIRE(!!result);
        CHECK(*result == 42);
    }
    {
        std::shared_ptr<int> result = std::make_shared<int>(42);
        REQUIRE_THROWS_AS(
            parse("1", shared_ptr<int>(eps[([] { throw std::runtime_error{"failed"}; })]), result),
            std::runtime_error
        );
        REQUIRE(!!result);
        CHECK(*result == 42);
    }
}

TEST_CASE("shared_ptr (custom deleter)")
{
    using x4::shared_ptr;
    using x4::int_;
    using x4::eps;

    // Initial ptr = nullptr
    {
        std::shared_ptr<int> result;
        REQUIRE(parse("1", shared_ptr<int, custom_deleter<int>>(int_), result));
        REQUIRE(!!result);
        CHECK(*result == 1);
    }
    {
        std::shared_ptr<int> result;
        REQUIRE_FALSE(parse("1", shared_ptr<int, custom_deleter<int>>(eps(false)), result));
        REQUIRE(!result);
    }
    {
        std::shared_ptr<int> result;
        REQUIRE_THROWS_AS(
            parse("1", shared_ptr<int, custom_deleter<int>>(eps[([] { throw std::runtime_error{"failed"}; })]), result),
            std::runtime_error
        );
        REQUIRE(!result);
    }

    // Initial ptr = some valid value
    {
        std::shared_ptr<int> result(new int(42), custom_deleter<int>{});
        REQUIRE(parse("1", shared_ptr<int, custom_deleter<int>>(int_), result));
        REQUIRE(!!result);
        CHECK(*result == 1);
    }
    {
        std::shared_ptr<int> result(new int(42), custom_deleter<int>{});
        REQUIRE_FALSE(parse("1", shared_ptr<int, custom_deleter<int>>(eps(false)), result));
        REQUIRE(!!result);
        CHECK(*result == 42);
    }
    {
        std::shared_ptr<int> result(new int(42), custom_deleter<int>{});
        REQUIRE_THROWS_AS(
            parse("1", shared_ptr<int, custom_deleter<int>>(eps[([] { throw std::runtime_error{"failed"}; })]), result),
            std::runtime_error
        );
        REQUIRE(!!result);
        CHECK(*result == 42);
    }
}
