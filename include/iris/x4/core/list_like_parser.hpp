#ifndef IRIS_X4_CORE_LIST_LIKE_PARSER_HPP
#define IRIS_X4_CORE_LIST_LIKE_PARSER_HPP

#include <iris/config.hpp>

#include <iris/x4/traits/tuple_traits.hpp>
#include <iris/x4/traits/variant_traits.hpp>

#include <iris/x4/core/detail/parse_into_container.hpp> // export
#include <iris/x4/core/parser.hpp> // export
#include <iris/x4/core/attribute.hpp>
#include <iris/x4/core/container_appender.hpp>

#include <iris/rvariant/rvariant.hpp>
#include <iris/rvariant/variant_helper.hpp>

#include <iris/alloy/traits.hpp>

namespace iris::x4 {

namespace list_like_parser {

namespace detail {

template<X4NonUnusedAttribute ParserAttr, X4NonUnusedAttribute ExposedAttr>
    // non-variant `ExposedAttr`
struct unwrap_container_candidate
{
    using type = traits::unwrap_if_single_element_tuple_like<
        unwrap_recursive_type<
            typename unwrap_container_appender<ExposedAttr>::type
        >
    >::type;
};

template<X4NonUnusedAttribute ParserAttr, X4NonUnusedAttribute ExposedVariant>
    requires traits::is_variant_v<unwrap_recursive_type<ExposedVariant>>
struct unwrap_container_candidate<ParserAttr, ExposedVariant>
{
    using type = traits::variant_find_holdable_type<
        unwrap_recursive_type<ExposedVariant>, ParserAttr
    >::type;
};

template<X4NonUnusedAttribute ParserAttr, X4NonUnusedAttribute ExposedAttr>
struct chunk_buffer_impl
{
    using type = unwrap_container_candidate<ParserAttr, ExposedAttr>::type;
    static_assert(traits::X4Container<typename unwrap_container_candidate<ParserAttr, ExposedAttr>::type>);
};

} // detail


template<X4NonUnusedAttribute ParserAttr, X4NonUnusedAttribute ExposedAttr>
using chunk_buffer = detail::chunk_buffer_impl<ParserAttr, ExposedAttr>::type;


template<X4NonUnusedAttribute ParserAttr, X4NonUnusedAttribute ExposedAttr>
[[nodiscard]] constexpr auto& get_container(ExposedAttr& attr)
{
    using unwrapped_attr_type = traits::unwrap_single_element_plain<
        unwrap_recursive_type<ExposedAttr>
    >::type;
    auto& unwrapped_attr = traits::do_unwrap_if_single_element_tuple_like(iris::unwrap_recursive(attr));

    if constexpr (traits::is_variant_v<unwrapped_attr_type>) {
        using container_alternative = traits::variant_find_holdable_type<
            unwrapped_attr_type, ParserAttr
        >::type;

        if (iris::holds_alternative<container_alternative>(unwrapped_attr)) {
            return iris::unsafe_get<container_alternative>(unwrapped_attr);
        } else {
            return unwrapped_attr.template emplace<container_alternative>();
        }

    } else {
        return unwrapped_attr;
    }
}

template<traits::X4Container ChunkBuf, traits::X4Container ExposedAttr>
constexpr void successful_merge_into(ChunkBuf& chunk_buf, ExposedAttr& container_attr)
{
    traits::append(
        container_attr,
        std::make_move_iterator(traits::begin(chunk_buf)),
        std::make_move_iterator(traits::end(chunk_buf))
    );
    traits::clear(chunk_buf);
}

} // list_like_parser

} // iris::x4

#endif
