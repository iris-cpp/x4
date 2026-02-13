# Iris.X4 [![Build Status](https://github.com/iris-cpp/x4/actions/workflows/ci.yml/badge.svg)](https://github.com/iris-cpp/x4/actions/workflows/ci.yml)

[**Iris.X4**](https://github.com/iris-cpp/x4) is a modern, header-only PEG parser combinator library for C++23. It lets you write EBNF-like grammars directly in C++ with deterministic, top-down parsing (ordered choice, lookahead, greedy repetition).

X4 succeeds [Boost.Spirit.X3](https://github.com/boostorg/spirit) and is redesigned for clarity, maintainability, and performance using modern C++ features such as concepts and constexpr. In contrast to the original library, Iris.X4 has no dependencies on the Boost Libraries.

X4 scales from quick prototypes to production parsers for DSLs, data formats, and interpreters.

### Supported Environments

- C++23 and C++26
- GCC 14
- Clang 21
- MSVC 2022 and 2026

## How to use X4

Add Iris.X4 to your application:

```console
cd my_app
git submodule add https://github.com/iris-cpp/x4.git modules/iris_x4
```

Edit your `CMakeLists.txt`:

```cmake
add_subdirectory(modules/iris_x4)
target_link_libraries(my_app PRIVATE Iris::X4)
```
