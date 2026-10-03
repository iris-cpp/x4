#ifndef IRIS_ZZ_X4_CORE_LIST_LIKE_PARSER_HPP
#define IRIS_ZZ_X4_CORE_LIST_LIKE_PARSER_HPP

#include <iris/config.hpp> // IWYU pragma: keep

#include <iris/x4/core/traits/tuple_traits.hpp>
#include <iris/x4/core/traits/variant_traits.hpp>

#include <iris/x4/core/detail/parse_into_container.hpp> // export
#include <iris/x4/core/parser.hpp> // IWYU pragma: export
#include <iris/x4/core/attribute.hpp>

#include <iris/rvariant/rvariant.hpp>
#include <iris/rvariant/variant_helper.hpp>

#include <iris/alloy/tuple.hpp>
#include <iris/alloy/traits.hpp>

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


// The container which a list-like parser yielding `ParserAttr` appends into, as
// `x4::detail::ref_or_init_attribute_for` refers to it in `ExposedAttr`
template<X4NonUnusedAttribute ParserAttr, X4NonUnusedAttribute ExposedAttr>
using chunk_buffer = detail::chunk_buffer_impl<ParserAttr, ExposedAttr>::type;

// A repetition writes its whole value as one new element when the container takes it as-is
// (e.g., the value of `*char_` into `vector<string>` is one string).
//
// Otherwise, each parse of the subject is written into the container as a part.
template<X4NonUnusedAttribute ParserAttr, X4NonUnusedAttribute ExposedAttr>
inline constexpr bool writes_as_one_element = [] {
    using container_type = planner::storage_t<chunk_buffer<ParserAttr, ExposedAttr>>;
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

template<traits::X4Container ChunkBuf, traits::X4Container ExposedAttr>
constexpr void successful_merge_into(ChunkBuf& chunk_buf, ExposedAttr& container_attr)
{
    iris::container::append_range(container_attr, chunk_buf | std::views::as_rvalue);
    iris::container::clear(chunk_buf);
}

} // list_like_parser

} // iris::x4

#endif
