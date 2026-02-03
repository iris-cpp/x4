#ifndef IRIS_X4_DIRECTIVE_WITH_LOCAL_HPP
#define IRIS_X4_DIRECTIVE_WITH_LOCAL_HPP

#include <iris/x4/core/parser.hpp>
#include <iris/x4/core/context.hpp>

#include <concepts>
#include <iterator>
#include <type_traits>
#include <utility>

namespace boost::spirit::x4 {

namespace contexts {

// Points to the innermost local variable created by
// `x4::with_local<T>[...]` directive.
//
// - If you need multiple local variables, use `std::tuple<...>` for `T`.
// - If you need distinct local variables in a nested grammar structure,
//   don't use `x4::local_var`; use your own context tag type.
struct local_var
{
    static constexpr bool is_unique = true;
};

} // contexts

namespace detail {

struct local_var_fn
{
    template<class Context>
    [[nodiscard]] static constexpr decltype(auto) operator()(Context const& ctx) noexcept
    {
        return x4::get<contexts::local_var>(ctx);
    }
};

} // detail

inline namespace cpos {

[[maybe_unused]] inline constexpr detail::local_var_fn _local_var{};

} // cpos


template<class Subject, class ID, class T>
struct with_local_directive : proxy_parser<Subject, with_local_directive<Subject, ID, T>>
{
    static_assert(std::default_initializable<T>);

    using id_type = ID;
    using value_type = T;

    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
    [[nodiscard]] constexpr bool
    parse(It& first, Se const& last, Context const& ctx, Attr& attr) const
        noexcept(
            std::is_nothrow_default_constructible_v<T> &&
            x4::is_nothrow_parsable_v<
                Subject, It, Se,
                decltype(x4::replace_first_context<ID>(ctx, std::declval<T&>())),
                Attr
            >
        )
    {
        // `x4::make_context(...)` cannot be used here as it invokes infinite recursive instantiation.

        T local_var{}; // value-initialize
        return this->subject.parse(first, last, x4::replace_first_context<ID>(ctx, local_var), attr);
    }
};

namespace detail {

template<class ID, class T>
struct with_local_gen
{
    template<class Subject>
    [[nodiscard]] constexpr with_local_directive<std::remove_cvref_t<Subject>, ID, T>
    operator[](Subject&& subject) const // TODO: MSVC 2022 does not properly handle static operator[]
        noexcept(std::is_nothrow_constructible_v<with_local_directive<std::remove_cvref_t<Subject>, ID, T>, Subject>)
    {
        return with_local_directive<std::remove_cvref_t<Subject>, ID, T>{
            std::forward<Subject>(subject)
        };
    }
};

} // detail

namespace parsers::directive {

template<class T, class ID = contexts::local_var>
[[maybe_unused]] inline constexpr detail::with_local_gen<ID, T> with_local{};

} // parsers::directive

using parsers::directive::with_local;

} // boost::spirit::x4

#endif
