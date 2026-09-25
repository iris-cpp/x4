#ifndef IRIS_X4_CORE_LIST_LIKE_PARSER_HPP
#define IRIS_X4_CORE_LIST_LIKE_PARSER_HPP

#include <iris/config.hpp> // IWYU pragma: keep

#include <iris/x4/core/traits/tuple_traits.hpp>
#include <iris/x4/core/traits/variant_traits.hpp>

#include <iris/x4/core/detail/parse_into_container.hpp> // export
#include <iris/x4/core/parser.hpp> // IWYU pragma: export
#include <iris/x4/core/attribute.hpp>
#include <iris/x4/core/container_appender.hpp>

#include <iris/rvariant/rvariant.hpp>
#include <iris/rvariant/variant_helper.hpp>

#include <iris/alloy/tuple.hpp>
#include <iris/alloy/traits.hpp>

#include <type_traits>

namespace iris::x4 {

namespace list_like_parser {

namespace detail {

template<X4NonUnusedAttribute ParserAttr, X4NonUnusedAttribute ExposedAttr>
    // non-variant `ExposedAttr`
struct unwrap_container_candidate
{
    using type = unwrap_single_element_t<
        unwrap_recursive_t<
            typename unwrap_container_appender<ExposedAttr>::type
        >
    >;
};

template<X4NonUnusedAttribute ParserAttr, X4NonUnusedAttribute ExposedVariant>
    requires is_variant_v<unwrap_recursive_t<ExposedVariant>>
struct unwrap_container_candidate<ParserAttr, ExposedVariant>
{
    using type = variant_find_holdable_type<
        unwrap_recursive_t<ExposedVariant>, ParserAttr
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
    using unwrapped_attr_type = unwrap_single_element_t<unwrap_recursive_t<ExposedAttr>>;
    auto& unwrapped_attr = x4::unwrap_single_element(iris::unwrap_recursive(attr));

    if constexpr (is_variant_v<unwrapped_attr_type>) {
        using container_alternative = variant_find_holdable_type<
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
