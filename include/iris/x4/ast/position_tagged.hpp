#ifndef IRIS_X4_AST_POSITION_TAGGED_HPP
#define IRIS_X4_AST_POSITION_TAGGED_HPP

/*=============================================================================
    Copyright (c) 2014 Joel de Guzman
    Copyright (c) 2025 Nana Sakisaka

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <iris/x4/core/attribute.hpp>

#include <concepts>
#include <ranges>

namespace boost::spirit::x4 {

namespace ast {

struct position_tagged
{
    // Use this to annotate an AST with the iterator position.
    // These ids are used as a key to the position_cache (below)
    // and marks the start and end of an AST node.
    int id_first = -1;
    int id_last = -1;
};

template<class Container>
class position_cache
{
public:
    using iterator_type = typename Container::value_type;

    position_cache(iterator_type first, iterator_type last)
        : first_(first)
        , last_(last)
    {}

    template<X4Attribute Attr>
        requires std::derived_from<Attr, position_tagged>
    [[nodiscard]] std::ranges::subrange<iterator_type>
    position_of(Attr const& attr) const
    {
        return std::ranges::subrange<iterator_type>{
            positions_.at(attr.id_first),
            positions_.at(attr.id_last)
        };
    }

    template<X4Attribute Attr>
        requires (!std::derived_from<Attr, position_tagged>)
    [[nodiscard]] std::ranges::subrange<iterator_type>
    position_of(Attr const&) const
    {
        // returns an empty position
        return std::ranges::subrange<iterator_type>{};
    }

    // This will catch all nodes except those inheriting from position_tagged
    template<X4Attribute Attr>
        requires (!std::derived_from<Attr, position_tagged>)
    static void annotate(Attr&, iterator_type const&, iterator_type const&)
    {
        // (no-op) no need for tags
    }

    template<X4Attribute Attr>
        requires std::derived_from<Attr, position_tagged>
    void annotate(Attr& attr, iterator_type first, iterator_type last)
    {
        attr.id_first = static_cast<int>(positions_.size());
        positions_.push_back(std::move(first));
        attr.id_last = static_cast<int>(positions_.size());
        positions_.push_back(std::move(last));
    }

    [[nodiscard]] Container const&
    get_positions() const noexcept
    {
        return positions_;
    }

    iterator_type first() const { return first_; }
    iterator_type last() const { return last_; }

private:
    Container positions_;
    iterator_type first_;
    iterator_type last_;
};

} // ast

using position_tagged [[deprecated("Use `ast::`")]] = ast::position_tagged;

template<class Container>
using position_cache [[deprecated("Use `ast::`")]] = ast::position_cache<Container>;

} // boost::spirit::x4

#endif
