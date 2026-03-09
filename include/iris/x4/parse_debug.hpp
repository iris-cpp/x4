#ifndef IRIS_X4_PARSE_DEBUG_HPP
#define IRIS_X4_PARSE_DEBUG_HPP

#include <iris/config.hpp>
#include <iris/x4/debug/default_error_handler.hpp>
#include <iris/x4/directive/with.hpp>
#include <iris/x4/parse.hpp>

#include <print>
#include <sstream>

namespace iris::x4 {

namespace detail {

struct parse_debug_fn_main
{
    // --------------------------------------------
    // parse(range)

    // R + Parser + Attribute
    template<std::ranges::forward_range R, X4RangeParseParser<R> Parser, X4Attribute ParseAttr>
    static constexpr parse_result_for<R>
    operator()(R const& range, Parser&& p, ParseAttr& attr)
    {
        auto const& range_ = detail::as_parse_range(range);
        std::ostringstream error_out, trace_out;
        default_error_handler error_handler{
            std::ranges::begin(range_), std::ranges::end(range_),
            &error_out, &trace_out
        };

        auto const res = x4::parse(range_, x4::with<contexts::error_handler>(error_handler)[std::forward<Parser>(p)], attr);
        if (!trace_out.str().empty()) std::println("{}", trace_out.str());
        if (!res && !error_out.str().empty()) {
            std::println("{}", error_out.str());
        }
        return res;
    }

    // parse_result + R + Parser + Attribute
    template<std::ranges::forward_range R, X4RangeParseParser<R> Parser, X4Attribute ParseAttr>
    static constexpr void
    operator()(parse_result_for<R>& res, R const& range, Parser&& p, ParseAttr& attr)
    {
        auto const& range_ = detail::as_parse_range(range);
        std::ostringstream error_out, trace_out;
        default_error_handler error_handler{
            std::ranges::begin(range_), std::ranges::end(range_),
            &error_out, &trace_out
        };

        x4::parse(res, range_, x4::with<contexts::error_handler>(error_handler)[std::forward<Parser>(p)], attr);
        if (!trace_out.str().empty()) std::println("{}", trace_out.str());
        if (!res && !error_out.str().empty()) {
            std::println("{}", error_out.str());
        }
    }

    // --------------------------------------------
    // phrase_parse(range)

    // R + Parser + Skipper + Attribute + (root_skipper_flag)
    template<std::ranges::forward_range R, X4RangeParseParser<R> Parser, X4RangeParseSkipper<R> Skipper, X4Attribute ParseAttr>
    static constexpr parse_result_for<R>
    operator()(R const& range, Parser&& p, Skipper const& s, ParseAttr& attr, root_skipper_flag flag = root_skipper_flag::do_post_skip)
    {
        auto const& range_ = detail::as_parse_range(range);
        std::ostringstream error_out, trace_out;
        default_error_handler error_handler{
            std::ranges::begin(range_), std::ranges::end(range_),
            &error_out, &trace_out
        };

        auto const res = x4::parse(range_, x4::with<contexts::error_handler>(error_handler)[std::forward<Parser>(p)], s, attr, flag);
        if (!trace_out.str().empty()) std::println("{}", trace_out.str());
        if (!res && !error_out.str().empty()) {
            std::println("{}", error_out.str());
        }
        return res;
    }

    // parse_result + R + Parser + Skipper + Attribute
    template<std::ranges::forward_range R, X4RangeParseParser<R> Parser, X4RangeParseSkipper<R> Skipper, X4Attribute ParseAttr>
    static constexpr void
    operator()(parse_result_for<R>& res, R const& range, Parser&& p, Skipper const& s, ParseAttr& attr, root_skipper_flag flag = root_skipper_flag::do_post_skip)
    {
        auto const& range_ = detail::as_parse_range(range);
        std::ostringstream error_out, trace_out;
        default_error_handler error_handler{
            std::ranges::begin(range_), std::ranges::end(range_),
            &error_out, &trace_out
        };

        x4::parse(res, range_, x4::with<contexts::error_handler>(error_handler)[std::forward<Parser>(p)], s, attr, flag);
        if (!trace_out.str().empty()) std::println("{}", trace_out.str());
        if (!res && !error_out.str().empty()) {
            std::println("{}", error_out.str());
        }
    }

