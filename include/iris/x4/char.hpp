#ifndef IRIS_ZZ_X4_CHAR_HPP
#define IRIS_ZZ_X4_CHAR_HPP

/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <iris/config.hpp> // IWYU pragma: keep

#include <iris/x4/char_string_literal.hpp> // IWYU pragma: export

#include <iris/x4/char/char.hpp> // IWYU pragma: export
#include <iris/x4/char/char_parser.hpp> // IWYU pragma: export
#include <iris/x4/char/char_class.hpp> // IWYU pragma: export
#include <iris/x4/char/char_set.hpp> // IWYU pragma: export

#ifdef IRIS_X4_UNICODE
# include <iris/x4/char/unicode_char_class.hpp> // IWYU pragma: export
#endif

#endif
