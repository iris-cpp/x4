#ifndef IRIS_ZZ_X4_CORE_LIST_LIKE_PARSER_HPP
#define IRIS_ZZ_X4_CORE_LIST_LIKE_PARSER_HPP

#include <iris/config.hpp> // IWYU pragma: keep

#include <iris/x4/core/detail/parse_into_container.hpp> // export
#include <iris/x4/core/parser.hpp> // IWYU pragma: export
#include <iris/x4/core/attribute.hpp>

#include <iris/container_traits.hpp>

#include <type_traits>
#include <utility>

namespace iris::x4 {

namespace list_like_parser {

namespace detail {

template<X4NonUnusedAttribute ParserAttr, X4NonUnusedAttribute ExposedAttr>
struct chunk_buffer_impl
{
    using type = std::remove_cvref_t<decltype(x4::detail::ref_or_init_attribute_for<ParserAttr>(std::declval<ExposedAttr&>()))>;
    static_assert(
        !is_variant_v<type>,
        "The variant has no alternative which can hold the container of the parser's attribute"
    );
    static_assert(traits::X4Container<type>);
};

} // detail


// The container which a list-like parser yielding `ParserAttr` writes each parse of `Subject` into.
// The container that `x4::detail::ref_or_init_attribute_for` refers to in `ExposedAttr` is written
// into directly when the parse leaves it unchanged on failure, and through a buffer otherwise.
template<class Subject, X4NonUnusedAttribute ParserAttr, X4NonUnusedAttribute ExposedAttr>
class chunk_buffer
{
    using container_type = detail::chunk_buffer_impl<ParserAttr, ExposedAttr>::type;
    static constexpr bool is_buffered = x4::detail::needs_chunk_buffer<Subject, container_type>;

public:
    explicit constexpr chunk_buffer(container_type& container_attr) noexcept
        : container_attr_(container_attr)
    {}

    [[nodiscard]] constexpr container_type& container() noexcept
    {
        if constexpr (is_buffered) {
            return buffer_;
        } else {
            return container_attr_;
        }
    }

    // Moves the elements of the successful parses into the container
    constexpr void merge()
    {
        if constexpr (is_buffered) {
            iris::container::transfer_from(container_attr_, buffer_);
            iris::container::clear(buffer_);
        }
    }

private:
    container_type& container_attr_;
    std::conditional_t<is_buffered, container_type, unused_type> buffer_{};
};

// A repetition writes its whole value as one new element when the container takes it as-is
// (e.g., the value of `*char_` into `vector<string>` is one string).
//
// Otherwise, each parse of the subject is written into the container as a part.
template<X4NonUnusedAttribute ParserAttr, X4NonUnusedAttribute ExposedAttr>
inline constexpr bool writes_as_one_element = [] {
    using container_type = planner::storage_t<typename detail::chunk_buffer_impl<ParserAttr, ExposedAttr>::type>;
    constexpr planner::node_write_strategy strategy = planner::node_write_strategy_of<
        planner::write_node<container_type, planner::model_value_t<ParserAttr>>
    >;
    return strategy.is_writable && strategy.kind == planner::branch_kind::whole;
}();

template<class Parser, std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4NonUnusedAttribute ExposedAttr>
[[nodiscard]] constexpr bool parse_as_one_element(Parser const& parser, It& first, Se const& last, Context const& ctx, ExposedAttr& attr)
{
    auto& container_attr = x4::detail::ref_or_init_attribute_for<typename parser_traits<Parser>::attribute_type>(attr);
    return x4::detail::parse_into_container_impl_default<Parser>::parse_part(parser, first, last, ctx, container_attr);
}

} // list_like_parser

} // iris::x4

#endif