    // --------------------------------------------
    // parse(it/se)

    // It/Se + Parser + Attribute
    template<std::forward_iterator It, std::sentinel_for<It> Se, X4Parser<It, Se> Parser, X4Attribute ParseAttr>
    static constexpr parse_result<It, Se>
    operator()(It first, Se last, Parser&& p, ParseAttr& attr)
    {
        std::ostringstream error_out, trace_out;
        default_error_handler error_handler{
            first, last,
            &error_out, &trace_out
        };

        auto const res = x4::parse(first, last, x4::with<contexts::error_handler>(error_handler)[std::forward<Parser>(p)], attr);
        if (!trace_out.str().empty()) std::println("{}", trace_out.str());
        if (!res && !error_out.str().empty()) {
            std::println("{}", error_out.str());
        }
        return res;
    }

    // parse_result + It/Se + Parser + Attribute
    template<std::forward_iterator It, std::sentinel_for<It> Se, X4Parser<It, Se> Parser, X4Attribute ParseAttr>
    static constexpr void
    operator()(parse_result<It, Se>& res, It first, Se last, Parser&& p, ParseAttr& attr)
    {
        std::ostringstream error_out, trace_out;
        default_error_handler error_handler{
            first, last,
            &error_out, &trace_out
        };

        x4::parse(res, first, last, x4::with<contexts::error_handler>(error_handler)[std::forward<Parser>(p)], attr);
        if (!trace_out.str().empty()) std::println("{}", trace_out.str());
        if (!res && !error_out.str().empty()) {
            std::println("{}", error_out.str());
        }
    }

    // --------------------------------------------
    // phrase_parse(it/se)

    // It/Se + Parser + Skipper + Attribute + (root_skipper_flag)
    template<std::forward_iterator It, std::sentinel_for<It> Se, X4Parser<It, Se> Parser, X4ExplicitParser<It, Se> Skipper, X4Attribute ParseAttr>
    static constexpr parse_result<It, Se>
    operator()(It first, Se last, Parser&& p, Skipper const& s, ParseAttr& attr, root_skipper_flag flag = root_skipper_flag::do_post_skip)
    {
        std::ostringstream error_out, trace_out;
        default_error_handler error_handler{
            first, last,
            &error_out, &trace_out
        };

        auto const res = x4::parse(first, last, x4::with<contexts::error_handler>(error_handler)[std::forward<Parser>(p)], s, attr, flag);
        if (!trace_out.str().empty()) std::println("{}", trace_out.str());
        if (!res && !error_out.str().empty()) {
            std::println("{}", error_out.str());
        }
        return res;
    }

    // parse_result + It/Se + Parser + Skipper + Attribute + (root_skipper_flag)
    template<std::forward_iterator It, std::sentinel_for<It> Se, X4Parser<It, Se> Parser, X4ExplicitParser<It, Se> Skipper, X4Attribute ParseAttr>
    static constexpr void
    operator()(parse_result<It, Se>& res, It first, Se last, Parser&& p, Skipper const& s, ParseAttr& attr, root_skipper_flag flag = root_skipper_flag::do_post_skip)
    {
        std::ostringstream error_out, trace_out;
        default_error_handler error_handler{
            first, last,
            &error_out, &trace_out
        };

        x4::parse(res, first, last, x4::with<contexts::error_handler>(error_handler)[std::forward<Parser>(p)], s, attr, flag);
        if (!trace_out.str().empty()) std::println("{}", trace_out.str());
        if (!res && !error_out.str().empty()) {
            std::println("{}", error_out.str());
        }
    }
}; // parse_debug_fn

} // detail

inline namespace cpos {

[[maybe_unused]] inline constexpr detail::parse_debug_fn_main parse_debug{};

} // cpos

} // iris::x4

#endif
