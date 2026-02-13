#ifndef IRIS_X4_DEBUG_ERROR_REPORTING_HPP
#define IRIS_X4_DEBUG_ERROR_REPORTING_HPP

/*=============================================================================
    Copyright (c) 2014 Joel de Guzman
    Copyright (c) 2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <iris/x4/core/unused.hpp>

#include <iris/x4/debug/error_handler.hpp>
#include <iris/x4/debug/print_token.hpp>
#include <iris/x4/debug/print_attribute.hpp>

#include <iris/x4/ast/position_tagged.hpp>
#include <iris/x4/string/utf8.hpp>

#include <ranges>
#include <ostream>
#include <string>
#include <string_view>
#include <iterator>

namespace iris::x4 {

template<std::forward_iterator It>
class default_error_handler
{
    static constexpr int IndentSpaces = 2;
    static constexpr int CharsToPrint = 20;

public:
    using iterator_type = It;

    default_error_handler(
        It first, It last,
        std::ostream& err_out,
        std::string file = "",
        int tabs = 4
    )
        : err_out_(err_out)
        , file_(file)
        , tabs_(tabs)
        , pos_cache_(first, last)
    {}

    template<std::sentinel_for<It> Se, class Context, X4Attribute Attr>
    void on_success(It const& first, Se const& last, Context const& /*ctx*/, Attr& attr)
    {
        pos_cache_.annotate(attr, first, last);
    }

    template<std::sentinel_for<It> Se, class Context>
    void on_expectation_failure(It const&, Se const&, Context const& /*ctx*/, expectation_failure<It> const& failure)
    {
        (*this)(failure.where(), "Error! Expecting: " + failure.which() + " here:");
    }

    template<class Context, X4Attribute Attr>
    void on_trace(
        It first,
        std::sentinel_for<It> auto last,
        Context const& /* ctx */,
        Attr const& attr,
        std::string_view rule_name,
        tracer_state const state
    )
    {
        using enum tracer_state;

        switch (state) {
        case pre_parse:
            default_error_handler::print_indent(trace_indent_++);
            err_out_ << '<' << rule_name << '>' << std::endl;
            default_error_handler::print_some("try", first, last);
            break;

        case parse_succeeded:
            default_error_handler::print_some("success", first, last);
            if constexpr (!std::same_as<Attr, unused_type>) {
                default_error_handler::print_indent(trace_indent_);
                err_out_ << "<attributes>";
                traits::print_attribute(err_out_, attr);
                err_out_ << "</attributes>";
                err_out_ << std::endl;
            }
            default_error_handler::print_indent(--trace_indent_);
            err_out_ << "</" << rule_name << '>' << std::endl;
            break;

        case parse_failed:
            default_error_handler::print_indent(trace_indent_);
            err_out_ << "<fail/>" << std::endl;
            default_error_handler::print_indent(--trace_indent_);
            err_out_ << "</" << rule_name << '>' << std::endl;
            break;
        }
    }

private:
    void operator()(It err_pos, std::string const& error_message) const;
    void operator()(It err_first, It err_last, std::string const& error_message) const;

    void operator()(ast::position_tagged const& pos, std::string const& message) const
    {
        auto where = pos_cache_.position_of(pos);
        (*this)(where.begin(), where.end(), message);
    }

    [[nodiscard]] std::ranges::subrange<It>
    position_of(ast::position_tagged const& pos) const
    {
        return pos_cache_.position_of(pos);
    }

    [[nodiscard]] ast::position_cache<std::vector<It>> const&
    get_position_cache() const noexcept
    {
        return pos_cache_;
    }

    // tracer related
    void print_indent(int n) const
    {
        n *= IndentSpaces;
        for (int i = 0; i != n; ++i) {
            err_out_ << ' ';
        }
    }

    void print_some(std::string_view tag, It first, It last) const
    {
        default_error_handler::print_indent(trace_indent_);

        err_out_ << '<' << tag << '>';

        for (int i = 0; first != last && i != CharsToPrint && *first; ++i, ++first) {
            traits::print_token(err_out_, *first);
        }
        err_out_ << "</" << tag << '>' << std::endl;

        // TODO: convert invalid xml characters (e.g. '<') to valid character entities
    }

    void print_file_line(std::size_t line) const;
    void print_line(It line_start, It last) const;
    void print_indicator(It& line_start, It last, char ind) const;
    It get_line_start(It first, It pos) const;
    std::size_t position(It i) const;

    std::ostream& err_out_;
    std::string file_;
    int tabs_;
    ast::position_cache<std::vector<It>> pos_cache_;

    int trace_indent_ = 0;
};

template<std::forward_iterator It>
void default_error_handler<It>::print_file_line(std::size_t line) const
{
    if (file_ != "") {
        err_out_ << "In file " << file_ << ", ";
    } else {
        err_out_ << "In ";
    }

    err_out_ << "line " << line << ':' << '\n';
}

template<std::forward_iterator It>
void default_error_handler<It>::print_line(It start, It last) const
{
    auto end = start;
    while (end != last) {
        auto c = *end;
        if (c == '\r' || c == '\n') break;
        ++end;
    }
    using char_type = typename std::iterator_traits<It>::value_type;
    std::basic_string<char_type> line{start, end};
    err_out_ << x4::to_utf8(line) << '\n';
}

template<std::forward_iterator It>
void default_error_handler<It>::print_indicator(It& start, It last, char ind) const
{
    for (; start != last; ++start) {
        auto c = *start;
        if (c == '\r' || c == '\n') break;
        if (c == '\t') {
            for (int i = 0; i < tabs_; ++i) {
                err_out_ << ind;
            }
        } else {
            err_out_ << ind;
        }
    }
}

template<std::forward_iterator It>
It default_error_handler<It>::get_line_start(It first, It pos) const
{
    It latest = first;
    for (It i = first; i != pos;) {
        if (*i == '\r' || *i == '\n') {
            latest = ++i;
        } else {
            ++i;
        }
    }
    return latest;
}

template<std::forward_iterator It>
std::size_t default_error_handler<It>::position(It i) const
{
    std::size_t line {1};
    typename std::iterator_traits<It>::value_type prev {0};

    for (It pos = pos_cache_.first(); pos != i; ++pos) {
        auto c = *pos;
        switch (c) {
        case '\n':
            if (prev != '\r') ++line;
            break;
        case '\r':
            ++line;
            break;
        default:
            break;
        }
        prev = c;
    }

    return line;
}

template<std::forward_iterator It>
void default_error_handler<It>::operator()(It err_pos, std::string const& error_message) const
{
    It first = pos_cache_.first();
    It last = pos_cache_.last();

    print_file_line(position(err_pos));
    err_out_ << error_message << '\n';

    It start = get_line_start(first, err_pos);
    print_line(start, last);
    print_indicator(start, err_pos, '_');
    err_out_ << "^_" << '\n';
}

template<std::forward_iterator It>
void default_error_handler<It>::operator()(It err_first, It err_last, std::string const& error_message) const
{
    It first = pos_cache_.first();
    It last = pos_cache_.last();

    print_file_line(position(err_first));
    err_out_ << error_message << '\n';

    It start = get_line_start(first, err_first);
    print_line(start, last);
    print_indicator(start, err_first, ' ');
    print_indicator(start, err_last, '~');
    err_out_ << " <<-- Here" << '\n';
}

} // iris::x4

#endif
