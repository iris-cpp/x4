#ifndef IRIS_ZZ_X4_DEBUG_ERROR_REPORTING_HPP
#define IRIS_ZZ_X4_DEBUG_ERROR_REPORTING_HPP

/*=============================================================================
    Copyright (c) 2014 Joel de Guzman
    Copyright (c) 2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <iris/x4/core/unused.hpp>

#include <iris/x4/debug/error_handler.hpp>
#include <iris/x4/debug/print_attribute.hpp>

#include <iris/colorize_format.hpp>

#include <print>
#include <ranges>
#include <filesystem>
#include <iterator>
#include <ostream>
#include <string>
#include <string_view>

namespace iris::x4 {

template<std::forward_iterator It>
class default_error_handler
{
public:
    using iterator_type = It;

    static constexpr int indent_space_width = 2;
    static constexpr int code_points_to_print = 20;
    static constexpr int highlight_chars = 2;

    inline static auto const colorize_cfg = iris::ansi_colorize::colorizer<>::make_config({
        {"$tag",   "fg:rgb(140,140,140)"},
        {"$text",  "fg:rgb(249,190,182)"},
        {"$key",   "fg:rgb(118,118,118)|bold"},
        {"$attr",  "fg:rgb(145,220,254)"},
        {"$fail",  "fg:rgb(141,44,43)|bold"},

        // source highlight for expectation failure
        {"$expect_left",  "bg:rgb(40,40,140)"},
        {"$expect_right", "bg:rgb(120,0,0)"},
    });

    [[nodiscard]] static bool is_internal_rule(std::string_view rule_name) noexcept
    {
        if (rule_name.empty()) return true;
        if (rule_name.starts_with(std::string_view{"__"})) return true;
        return false;
    }

    default_error_handler(It source_first, It source_last, std::ostream* error_out, std::ostream* trace_out, std::filesystem::path file_path = {})
        : source_first_(std::move(source_first))
        , source_last_(std::move(source_last))
        , error_out_(error_out)
        , trace_out_(trace_out)
        , file_path_(std::move(file_path))
    {
    }

    [[nodiscard]] It source_first() const { return source_first_; }
    [[nodiscard]] It source_last() const { return source_last_; }

    [[nodiscard]] std::ostream* error_out() const noexcept { return error_out_; }
    [[nodiscard]] std::ostream* trace_out() const noexcept { return trace_out_; }
    [[nodiscard]] std::filesystem::path const& file_path() const noexcept { return file_path_; }

    //template<std::sentinel_for<It> Se, class Context, X4Attribute Attr>
    //void on_success(It const first, Se const last, Context const& /*ctx*/, Attr& attr)
    //{
    //}

    template<std::sentinel_for<It> Se, class Context>
    void on_expectation_failure(It const, Se const, Context const& /*ctx*/, expectation_failure<It> const& failure)
    {
        this->print_expectation(failure.where(), "error: expecting `" + failure.which() + "` here:");
    }

    template<class Context, X4Attribute Attr>
    void on_trace(
        It first,
        std::sentinel_for<It> auto const last,
        Context const& /*ctx*/,
        Attr const& attr,
        std::string_view rule_name,
        tracer_state const state
    )
    {
        using enum tracer_state;

        if (!this->trace_out()) return;

        switch (state) {
        case pre_parse:
            if (default_error_handler::is_internal_rule(rule_name)) ++tracer_internal_rule_stack_;
            if (tracer_internal_rule_stack_ > 0) break;

            this->print_indent(tracer_indent_++);
            this->print_trace("[$tag]<{}>[/$tag]\n", rule_name);
            this->print_some("try   ", first, last);
            break;

        case parse_succeeded:
            if (default_error_handler::is_internal_rule(rule_name)) {
                --tracer_internal_rule_stack_;
                if (tracer_internal_rule_stack_ >= 0) break;

            } else {
                if (tracer_internal_rule_stack_ > 0) break;
            }
            this->print_some("ok    ", first, last);

            if constexpr (!std::same_as<Attr, unused_type>) {
                this->print_indent(tracer_indent_);

                this->print_trace("attr  ");
                this->print_trace("[$attr]");
                x4::print_attribute(*this->trace_out(), attr);
                this->print_trace("[/$attr]\n");
            }
            this->print_indent(--tracer_indent_);

            this->print_trace("[$tag]</{}>[/$tag]\n", rule_name);
            break;

        case parse_failed:
            if (default_error_handler::is_internal_rule(rule_name)) {
                --tracer_internal_rule_stack_;
                if (tracer_internal_rule_stack_ >= 0) break;

            } else {
                if (tracer_internal_rule_stack_ > 0) break;
            }

            this->print_indent(tracer_indent_);
            this->print_trace("[$fail]fail[/$fail]\n");
            this->print_indent(--tracer_indent_);

            this->print_trace("[$tag]</{}>[/$tag]\n", rule_name);
            break;

        default:
            break;
        }
    }

    void print_line_highlight(std::ranges::subrange<It> const line, It const err_pos) const
    {
        if (!error_out_) return;

        using char_type = std::iterator_traits<It>::value_type;
        using string_view_type = std::basic_string_view<char_type>;

        auto const [left_it, left_count] = iris::unicode::bounded_prev(line.begin(), err_pos, highlight_chars);
        auto const [right_it, right_count] = iris::unicode::bounded_next(err_pos, line.end(), highlight_chars);

        if (left_count > 0) {
            this->print_error(
                "{}[$expect_left]{}[/$expect_left]",
                iris::unicode::transcode<char>(string_view_type{line.begin(), left_it}),
                iris::unicode::transcode<char>(string_view_type{left_it, err_pos})
            );
        }
        if (right_count > 0) {
            this->print_error(
                "[$expect_right]{}[/$expect_right]{}",
                iris::unicode::transcode<char>(string_view_type{err_pos, right_it}),
                iris::unicode::transcode<char>(string_view_type{right_it, line.end()})
            );
        }
        *error_out_ << "\n";
    }

