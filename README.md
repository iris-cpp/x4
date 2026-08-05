# Iris.X4 [![Build Status](https://github.com/iris-cpp/x4/actions/workflows/ci.yml/badge.svg)](https://github.com/iris-cpp/x4/actions/workflows/ci.yml)

[**Iris.X4**](https://github.com/iris-cpp/x4) is a modern, header-only PEG parser combinator library for C++23. It lets you write EBNF-like grammars directly in C++ with deterministic, top-down parsing (ordered choice, lookahead, greedy repetition).

X4 succeeds [Boost.Spirit.X3](https://github.com/boostorg/spirit) and is redesigned for clarity, maintainability, and performance using modern C++ features such as concepts and constexpr. In contrast to the original library, Iris.X4 has no dependencies on the Boost Libraries.

X4 scales from quick prototypes to production parsers for DSLs, data formats, and interpreters.

### Supported Environments

- C++23 and C++26
- GCC 14
- Clang 21
- MSVC 2026

## How to use X4

Add Iris.X4 to your application:

```console
cd my_app
git submodule add https://github.com/iris-cpp/x4.git modules/x4
```

Edit your `CMakeLists.txt`:

```cmake
add_subdirectory(modules/x4)
target_link_libraries(my_app PUBLIC Iris::X4)
```


## Core Concepts

### attribute

An *attribute* is the value produced by a successful parse. It represents the semantic result of a parser after it consumes input, and is propagated through combinators, directives, and rules according to their transformation rules. Attributes may be primitive values, containers, or user-defined types, and can be constructed, transformed, or suppressed depending on the parser expression.

Some parsers do not produce an attribute. For such parsers, the attribute type is `x4::unused_type`.

### exposed attribute

An *exposed attribute* is an attribute instance supplied by the parser's caller. Its type may either match the parser's own attribute type or be entirely different, depending on the caller.

```cpp
// exposed attr: `int`, parser attr: `int`
int i;
x4::parse("...", x4::int_, i);

// exposed attr: `long`, parser attr: `int`
long l;
x4::parse("...", x4::int_, l);
```

When a parser is instantiated as a plain variable and has not yet been passed to `x4::parse(...)`, its exposed attribute type is *unspecified*. In this state, the parser behaves as a generic function and may accept any user-provided type as input.

```cpp
// exposed attr: unspecified, parser attr: `int`
constexpr auto p = x4::int_;
```

The `x4::sequence` parser (normally instantiated via `a >> b` syntax) yields a special tuple attribute. This attribute is generic in nature and can be transformed into any tuple-like or sequence-like concrete type, depending on the caller.

To adapt a user-defined type as an tuple-like type, use `IRIS_ALLOY_ADAPT_STRUCT` from [<iris/alloy/adapt.hpp>](https://github.com/iris-cpp/x4/blob/main/include/iris/alloy/adapt.hpp).

```cpp
// parser attr: `iris::alloy::tuple<int, int>`
constexpr auto p = x4::int_ >> x4::int_;

// exposed attr: `std::vector<int>`
std::vector<int> ints;
x4::parse("...", p, ints);

// exposed attr: `std::tuple<int, int>`
std::tuple<int, int> int_int_tup;
x4::parse("...", p, int_int_tup);

// exposed attr: `Point`
struct Point { int x, y; };
IRIS_ALLOY_ADAPT_STRUCT(Point, x, y);
Point point;
x4::parse("...", p, point);
```

When multiple nested parser invocations are involved, the exposed attribute type may vary depending on context.

```cpp
long long result;
x4::parse(
    "...",
    x4::as<int>( // <-- exposed attr: `long long`, parser attr: `int`
        x4::short_ // <-- exposed attr: `int`, parser attr: `short`
    ),
    result
);
```

### semantic action

A *semantic action* is a user-provided invocable object (usually a lambda) that is executed when a parser successfully matches its input. It is used to inspect, transform, or validate the parsed result, and may optionally influence control flow by accepting or rejecting the match. Semantic actions operate on the current parsing context and the attribute produced by the parser, allowing fine-grained post-processing of successful parses.

The primary syntax for attaching a semantic action is `p.on_match(f)`, where `f` is invoked after `p` matches. The callable may observe the matched attribute, access contextual information, and optionally return a boolean to either accept the result or force the parser to treat the match as a failure, enabling backtracking when appropriate.

The signature of a semantic action is `[](auto&& ctx) { /* ... */ }`.

> While highly flexible and convenient, **it is generally discouraged to introduce semantic actions prematurely as part of a language's syntax definition,** since most grammars can be expressed purely through combinations of concrete parsers (typically `x4::rule` parsers) when they are properly structured.
>
> The primary intended use of semantic actions is to handle cases that require ad hoc transformation, such as constructing a binary operator object through a more complex algorithm like precedence climbing.


## Directory Structure

Each parser header is organized into subdirectories as needed, but there are several special directories that contain very specific kinds of components. These are described below.

#### `core/`

The core components of X4. In contrast to the facilities in the `traits/` directory, the core components are not intended to be user-customizable.

> **Note for contributors:** every non-detail header in this directory must start with `#include <iris/config.hpp>`.

#### `traits/`

Customizable type traits that allow user-defined types to participate in parser logic or attribute processing. A trait may alter parser semantics, type classification, compatibility checks, attribute propagation, storage, conversion, or transformation.

#### `operator/`

Directives or combinators that use overloaded C++ operators as their composition syntax. They form the fundamental grammar-composition vocabulary, including sequence, alternative, repetition, optionality, predicates, difference, and list composition.

#### `directive/`

Parser adapters or combinators that operate on one or more subject parsers and modify parsing behavior, context, control flow, repetition, or interpretation. They do not use special C++ operators for composition, but normally use subscript syntax such as `directive[p]` to express the combination. Facilities whose primary purpose is value production or attribute representation may instead belong to `attribute/`.

#### Other Subdirectories

Remaining facilities that do not fit the categories described above are organized into additional subdirectories when a clear semantic grouping warrants it.


## Quick Reference

The descriptions below focus on recognition behavior and omit many details concerning attribute propagation and customization.

### Operators

| Syntax   | Attribute | Meaning                                                                                                                                                   |
| -------- | --------- | --------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `a \| b` | [`rvariant<A, B>`](https://github.com/iris-cpp/iris/blob/main/doc/rvariant.adoc) | Try `a`. If it fails normally, restore the input position and try `b`.                               |
| `a >> b` | `tuple<A, B>` | Parse `a`, followed by `b`. If either parser fails normally, restore the input position to the beginning of the sequence.                                 |
| `a > b`  | `tuple<A, B>` | Parse `a`, followed by an expected `b`. Failure of `b` records an expectation failure and prevents ordinary backtracking.<br>Equivalent to `a >> expect[b]`. |
| `*p`     | `vector<P>` | Parse zero or more occurrences of `p`.                                                                                                                    |
| `+p`     | `vector<P>` | Parse one or more occurrences of `p`.                                                                                                                     |
| `a % b`  | `vector<A>` | Parse one or more occurrences of `a`, delimited by `b`.                                                                                                   |
| `a - b`  | `A` | Parse `a` only when `b` does not match at the same input position. The test of `b` does not consume input.<br>Equivalent to `!b >> a`.                                                |
| `-p`     | `optional<P>` | Parse zero or one occurrence of `p`.                                                                                                                      |
| `&p`     | `unused_type` | Succeed when `p` matches, without consuming input or producing its attribute.                                                                             |
| `!p`     | `unused_type` | Succeed when `p` does not match, without consuming input or producing an attribute.                                                                       |

#### Notes

- `a`, `b`, `p`: parsers.
- `A`, `B`, `P`: the corresponding attribute types of the parsers, respectively.
- **`unused_type` compaction:**
  When `unused_type` appears as an element of a tuple, that element is discarded. After compaction, if the tuple contains only a single element, the tuple is *unwrapped* and replaced with the element's type.
- **Tuple-to-container conversion**:
  When the attribute type is a tuple and the exposed attribute is a container type, each decomposed tuple element is appended to the container, unless the container's value type is itself a tuple-like type and the components match exactly.
- **Optional elements in containers**:
  When the attribute type is `optional<T>` and the exposed attribute is a container type, the contained object `T` is appended to the container if and only if the optional instance contains a value; otherwise, it is discarded.

### Major Directives

These facilities commonly appear in production language grammars.

| Syntax                 | Meaning                                                                                                                                     |
| ---------------------- | ------------------------------------------------------------------------------------------------------------------------------------------- |
| `p.on_match(f)`        | After `p` matches, invoke the semantic action `f`. The action may inspect or modify the parsing context and may reject the match.           |
| `expect[p]`            | Parse `p`. If it fails, record an expectation failure that prevents ordinary backtracking and alternative recovery. Usually invoked indirectly via the `a > b` syntax. |
| `lexeme[p]`            | Perform the normal pre-skip, then parse `p` with automatic skipping disabled inside it.                                                     |
| `with<ID>(value)[p]`   | Bind `value` to the context id `ID` while parsing `p`.<br>The instance be fetched via `x4::get<ID>(ctx)` in semantic action.               |
| `with_local<T, ID>[p]`     | Create a value-initialized local value of type `T` for each invocation of `p` and bind it to the context id `ID`. If `ID` is omitted, the default local-variable context id (`x4::contexts::local_var`) is used.<br>The instance be fetched via `x4::get<ID>(ctx)` or `x4::_local_var(ctx)`, respectively, in semantic action. |

### Minor Directives

These facilities are used less frequently in ordinary language grammars.

| Syntax                           | Meaning                                                                                                                                                     |
| -------------------------------- | ----------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `omit[p]`                        | Parse `p`, but suppress its attribute.                                                                                                                      |
| `skip(s)[p]`                     | Parse `p` using `s` as its active skipper, replacing the previously active skipper within the directive.                                                    |
| `no_skip[p]`                     | Parse `p` without performing a pre-skip and with automatic skipping disabled inside it. Most ordinary contiguous-token parsing can instead use `lexeme[p]`. |
| `no_case[p]`                     | Apply case-insensitive character and string comparison while parsing `p`.                                                                                   |
| `matches[p]`                     | Attempt to parse `p` and expose the result as `bool`: `true` when `p` matches and `false` when it fails normally.                                           |
| `repeat(n)[p]`                   | Parse exactly `n` occurrences of `p`.                                                                                                                       |
| `repeat(min, max)[p]`            | Parse between `min` and `max` occurrences of `p`, inclusive.                                                                                                |
| `repeat(min, x4::repeat_inf)[p]` | Parse at least `min` occurrences of `p`, with no upper limit.                                                                                               |
| `without<IDs...>[p]`             | Remove every context entry whose key is one of `IDs...` while parsing `p`.<br>Useful for sanitizing the context type correlated with the `x4::rule` type required by `IRIS_X4_INSTANTIATE`. |

### Attribute Facilities

| Syntax               | Meaning                                                                                                                                                                     |
| -------------------- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `as<T>(p)`           | Force `p` to expose `T` as its attribute type. When the outer destination has another compatible type, parse into a temporary `T` and move the result into the destination. |
| `fixed_value(value)` | Always succeed without consuming input and copy the stored `value` into the exposed attribute.                                                                              |
| `reset_value<T>`     | Always succeed without consuming input and reset the exposed attribute. Containers are cleared; other values are assigned a value-initialized instance.                     |
| `unique_ptr(p)`      | Expose a `std::unique_ptr` attribute and parse `p` into its pointee. Deduce the pointee type from the attribute of `p`.                                                     |
| `unique_ptr<T>(p)`   | Expose a `std::unique_ptr<T>` attribute and parse `p` into its pointee. An optional deleter type `D` may also be specified.                                                 |
| `shared_ptr(p)`      | Expose a `std::shared_ptr` attribute and parse `p` into its pointee. Deduce the pointee type from the attribute of `p`.                                                     |
| `shared_ptr<T>(p)`   | Expose a `std::shared_ptr<T>` attribute and parse `p` into its pointee. An optional deleter type `D` may also be specified.                                                 |

### Primitives

| Syntax           | Meaning                                                                                                                                                        |
| ---------------- | -------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `eol`            | Match and consume an end-of-line sequence: `"\n"`, `"\r"`, or `"\r\n"`.                                                             |
| `eoi`            | Succeed only when the input has been exhausted.                                                                                     |
| `eps`            | Always succeed without consuming input.                                                                                             |
| `eps(cond)` | Succeed without consuming input when `cond` is `true`.                                                                         |
| `eps(f)`         | Lazily invoke `f` and succeed without consuming input when it returns `true`. The callable may accept the parsing context or no arguments. |

### Character and String Parsers

| Syntax            | Meaning                                                                                      |
| ----------------- | -------------------------------------------------------------------------------------------- |
| `lit('a')`        | Match the character `'a'` without producing an attribute.                                    |
| `lit("str")`      | Match the exact string `"str"` without producing an attribute.                               |
| `char_`           | Match any valid character and expose the matched character.                                  |
| `char_('a')`      | Match the character `'a'` and expose it.                                                     |
| `char_("abc")`    | Match one character from the set `{'a', 'b', 'c'}` and expose it.                            |
| `char_("a-zA-Z")` | Match and expose one character from the specified character set. A hyphen defines an inclusive range, so this example matches an alphabet. |
| `char_('a', 'z')` | Match one character in the inclusive range `'a'` through `'z'` and expose it.                |
| `~char_(...)`     | Match and expose one character not accepted by the enclosed character parser, set, or range. |
| `string("str")`   | Match the exact string `"str"` and expose the matched string.                                |

### Numeric Parsers

#### Boolean Parsers

| Syntax   | Meaning                                                                |
| -------- | ---------------------------------------------------------------------- |
| `bool_`  | Match `"true"` or `"false"` and expose the corresponding `bool` value. |
| `true_`  | Match `"true"` and expose `true`.                                      |
| `false_` | Match `"false"` and expose `false`.                                    |

#### Signed Integer Parsers

| Syntax      | Meaning                                                         |
| ----------- | --------------------------------------------------------------- |
| `short_`    | Parse a signed integer and expose the result as `short`.        |
| `int_`      | Parse a signed integer and expose the result as `int`.          |
| `long_`     | Parse a signed integer and expose the result as `long`.         |
| `long_long` | Parse a signed integer and expose the result as `long long`.    |
| `int8`      | Parse a signed integer and expose the result as `std::int8_t`.  |
| `int16`     | Parse a signed integer and expose the result as `std::int16_t`. |
| `int32`     | Parse a signed integer and expose the result as `std::int32_t`. |
| `int64`     | Parse a signed integer and expose the result as `std::int64_t`. |

#### Unsigned Integer Parsers

| Syntax       | Meaning                                                                 |
| ------------ | ----------------------------------------------------------------------- |
| `ushort_`    | Parse an unsigned integer and expose the result as `unsigned short`.     |
| `uint_`      | Parse an unsigned integer and expose the result as `unsigned int`.       |
| `ulong_`     | Parse an unsigned integer and expose the result as `unsigned long`.      |
| `ulong_long` | Parse an unsigned integer and expose the result as `unsigned long long`. |
| `uint8`      | Parse an unsigned integer and expose the result as `std::uint8_t`.       |
| `uint16`     | Parse an unsigned integer and expose the result as `std::uint16_t`.      |
| `uint32`     | Parse an unsigned integer and expose the result as `std::uint32_t`.      |
| `uint64`     | Parse an unsigned integer and expose the result as `std::uint64_t`.      |
| `bin`        | Parse a base-2 unsigned integer and expose the result as `unsigned int`.        |
| `oct`        | Parse a base-8 unsigned integer and expose the result as `unsigned int`.        |
| `hex`        | Parse a base-16 unsigned integer and expose the result as `unsigned int`.       |

#### Real Number Parsers

| Syntax        | Meaning                                                    |
| ------------- | ---------------------------------------------------------- |
| `float_`      | Parse a signed real number and expose the result as `float`.       |
| `double_`     | Parse a signed real number and expose the result as `double`.      |
| `long_double` | Parse a signed real number and expose the result as `long double`. |

### Context Fetchers

The following function objects retrieve commonly used values from the parsing context (usually via a semantic action). Each fetcher returns the context entry itself, normally by reference, and is available only when the corresponding entry exists in the current context.

#### Example

```cpp
constexpr auto p = x4::int_.on_match([](auto&& ctx) {
    x4::_attr(ctx) *= 5;
});
```

| Syntax            | Meaning                                                                                                                                                                                                                                                                                                              |
| ----------------- | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `_attr(ctx)`      | Return the reference to the attribute associated with the current semantic action. This is the attribute instance produced by the parser `p` on the expression `p.on_match(f)`. It is available only when that parser exposes a non-`unused_type` attribute. Equivalent to `x4::get<x4::contexts::attr>(ctx)`.                                          |
| `_rule_var(ctx)`  | Return the reference to the attribute variable of the innermost active `x4::rule` invocation. In a recursive rule, this always refers to the current recursive invocation rather than an outer invocation. Equivalent to `x4::get<x4::contexts::rule_var>(ctx)`. |
| `_local_var(ctx)` | Return the reference to the innermost local variable created by `with_local<T>[p]` using the default `x4::contexts::local_var` context id. Equivalent to `x4::get<x4::contexts::local_var>(ctx)`.<br>*Note:* when `with_local<T, ID>[p]` uses a custom id, retrieve the value with `x4::get<ID>(ctx)` instead. |
| `_as_var(ctx)`    | Return the reference to the attribute variable managed by the innermost active `as<T>(p)` facility. Semantic actions inside `p` can use this fetcher to access the value being constructed for the enclosing `as<T>` parser. Equivalent to `x4::get<x4::contexts::as_var>(ctx)`.                                                      |
