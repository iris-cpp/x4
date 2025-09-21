#ifndef BOOST_SPIRIT_X4_AST_POSITION_TAGGED_HPP
#define BOOST_SPIRIT_X4_AST_POSITION_TAGGED_HPP

/*=============================================================================
    Copyright (c) 2014 Joel de Guzman
    Copyright (c) 2025 Nana Sakisaka

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <concepts>
#include <ranges>

namespace boost::spirit::x4 {

    struct position_tagged
    {
        // Use this to annotate an AST with the iterator position.
        // These ids are used as a key to the position_cache (below)
        // and marks the start and end of an AST node.
        int id_first = -1;
        int id_last = -1;
    };

    template <typename Container>
    class position_cache
    {
    public:
        using iterator_type = typename Container::value_type;

        position_cache(iterator_type first, iterator_type last)
            : first_(first)
            , last_(last)
        {}

        template <typename AST>
            requires std::derived_from<AST, position_tagged>
        [[nodiscard]] std::ranges::subrange<iterator_type>
        position_of(AST const& ast) const
        {
            return std::ranges::subrange<iterator_type>{
                positions_.at(ast.id_first),
                positions_.at(ast.id_last)
            };
        }

        template <typename AST>
            requires (!std::derived_from<AST, position_tagged>)
        [[nodiscard]] std::ranges::subrange<iterator_type>
        position_of(AST const&) const
        {
            // returns an empty position
            return std::ranges::subrange<iterator_type>{};
        }

        // This will catch all nodes except those inheriting from position_tagged
        template <typename AST>
            requires (!std::derived_from<AST, position_tagged>)
        void annotate(AST&, iterator_type const&, iterator_type const&)
        {
            // (no-op) no need for tags
        }

        template <typename AST>
            requires std::derived_from<AST, position_tagged>
        void annotate(AST& ast, iterator_type first, iterator_type last)
        {
            ast.id_first = int(positions_.size());
            positions_.push_back(std::move(first));
            ast.id_last = int(positions_.size());
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

} // boost::spirit::x4

#endif
