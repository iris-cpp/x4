#ifndef IRIS_X4_DEBUG_PRINT_ATTRIBUTE_HPP
#define IRIS_X4_DEBUG_PRINT_ATTRIBUTE_HPP

/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2001-2011 Hartmut Kaiser
    Copyright (c) 2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
================================================_==============================*/

#include <iris/x4/core/attribute.hpp>
#include <iris/x4/traits/attribute_category.hpp>
#include <iris/x4/traits/variant_traits.hpp>

#include <iris/rvariant/rvariant_io.hpp>

#include <iris/alloy/utility.hpp>

#ifdef IRIS_X4_UNICODE
# include <iris/x4/char_encoding/unicode.hpp>
#endif

#include <iosfwd>

namespace iris::x4::traits {

template<X4Attribute Attr>
void print_attribute(std::ostream& os, Attr const& attr_);

namespace detail {

template<class Out>
struct print_tuple_like
{
    print_tuple_like(Out& out)
        : out(out)
        , is_first(true)
    {}

    using result_type = void;

    template<class T>
    void operator()(T const& val) const
    {
        if (is_first) {
            is_first = false;
        } else {
            out << ", ";
        }
        traits::print_attribute(out, val);
    }

    Out& out;  // NOLINT(cppcoreguidelines-avoid-const-or-ref-data-members)
    mutable bool is_first;
};

// print elements in a variant
template<class Out>
struct print_visitor
{
    explicit print_visitor(Out& out)
        : out(out)
    {}

    template<class T>
    void operator()(T const& val) const
    {
        traits::print_attribute(out, val);
    }

    Out& out;  // NOLINT(cppcoreguidelines-avoid-const-or-ref-data-members)
};

} // detail

template<class T>
struct print_attribute_debug
{
    static void call(std::ostream& out, unused_type const&)
    {
        out << "unused";
    }

    static void call(std::ostream& out, unused_container_type const&)
    {
        out << "unused_container";
    }

    static void call(std::ostream& out, CategorizedAttr<plain_attr> auto const& val)
    {
        if constexpr (std::formattable<T, char>) {
            std::format_to(std::ostreambuf_iterator{out}, "{}", val);
        } else {
            out << val;
        }
    }

#ifdef IRIS_X4_UNICODE
    static void call(std::ostream& out, char_encoding::unicode::char_type const& val)
    {
        if (val >= 0 && val < 127) {
            if (iscntrl(val)) { // TODO
                out << "\\" << std::oct << int(val) << std::dec;
            } else if (isprint(val)) {
                out << char(val);
            } else {
                out << "\\x" << std::hex << int(val) << std::dec;
            }

        } else {
          out << "\\x" << std::hex << int(val) << std::dec;
        }
    }

    static void call(std::ostream& out, char const& val)
    {
        print_attribute_debug::call(out, static_cast<char_encoding::unicode::char_type>(val));
    }
#endif

    // for tuple-likes
    static void call(std::ostream& out, CategorizedAttr<tuple_attr> auto const& val)
    {
        out << '[';
        alloy::for_each(val, detail::print_tuple_like<std::ostream>(out));
        out << ']';
    }

    template<CategorizedAttr<container_attr> T_>
        requires (!std::is_same_v<T_, unused_container_type>)
    static void call(std::ostream& out, T_ const& val)
    {
        out << '[';
        bool is_first = true;
        auto last = traits::end(val);
        for (auto it = traits::begin(val); it != last; ++it) {
            if (is_first) {
                is_first = false;
            } else {
                out << ", ";
            }
            traits::print_attribute(out, *it);
        }
        out << ']';
    }

    // for variant types
    static void call(std::ostream& out, CategorizedAttr<variant_attr> auto const& val)
    {
        iris::visit(detail::print_visitor<std::ostream>{out}, val);
    }

    static void call(std::ostream& out, CategorizedAttr<optional_attr> auto const& val)
    {
        if (val) {
            traits::print_attribute(out, *val);
        } else {
            out << "[empty]";
        }
    }
};

template<X4Attribute Attr>
void print_attribute(std::ostream& os, Attr const& attr_)
{
    print_attribute_debug<Attr>::call(os, attr_);
}

} // iris::x4::traits

#endif