private:
    template<class... Args>
    void print_error(std::string_view fmt_str, Args&&... args) const
    {
        iris::colorize_format_to(*error_out_, colorize_cfg, fmt_str, std::forward<Args>(args)...);
    }

    template<class... Args>
    void print_trace(std::string_view fmt_str, Args&&... args) const
    {
        iris::colorize_format_to(*trace_out_, colorize_cfg, fmt_str, std::forward<Args>(args)...);
    }

    void print_indent(int n) const
    {
        n *= indent_space_width;
        for (int i = 0; i != n; ++i) {
            *trace_out_ << ' ';
        }
    }

    void print_some(char const* tag, It first, It const last) const
    {
        this->print_indent(tracer_indent_);

        if (first == last) {
            this->print_trace("{}[$key]eoi[/$key]\n", tag);
            return;
        }

        this->print_trace("{}[$key]|[/$key]", tag);

        this->print_trace("[$text]");
        x4::print_chars(*trace_out_, first, last, code_points_to_print);
        this->print_trace("[/$text][$key]|[/$key]\n");
    }

    void print_expectation(It err_pos, std::string_view error_message) const
    {
        if (!error_out_) return;

        x4::skip_whitespace_for_print(err_pos, source_last_);

        this->print_file_line(x4::calc_line_number(source_first_, err_pos));
        *error_out_ << error_message << '\n';

        std::ranges::subrange<It> const line{
            x4::fetch_line_start(source_first_, err_pos),
            x4::fetch_line_last(err_pos, source_last_)
        };
        this->print_line_highlight(line, err_pos);
    }

    void print_file_line(int line) const
    {
        if (!error_out_) return;

        if (file_path_.empty()) {
            std::print(*error_out_, "[in-memory source]({}): ", line);
        } else {
            std::print(*error_out_, "{}({}): ", file_path_.string(), line);
        }
    }

    It source_first_, source_last_;
    std::ostream* error_out_ = nullptr, *trace_out_ = nullptr;
    std::filesystem::path file_path_;

    int tracer_internal_rule_stack_ = 0;
    int tracer_indent_ = 0;
};

} // iris::x4

#endif
