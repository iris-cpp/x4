# Iris.X4 [![Build Status](https://github.com/iris-cpp/x4/actions/workflows/ci.yml/badge.svg)](https://github.com/iris-cpp/x4/actions/workflows/ci.yml)

[**Iris.X4**](https://github.com/iris-cpp/x4) is a modern, header-only PEG parser combinator library for C++23. It lets you write EBNF-like grammars directly in C++ with deterministic, top-down parsing (ordered choice, lookahead, greedy repetition).

X4 succeeds [Boost.Spirit.X3](https://github.com/boostorg/spirit) and is redesigned for clarity, maintainability, and performance using modern C++ features such as concepts and constexpr.

X4 scales from quick prototypes to production parsers for DSLs, data formats, and interpreters.

### Supported Environments

- C++23 and C++26
- GCC 14
- Clang 21
- MSVC 2022 and 2026

## How to use X4

### Step 1. Setup the Boost superproject

Note: Boost dependency is going to be removed entirely in the near future.

#### For applications:

```console
cd my_app

git submodule add https://github.com/boostorg/boost.git modules/boost
cd modules/boost

git submodule add https://github.com/iris-cpp/x4.git libs/iris_x4
```

#### For X4 development:

```console
git clone https://github.com/boostorg/boost.git
cd boost

git clone https://github.com/iris-cpp/x4.git libs/iris_x4
```

### Step 2. Setup the dependent Boost libraries

Note: Boost dependency is going to be removed entirely in the near future.

```console
git submodule update --init --depth 1 --recursive -- \
    tools/build tools/boost_install libs/assert libs/bind libs/config \
    libs/container_hash libs/core libs/describe libs/detail \
    libs/function libs/function_types libs/functional libs/fusion \
    libs/integer libs/io libs/mp11 libs/mpl libs/predef libs/preprocessor \
    libs/static_assert libs/throw_exception libs/tuple libs/type_index \
    libs/type_traits libs/typeof libs/utility libs/variant

# Linux
./bootstrap.sh
./b2 headers

# Windows
.\bootstrap.bat
.\b2 headers
```

### Step 3. Start using X4

#### For applications:

Edit your `CMakeLists.txt`:

```cmake
add_subdirectory(modules/boost/libs/iris_x4)
target_link_libraries(my_app PRIVATE Iris::X4)
```

#### For developing X4:

```console
cd iris_x4
cmake -B build
```
