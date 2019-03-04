/*
    Corrade::TestSuite
        — like Google Test, but without the crap

    https://doc.magnum.graphics/corrade/classCorrade_1_1TestSuite_1_1Tester.html

    This is a single-header library generated from the Corrade project. With
    the goal being easy integration, it's deliberately free of all comments
    to keep the file size small. More info, detailed changelogs and docs here:

    -   Project homepage — https://magnum.graphics/corrade/
    -   Documentation — https://doc.magnum.graphics/corrade/
    -   GitHub project page — https://github.com/mosra/corrade
    -   GitHub Singles repository — https://github.com/mosra/magnum-singles

    v2019.01-61-ge20d61a2 (2019-03-01)
    -   Hastily put together while I'm running off to a pub. NO WARRANTY.

    Generated from Corrade v2019.01-61-ge20d61a2 (2019-03-01), 5283 / 42921 LoC
*/

/*
    This file is part of Corrade.

    Copyright © 2007, 2008, 2009, 2010, 2011, 2012, 2013, 2014, 2015, 2016,
                2017, 2018, 2019 Vladimír Vondruš <mosra@centrum.cz>

    Permission is hereby granted, free of charge, to any person obtaining a
    copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included
    in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
    DEALINGS IN THE SOFTWARE.
*/

#include <algorithm>
#include <cassert>
#include <cctype>
#include <chrono>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <dirent.h>
#include <fcntl.h>
#include <initializer_list>
#include <iomanip>
#include <iosfwd>
#include <iostream>
#include <limits>
#include <new>
#include <random>
#include <sstream>
#include <string>
#include <sys/mman.h>
#include <sys/stat.h>
#include <tuple>
#include <type_traits>
#include <unistd.h>
#include <utility>
#include <vector>

#ifndef Corrade_configure_h
#define Corrade_configure_h

#define CORRADE_TARGET_UNIX

#if defined(__i386) || defined(__x86_64) || defined(_M_IX86) || defined(_M_X64)

#elif defined(__arm__) || defined(__aarch64__) || defined(_M_ARM) || defined(_M_ARM64)

#elif defined(__powerpc64__) || defined(_M_PPC)
#define CORRADE_TARGET_POWERPC

#endif

#ifdef _MSC_VER
#ifdef _MSVC_LANG
#define CORRADE_CXX_STANDARD _MSVC_LANG
#else
#define CORRADE_CXX_STANDARD 201103L
#endif
#else
#define CORRADE_CXX_STANDARD __cplusplus
#endif

#endif
#ifndef CORRADE_ASSERT
#ifdef NDEBUG
#define CORRADE_ASSERT(condition, message, returnValue) do {} while(0)
#else
#define CORRADE_ASSERT(condition, message, returnValue) assert(condition)
#endif
#endif

#ifndef CORRADE_CONSTEXPR_ASSERT
#ifdef NDEBUG
#define CORRADE_CONSTEXPR_ASSERT(condition, message) static_cast<void>(0)
#else
#define CORRADE_CONSTEXPR_ASSERT(condition, message)                        \
    static_cast<void>((condition) ? 0 : ([&]() {                            \
        assert(!#condition);                                                \
    }(), 0))
#endif
#endif

#ifndef CORRADE_ASSERT_OUTPUT
#ifdef NDEBUG
#define CORRADE_ASSERT_OUTPUT(call, message, returnValue)                   \
    static_cast<void>(call)
#else
#define CORRADE_ASSERT_OUTPUT(call, message, returnValue) assert(call)
#endif
#endif

#ifndef CORRADE_INTERNAL_ASSERT
#ifdef NDEBUG
#define CORRADE_INTERNAL_ASSERT(condition) do {} while(0)
#else
#define CORRADE_INTERNAL_ASSERT(condition) assert(condition)
#endif
#endif

#ifndef CORRADE_INTERNAL_CONSTEXPR_ASSERT
#ifdef NDEBUG
#define CORRADE_INTERNAL_CONSTEXPR_ASSERT(condition) static_cast<void>(0)
#else
#define CORRADE_INTERNAL_CONSTEXPR_ASSERT(condition)                        \
    static_cast<void>((condition) ? 0 : ([&]() {                            \
        assert(!#condition);                                                \
    }(), 0))
#endif
#endif

#ifndef CORRADE_INTERNAL_ASSERT_OUTPUT
#ifdef NDEBUG
#define CORRADE_INTERNAL_ASSERT_OUTPUT(call)                                \
    static_cast<void>(call)
#else
#define CORRADE_INTERNAL_ASSERT_OUTPUT(call) assert(call)
#endif
#endif

#ifndef CORRADE_ASSERT_UNREACHABLE
#ifdef NDEBUG
#ifdef __GNUC__
#define CORRADE_ASSERT_UNREACHABLE() __builtin_unreachable()
#else
#define CORRADE_ASSERT_UNREACHABLE() std::abort()
#endif
#else
#define CORRADE_ASSERT_UNREACHABLE() assert(false)
#endif
#endif
#ifndef Corrade_Utility_Macros_h
#define Corrade_Utility_Macros_h

#define _CORRADE_HELPER_PASTE2(a, b) a ## b
#define _CORRADE_HELPER_PASTE(a, b) _CORRADE_HELPER_PASTE2(a, b)
#define _CORRADE_HELPER_STR(X) #X
#define _CORRADE_HELPER_DEFER(M, ...) M(__VA_ARGS__)

#if defined(__GNUC__) || defined(__clang__)
#define CORRADE_DEPRECATED(message) __attribute((deprecated(message)))
#elif defined(_MSC_VER)
#define CORRADE_DEPRECATED(message) __declspec(deprecated(message))
#else
#define CORRADE_DEPRECATED(message)
#endif

#if defined(__GNUC__) || defined(__clang__)
#define CORRADE_DEPRECATED_ALIAS(message) __attribute((deprecated(message)))
#elif defined(_MSC_VER) && _MSC_VER >= 1910
#define CORRADE_DEPRECATED_ALIAS(message) [[deprecated(message)]]
#else
#define CORRADE_DEPRECATED_ALIAS(message)
#endif

#ifdef __clang__
#if (__clang_major__ < 6) || (0 && __clang_major__ < 10)
#define CORRADE_DEPRECATED_NAMESPACE(message) _Pragma("GCC diagnostic push") _Pragma("GCC diagnostic ignored \"-Wc++14-extensions\"") [[deprecated(message)]] _Pragma("GCC diagnostic pop")
#else
#define CORRADE_DEPRECATED_NAMESPACE(message) _Pragma("GCC diagnostic push") _Pragma("GCC diagnostic ignored \"-Wc++14-extensions\"") _Pragma("GCC diagnostic ignored \"-Wc++17-extensions\"") [[deprecated(message)]] _Pragma("GCC diagnostic pop")
#endif
#elif defined(_MSC_VER)
#define CORRADE_DEPRECATED_NAMESPACE(message) [[deprecated(message)]]
#else
#define CORRADE_DEPRECATED_NAMESPACE(message)
#endif

#if defined(__clang__) || (defined(__GNUC__) && __GNUC__ >= 6)
#define CORRADE_DEPRECATED_ENUM(message) __attribute((deprecated(message)))
#elif defined(_MSC_VER)
#define CORRADE_DEPRECATED_ENUM(message) [[deprecated(message)]]
#else
#define CORRADE_DEPRECATED_ENUM(message)
#endif

#ifdef __clang__
#define CORRADE_DEPRECATED_FILE(message) _Pragma(_CORRADE_HELPER_STR(GCC warning ("this file is deprecated: " message)))
#elif defined(__GNUC__) && __GNUC__*100 + __GNUC_MINOR__ >= 408
#define CORRADE_DEPRECATED_FILE(message) _Pragma(_CORRADE_HELPER_STR(GCC warning message))
#elif defined(_MSC_VER)
#define CORRADE_DEPRECATED_FILE(_message) __pragma(message ("warning: " __FILE__ " is deprecated: " _message))
#else
#define CORRADE_DEPRECATED_FILE(message)
#endif

#ifdef __clang__
#define CORRADE_DEPRECATED_MACRO(macro,message) _Pragma(_CORRADE_HELPER_STR(GCC warning ("this macro is deprecated: " message)))
#elif defined(__GNUC__) && __GNUC__*100 + __GNUC_MINOR__ >= 408
#define CORRADE_DEPRECATED_MACRO(macro,message) _Pragma(_CORRADE_HELPER_STR(GCC warning message))
#elif defined(_MSC_VER)
#define CORRADE_DEPRECATED_MACRO(macro,_message) __pragma(message (__FILE__ ": warning: " _CORRADE_HELPER_STR(macro) " is deprecated: " _message))
#else
#define CORRADE_DEPRECATED_MACRO(macro,message)
#endif

#ifdef __clang__
#define CORRADE_IGNORE_DEPRECATED_PUSH _Pragma("GCC diagnostic push") _Pragma("GCC diagnostic ignored \"-Wdeprecated-declarations\"") _Pragma("GCC diagnostic ignored \"-W#pragma-messages\"")
#elif defined(__GNUC__)
#define CORRADE_IGNORE_DEPRECATED_PUSH _Pragma("GCC diagnostic push") _Pragma("GCC diagnostic ignored \"-Wdeprecated-declarations\"")
#elif defined(_MSC_VER)
#define CORRADE_IGNORE_DEPRECATED_PUSH __pragma(warning(push)) __pragma(warning(disable: 4996))
#else
#define CORRADE_IGNORE_DEPRECATED_PUSH
#endif

#ifdef __GNUC__
#define CORRADE_IGNORE_DEPRECATED_POP _Pragma("GCC diagnostic pop")
#elif defined(_MSC_VER)
#define CORRADE_IGNORE_DEPRECATED_POP __pragma(warning(pop))
#else
#define CORRADE_IGNORE_DEPRECATED_POP
#endif

#ifdef __GNUC__
#define CORRADE_UNUSED __attribute__((__unused__))
#elif defined(_MSC_VER)
#define CORRADE_UNUSED __pragma(warning(suppress:4100))
#else
#define CORRADE_UNUSED
#endif

#if defined(__GNUC__) && __GNUC__*100 + __GNUC_MINOR__ < 408
#define CORRADE_ALIGNAS(alignment) __attribute__((aligned(alignment)))
#else
#define CORRADE_ALIGNAS(alignment) alignas(alignment)
#endif

#if defined(__GNUC__) && __GNUC__*100 + __GNUC_MINOR__ < 408
#define CORRADE_NORETURN __attribute__((noreturn))
#else
#define CORRADE_NORETURN [[noreturn]]
#endif

#define CORRADE_AUTOMATIC_INITIALIZER(function)                             \
    namespace {                                                             \
        struct Initializer_##function { static const int i; };              \
        const int Initializer_##function::i = function();                   \
    }

#define CORRADE_AUTOMATIC_FINALIZER(function)                               \
    class Finalizer_##function {                                            \
        public:                                                             \
            Finalizer_##function() {}                                       \
            ~Finalizer_##function() { function(); }                         \
    } Finalizer_##function;

#endif
#ifndef Corrade_Containers_Containers_h
#define Corrade_Containers_Containers_h

namespace Corrade { namespace Containers {

template<class T, class = void(*)(T*, std::size_t)> class Array;
template<class> class ArrayView;
template<std::size_t, class> class StaticArrayView;
template<std::size_t, class> class StaticArray;

template<class> class StridedArrayView;
template<class> class StridedIterator;

template<class T, typename std::underlying_type<T>::type fullValue = typename std::underlying_type<T>::type(~0)> class EnumSet;
template<class> class LinkedList;
template<class Derived, class List = LinkedList<Derived>> class LinkedListItem;

template<class T> class Optional;
template<class T> class Pointer;
template<class T> class Reference;

}}

#endif
#ifndef Corrade_Containers_Tags_h
#define Corrade_Containers_Tags_h

namespace Corrade { namespace Containers {

struct DefaultInitT {
    struct Init{};
    constexpr explicit DefaultInitT(Init) {}
};

struct ValueInitT {
    struct Init{};
    constexpr explicit ValueInitT(Init) {}
};

struct NoInitT {
    struct Init{};
    constexpr explicit NoInitT(Init) {}
};

struct NoCreateT {
    struct Init{};
    constexpr explicit NoCreateT(Init) {}
};

struct DirectInitT {
    struct Init{};
    constexpr explicit DirectInitT(Init) {}
};

struct InPlaceInitT {
    struct Init{};
    constexpr explicit InPlaceInitT(Init) {}
};

constexpr DefaultInitT DefaultInit{DefaultInitT::Init{}};

constexpr ValueInitT ValueInit{ValueInitT::Init{}};

constexpr NoInitT NoInit{NoInitT::Init{}};

constexpr NoCreateT NoCreate{NoCreateT::Init{}};

constexpr DirectInitT DirectInit{DirectInitT::Init{}};

constexpr InPlaceInitT InPlaceInit{InPlaceInitT::Init{}};

}}

#endif
#ifndef Corrade_Containers_EnumSet_h
#define Corrade_Containers_EnumSet_h

namespace Corrade { namespace Containers {

template<class T, typename std::underlying_type<T>::type fullValue>
class EnumSet {
    static_assert(std::is_enum<T>::value, "EnumSet type must be strongly typed enum");

    public:
        typedef T Type;

        typedef typename std::underlying_type<T>::type UnderlyingType;

        enum: UnderlyingType {
            FullValue = fullValue
        };

        constexpr /*implicit*/ EnumSet(): value() {}

        constexpr /*implicit*/ EnumSet(T value): value(static_cast<UnderlyingType>(value)) {}

        explicit EnumSet(NoInitT) {}

        constexpr bool operator==(EnumSet<T, fullValue> other) const {
            return value == other.value;
        }

        constexpr bool operator!=(EnumSet<T, fullValue> other) const {
            return !operator==(other);
        }

        constexpr bool operator>=(EnumSet<T, fullValue> other) const {
            return (*this & other) == other;
        }

        constexpr bool operator<=(EnumSet<T, fullValue> other) const {
            return (*this & other) == *this;
        }

        constexpr EnumSet<T, fullValue> operator|(EnumSet<T, fullValue> other) const {
            return EnumSet<T, fullValue>(value | other.value);
        }

        EnumSet<T, fullValue>& operator|=(EnumSet<T, fullValue> other) {
            value |= other.value;
            return *this;
        }

        constexpr EnumSet<T, fullValue> operator&(EnumSet<T, fullValue> other) const {
            return EnumSet<T, fullValue>(value & other.value);
        }

        EnumSet<T, fullValue>& operator&=(EnumSet<T, fullValue> other) {
            value &= other.value;
            return *this;
        }

        constexpr EnumSet<T, fullValue> operator^(EnumSet<T, fullValue> other) const {
            return EnumSet<T, fullValue>(value ^ other.value);
        }

        EnumSet<T, fullValue>& operator^=(EnumSet<T, fullValue> other) {
            value ^= other.value;
            return *this;
        }

        constexpr EnumSet<T, fullValue> operator~() const {
            return EnumSet<T, fullValue>(fullValue & ~value);
        }

        constexpr explicit operator bool() const {
            return value != 0;
        }

        constexpr explicit operator UnderlyingType() const {
            return value;
        }

    private:
        constexpr explicit EnumSet(UnderlyingType type): value(type) {}

        UnderlyingType value;
};

#define CORRADE_ENUMSET_OPERATORS(class)                                    \
    constexpr bool operator==(class::Type a, class b) {                     \
        return class(a) == b;                                               \
    }                                                                       \
    constexpr bool operator!=(class::Type a, class b) {                     \
        return class(a) != b;                                               \
    }                                                                       \
    constexpr bool operator>=(class::Type a, class b) {                     \
        return class(a) >= b;                                               \
    }                                                                       \
    constexpr bool operator<=(class::Type a, class b) {                     \
        return class(a) <= b;                                               \
    }                                                                       \
    constexpr class operator|(class::Type a, class b) {                     \
        return b | a;                                                       \
    }                                                                       \
    constexpr class operator&(class::Type a, class b) {                     \
        return b & a;                                                       \
    }                                                                       \
    constexpr class operator^(class::Type a, class b) {                     \
        return b ^ a;                                                       \
    }                                                                       \
    constexpr class operator~(class::Type a) {                              \
        return ~class(a);                                                   \
    }

#define CORRADE_ENUMSET_FRIEND_OPERATORS(class)                             \
    friend constexpr bool operator==(class::Type, class);                   \
    friend constexpr bool operator!=(class::Type, class);                   \
    friend constexpr bool operator>=(class::Type, class);                   \
    friend constexpr bool operator<=(class::Type, class);                   \
    friend constexpr class operator&(class::Type, class);                   \
    friend constexpr class operator|(class::Type, class);                   \
    friend constexpr class operator^(class::Type, class);                   \
    friend constexpr class operator~(class::Type);

}}

#endif
#ifndef Corrade_Utility_TypeTraits_h
#define Corrade_Utility_TypeTraits_h

namespace Corrade { namespace Utility {

#define CORRADE_HAS_TYPE(className, typeExpression)                         \
template<class U> class className {                                         \
    template<class T> static char get(T&&, typeExpression* = nullptr);      \
    static short get(...);                                                  \
    public:                                                                 \
        enum: bool { value = sizeof(get(std::declval<U>())) == sizeof(char) }; \
}

namespace Implementation {
    CORRADE_HAS_TYPE(HasMemberBegin, decltype(std::declval<T>().begin()));
    CORRADE_HAS_TYPE(HasMemberEnd, decltype(std::declval<T>().end()));
    CORRADE_HAS_TYPE(HasBegin, decltype(begin(std::declval<T>())));
    CORRADE_HAS_TYPE(HasEnd, decltype(end(std::declval<T>())));
}

template<class T> using IsIterable = std::integral_constant<bool,
    (Implementation::HasMemberBegin<T>::value || Implementation::HasBegin<T>::value) &&
    (Implementation::HasMemberEnd<T>::value || Implementation::HasEnd<T>::value)
    >;

}}

#endif
#ifndef Corrade_Utility_Utility_h
#define Corrade_Utility_Utility_h

namespace Corrade { namespace Utility {

class Arguments;

template<std::size_t> class HashDigest;

class Configuration;
class ConfigurationGroup;
enum class ConfigurationValueFlag: std::uint8_t;
typedef Containers::EnumSet<ConfigurationValueFlag> ConfigurationValueFlags;
template<class> struct ConfigurationValue;
#ifdef CORRADE_TARGET_UNIX
class FileWatcher;
#endif

class Debug;
class Warning;
class Error;
class Fatal;

class MurmurHash2;

class Sha1;
class Translator;

#ifdef CORRADE_TARGET_UNIX
template<class> struct TweakableParser;
enum class TweakableState: std::uint8_t;
#endif

}}

#endif
#ifndef Corrade_Utility_VisibilityMacros_h
#define Corrade_Utility_VisibilityMacros_h

#define CORRADE_VISIBILITY_EXPORT __attribute__ ((visibility ("default")))

#define CORRADE_VISIBILITY_IMPORT __attribute__ ((visibility ("default")))

#define CORRADE_VISIBILITY_STATIC __attribute__ ((visibility ("default")))

#define CORRADE_VISIBILITY_LOCAL __attribute__ ((visibility ("hidden")))

#endif
#ifndef Corrade_Utility_visibility_h
#define Corrade_Utility_visibility_h

#ifndef CORRADE_BUILD_STATIC
    #if defined(CorradeUtilityObjects_EXPORTS) || defined(CorradeUtility_EXPORTS) || defined(CorradeUtilityTestLib_EXPORTS)
        #define CORRADE_UTILITY_EXPORT CORRADE_VISIBILITY_EXPORT
    #else
        #define CORRADE_UTILITY_EXPORT CORRADE_VISIBILITY_IMPORT
    #endif
#else
    #define CORRADE_UTILITY_EXPORT CORRADE_VISIBILITY_STATIC
#endif
#define CORRADE_UTILITY_LOCAL CORRADE_VISIBILITY_LOCAL

#endif
#ifndef Corrade_Utility_Debug_h
#define Corrade_Utility_Debug_h

namespace Corrade { namespace Utility {

namespace Implementation { struct DebugOstreamFallback; }

class CORRADE_UTILITY_EXPORT Debug {
    public:
        enum class Flag: unsigned char {
            NoNewlineAtTheEnd = 1 << 0,

            DisableColors = 1 << 1
        };

        typedef Containers::EnumSet<Flag> Flags;

        typedef void(*Modifier)(Debug&);

        enum class Color: char {
            Black = 0,

            #ifndef CORRADE_UTILITY_USE_ANSI_COLORS
            Red = 1,
            #else
            Red = 4,
            #endif

            Green = 2,

            #ifndef CORRADE_UTILITY_USE_ANSI_COLORS
            Yellow = 3,
            #else
            Yellow = 6,
            #endif

            #ifndef CORRADE_UTILITY_USE_ANSI_COLORS
            Blue = 4,
            #else
            Blue = 1,
            #endif

            Magenta = 5,

            #ifndef CORRADE_UTILITY_USE_ANSI_COLORS
            Cyan = 6,
            #else
            Cyan = 3,
            #endif

            White = 7,

            #ifndef CORRADE_UTILITY_USE_ANSI_COLORS
            Default = 9
            #else
            Default = 7
            #endif
        };

        inline static void nospace(Debug& debug);

        static void newline(Debug& debug) {
            debug << nospace << "\n" << nospace;
        }

        static Modifier color(Color color);

        static Modifier boldColor(Color color);

        static void resetColor(Debug& debug);

        Debug& operator<<(Modifier f) {
            f(*this);
            return *this;
        }

        static std::ostream* output();

        static bool isTty(std::ostream* output);

        static bool isTty();

        explicit Debug(Flags flags = {});

        explicit Debug(std::ostream* output, Flags flags = {});

        Debug(const Debug&) = delete;

        Debug(Debug&&) = default;

        ~Debug();

        Debug& operator=(const Debug&) = delete;

        Debug& operator=(Debug&&) = delete;

        Debug& operator<<(const std::string& value);
        Debug& operator<<(const char* value);
        Debug& operator<<(const void* value);
        Debug& operator<<(bool value);
        Debug& operator<<(int value);
        Debug& operator<<(long value);
        Debug& operator<<(long long value);
        Debug& operator<<(unsigned value);
        Debug& operator<<(unsigned long value);
        Debug& operator<<(unsigned long long value);

        Debug& operator<<(float value);

        Debug& operator<<(double value);

        Debug& operator<<(long double value);

        Debug& operator<<(char32_t value);

        Debug& operator<<(const char32_t* value);

        Debug& operator<<(std::nullptr_t);

        Debug& operator<<(Implementation::DebugOstreamFallback&& value);

    protected:
        std::ostream* _output;

        enum class InternalFlag: unsigned char {
            NoNewlineAtTheEnd = 1 << 0,
            DisableColors = 1 << 1,
            NoSpaceBeforeNextValue = 1 << 2,
            ValueWritten = 1 << 3,
            ColorWritten = 1 << 4
        };
        typedef Containers::EnumSet<InternalFlag> InternalFlags;

        CORRADE_ENUMSET_FRIEND_OPERATORS(InternalFlags)

        CORRADE_UTILITY_LOCAL void cleanupOnDestruction();

        InternalFlags _flags;

    private:
        template<Color c, bool bold> CORRADE_UTILITY_LOCAL static Modifier colorInternal();

        static CORRADE_UTILITY_LOCAL std::ostream* _globalOutput;
        #ifndef CORRADE_UTILITY_USE_ANSI_COLORS
        static CORRADE_UTILITY_LOCAL Color _globalColor;
        static CORRADE_UTILITY_LOCAL bool _globalColorBold;
        #endif

        template<class T> CORRADE_UTILITY_LOCAL Debug& print(const T& value);
        CORRADE_UTILITY_LOCAL void resetColorInternal();

        std::ostream* _previousGlobalOutput;
        Color _previousColor;
        bool _previousColorBold;
};

CORRADE_ENUMSET_OPERATORS(Debug::Flags)
CORRADE_ENUMSET_OPERATORS(Debug::InternalFlags)

inline void Debug::nospace(Debug& debug) {
    debug._flags |= InternalFlag::NoSpaceBeforeNextValue;
}

template<class T> inline Debug& operator<<(Debug&& debug, const T& value) {
    return debug << value;
}

template<class Iterable> Debug& operator<<(typename std::enable_if<IsIterable<Iterable>::value && !std::is_same<Iterable, std::string>::value, Debug&>::type debug, const Iterable& value)
{
    debug << "{" << Debug::nospace;
    for(auto it = value.begin(); it != value.end(); ++it) {
        if(it != value.begin())
            debug << Debug::nospace << ",";
        debug << *it;
    }
    debug << Debug::nospace << "}";
    return debug;
}

namespace Implementation {
    template<std::size_t ...> struct Sequence {};

    template<std::size_t N, std::size_t ...sequence> struct GenerateSequence:
        GenerateSequence<N-1, N-1, sequence...> {};

    template<std::size_t ...sequence> struct GenerateSequence<0, sequence...> {
        typedef Sequence<sequence...> Type;
    };

    template<class T> inline void tupleDebugOutput(Debug&, const T&, Sequence<>) {}
    template<class T, std::size_t i, std::size_t ...sequence> void tupleDebugOutput(Debug& debug, const T& tuple, Sequence<i, sequence...>) {
        debug << std::get<i>(tuple);
        #ifdef _MSC_VER
        #pragma warning(push)
        #pragma warning(disable:4127)
        #endif
        if(i + 1 != std::tuple_size<T>::value)
            debug << Debug::nospace << ",";
        #ifdef _MSC_VER
        #pragma warning(pop)
        #endif
        tupleDebugOutput(debug, tuple, Sequence<sequence...>{});
    }
}

template<class ...Args> Debug& operator<<(Debug& debug, const std::tuple<Args...>& value) {
    debug << "(" << Debug::nospace;
    Implementation::tupleDebugOutput(debug, value, typename Implementation::GenerateSequence<sizeof...(Args)>::Type{});
    debug << Debug::nospace << ")";
    return debug;
}

template<class T, class U> Debug& operator<<(Debug& debug, const std::pair<T, U>& value) {
    return debug << "(" << Debug::nospace << value.first << Debug::nospace << "," << value.second << Debug::nospace << ")";
}

class CORRADE_UTILITY_EXPORT Warning: public Debug {
    public:
        static std::ostream* output();

        static bool isTty();

        explicit Warning(Flags flags = {});

        explicit Warning(std::ostream* output, Flags flags = {});

        Warning(const Warning&) = delete;

        Warning(Warning&&) = default;

        ~Warning();

        Warning& operator=(const Warning&) = delete;

        Warning& operator=(Warning&&) = delete;

    private:
        static CORRADE_UTILITY_LOCAL std::ostream* _globalWarningOutput;
        std::ostream* _previousGlobalWarningOutput;
};

class CORRADE_UTILITY_EXPORT Error: public Debug {
    friend Fatal;

    public:
        static std::ostream* output();

        static bool isTty();

        explicit Error(Flags flags = {});

        explicit Error(std::ostream* output, Flags flags = {});

        Error(const Error&) = delete;

        Error(Error&&) = default;

        ~Error();

        Error& operator=(const Error&) = delete;

        Error& operator=(Error&&) = delete;

    protected:
        CORRADE_UTILITY_LOCAL void cleanupOnDestruction();

    private:
        static CORRADE_UTILITY_LOCAL std::ostream* _globalErrorOutput;
        std::ostream* _previousGlobalErrorOutput;
};

class CORRADE_UTILITY_EXPORT Fatal: public Error {
    public:
        Fatal(int exitCode = 1, Flags flags = {}): Error{flags}, _exitCode{exitCode} {}

        Fatal(Flags flags): Fatal{1, flags} {}

        Fatal(std::ostream* output, int exitCode = 1, Flags flags = {}): Error{output, flags}, _exitCode{exitCode} {}

        Fatal(std::ostream* output, Flags flags = {}): Fatal{output, 1, flags} {}

        #ifndef CORRADE_MSVC2015_COMPATIBILITY
        CORRADE_NORETURN
        #endif
        ~Fatal();

    private:
        int _exitCode;
};

CORRADE_UTILITY_EXPORT Debug& operator<<(Debug& debug, Debug::Color value);

namespace Implementation {

struct DebugOstreamFallback {
    template<class T> /*implicit*/ DebugOstreamFallback(const T& t): applier(&DebugOstreamFallback::applyImpl<T>), value(&t) {}

    void apply(std::ostream& s) const {
        (this->*applier)(s);
    }

    template<class T> void applyImpl(std::ostream& s) const {
        s << *static_cast<const T*>(value);
    }

    using ApplierFunc = void(DebugOstreamFallback::*)(std::ostream&) const;
    const ApplierFunc applier;
    const void* value;
};

}

}}

#endif
#ifndef Corrade_TestSuite_Comparator_h
#define Corrade_TestSuite_Comparator_h

namespace Corrade { namespace TestSuite {

template<class T> class Comparator {
    public:
        explicit Comparator();

        bool operator()(const T& actual, const T& expected);

        void printErrorMessage(Utility::Error& e, const char* actual, const char* expected) const;

    private:
        const T* actualValue;
        const T* expectedValue;
};

template<class T> Comparator<T>::Comparator(): actualValue(), expectedValue() {}

template<class T> bool Comparator<T>::operator()(const T& actual, const T& expected) {
    if(actual == expected) return true;

    actualValue = &actual;
    expectedValue = &expected;
    return false;
}

template<class T> void Comparator<T>::printErrorMessage(Utility::Error& e, const char* actual, const char* expected) const {
    CORRADE_INTERNAL_ASSERT(actualValue && expectedValue);
    e << "Values" << actual << "and" << expected << "are not the same, actual is\n       "
      << *actualValue << Utility::Debug::newline << "        but expected\n       " << *expectedValue;
}

namespace Implementation {

template<class T> struct ComparatorOperatorTraits;

template<class T, class U, class V> struct ComparatorOperatorTraits<bool(T::*)(U, V)> {
    typedef typename std::decay<U>::type ActualType;
    typedef typename std::decay<V>::type ExpectedType;
};

template<class T> struct ComparatorTraits: ComparatorOperatorTraits<decltype(&Comparator<T>::operator())> {};

}

}}

#endif
#ifndef Corrade_TestSuite_TestSuite_h
#define Corrade_TestSuite_TestSuite_h

namespace Corrade { namespace TestSuite {

template<class> class Comparator;

}}

#endif
#ifndef Corrade_TestSuite_visibility_h
#define Corrade_TestSuite_visibility_h

#ifndef CORRADE_BUILD_STATIC
    #ifdef CorradeTestSuite_EXPORTS
        #define CORRADE_TESTSUITE_EXPORT CORRADE_VISIBILITY_EXPORT
    #else
        #define CORRADE_TESTSUITE_EXPORT CORRADE_VISIBILITY_IMPORT
    #endif
#else
    #define CORRADE_TESTSUITE_EXPORT CORRADE_VISIBILITY_STATIC
#endif
#define CORRADE_TESTSUITE_LOCAL CORRADE_VISIBILITY_LOCAL

#endif
#ifndef Corrade_TestSuite_Compare_FloatingPoint_h
#define Corrade_TestSuite_Compare_FloatingPoint_h

namespace Corrade { namespace TestSuite {

namespace Implementation {
    template<class T> class FloatComparatorEpsilon {};

    template<> class FloatComparatorEpsilon<float> {
        public:
            constexpr static float epsilon() { return 1.0e-6f; }
    };

    template<> class FloatComparatorEpsilon<double> {
        public:
            constexpr static double epsilon() { return 1.0e-12; }
    };

    template<> class FloatComparatorEpsilon<long double> {
        public:
            constexpr static long double epsilon() { return 1.0e-15l; }
    };

    template<class T> class CORRADE_TESTSUITE_EXPORT FloatComparator {
        public:
            bool operator()(T actual, T expected);
            void printErrorMessage(Utility::Error& e, const char* actual, const char* expected) const;

        private:
            T _actualValue, _expectedValue;
    };
}

template<> class Comparator<float>: public Implementation::FloatComparator<float> {};

template<> class Comparator<double>: public Implementation::FloatComparator<double> {};

template<> class Comparator<long double>: public Implementation::FloatComparator<long double> {};

}}

#endif
#ifndef Corrade_TestSuite_Tester_h
#define Corrade_TestSuite_Tester_h

namespace Corrade { namespace TestSuite {

namespace Implementation {
    template<class Actual, class Expected, bool = std::is_convertible<Actual, Expected>::value> struct CommonType {
        typedef Expected Type;
    };
    template<class Actual, class Expected> struct CommonType<Actual, Expected, false> {
        typedef typename std::common_type<Actual, Expected>::type Type;
    };
}

class CORRADE_TESTSUITE_EXPORT Tester {
    public:
        class CORRADE_TESTSUITE_EXPORT TesterConfiguration {
            public:
                explicit TesterConfiguration();

                const std::vector<std::string>& skippedArgumentPrefixes() const {
                    return _skippedArgumentPrefixes;
                }

                TesterConfiguration& setSkippedArgumentPrefixes(std::initializer_list<std::string> prefixes) {
                    _skippedArgumentPrefixes.insert(_skippedArgumentPrefixes.end(), prefixes);
                    return *this;
                }

            private:
                std::vector<std::string> _skippedArgumentPrefixes;
        };

        typedef Corrade::Utility::Debug Debug;

        typedef Corrade::Utility::Warning Warning;

        typedef Corrade::Utility::Error Error;

        enum class BenchmarkType {

            Default = 1,

            WallTime = 2,

            CpuTime = 3,

            CpuCycles = 4
        };

        enum class BenchmarkUnits {

            Nanoseconds = 100,
            Cycles = 101,
            Instructions = 102,
            Bytes = 103,
            Count = 104
        };

        explicit Tester(TesterConfiguration configuration = TesterConfiguration{});

        std::pair<int&, char**> arguments() { return {*_argc, _argv}; }

        template<class Derived> void addTests(std::initializer_list<void(Derived::*)()> tests) {
            addRepeatedTests<Derived>(tests, 1);
        }

        template<class Derived> void addRepeatedTests(std::initializer_list<void(Derived::*)()> tests, std::size_t repeatCount) {
            addRepeatedTests<Derived>(tests, repeatCount, nullptr, nullptr);
        }

        template<class Derived> void addTests(std::initializer_list<void(Derived::*)()> tests, void(Derived::*setup)(), void(Derived::*teardown)()) {
            addRepeatedTests<Derived>(tests, 1, setup, teardown);
        }

        template<class Derived> void addRepeatedTests(std::initializer_list<void(Derived::*)()> tests, std::size_t repeatCount, void(Derived::*setup)(), void(Derived::*teardown)()) {
            _testCases.reserve(_testCases.size() + tests.size());
            for(auto test: tests)
                _testCases.emplace_back(~std::size_t{}, repeatCount, static_cast<TestCase::Function>(test), static_cast<TestCase::Function>(setup), static_cast<TestCase::Function>(teardown));
        }

        template<class Derived> void addInstancedTests(std::initializer_list<void(Derived::*)()> tests, std::size_t instanceCount) {
            addRepeatedInstancedTests<Derived>(tests, 1, instanceCount);
        }

        template<class Derived> void addRepeatedInstancedTests(std::initializer_list<void(Derived::*)()> tests, std::size_t repeatCount, std::size_t instanceCount) {
            addRepeatedInstancedTests<Derived>(tests, repeatCount, instanceCount, nullptr, nullptr);
        }

        template<class Derived> void addInstancedTests(std::initializer_list<void(Derived::*)()> tests, std::size_t instanceCount, void(Derived::*setup)(), void(Derived::*teardown)()) {
            addRepeatedInstancedTests<Derived>(tests, 1, instanceCount, setup, teardown);
        }

        template<class Derived> void addRepeatedInstancedTests(std::initializer_list<void(Derived::*)()> tests, std::size_t repeatCount, std::size_t instanceCount, void(Derived::*setup)(), void(Derived::*teardown)()) {
            _testCases.reserve(_testCases.size() + tests.size());
            for(auto test: tests) for(std::size_t i = 0; i != instanceCount; ++i)
                _testCases.emplace_back(i, repeatCount, static_cast<TestCase::Function>(test), static_cast<TestCase::Function>(setup), static_cast<TestCase::Function>(teardown));
        }

        template<class Derived> void addBenchmarks(std::initializer_list<void(Derived::*)()> benchmarks, std::size_t batchCount, BenchmarkType benchmarkType = BenchmarkType::Default) {
            addBenchmarks<Derived>(benchmarks, batchCount, nullptr, nullptr, benchmarkType);
        }

        template<class Derived> void addBenchmarks(std::initializer_list<void(Derived::*)()> benchmarks, std::size_t batchCount, void(Derived::*setup)(), void(Derived::*teardown)(), BenchmarkType benchmarkType = BenchmarkType::Default) {
            addCustomBenchmarks<Derived>(benchmarks, batchCount, setup, teardown, nullptr, nullptr, BenchmarkUnits(int(benchmarkType)));
        }

        template<class Derived> void addCustomBenchmarks(std::initializer_list<void(Derived::*)()> benchmarks, std::size_t batchCount, void(Derived::*benchmarkBegin)(), std::uint64_t(Derived::*benchmarkEnd)(), BenchmarkUnits benchmarkUnits) {
            addCustomBenchmarks<Derived>(benchmarks, batchCount, nullptr, nullptr, static_cast<TestCase::BenchmarkBegin>(benchmarkBegin), static_cast<TestCase::BenchmarkEnd>(benchmarkEnd), benchmarkUnits);
        }

        template<class Derived> void addCustomBenchmarks(std::initializer_list<void(Derived::*)()> benchmarks, std::size_t batchCount, void(Derived::*setup)(), void(Derived::*teardown)(), void(Derived::*benchmarkBegin)(), std::uint64_t(Derived::*benchmarkEnd)(), BenchmarkUnits benchmarkUnits) {
            _testCases.reserve(_testCases.size() + benchmarks.size());
            for(auto benchmark: benchmarks)
                _testCases.emplace_back(~std::size_t{}, batchCount, static_cast<TestCase::Function>(benchmark), static_cast<TestCase::Function>(setup), static_cast<TestCase::Function>(teardown), static_cast<TestCase::BenchmarkBegin>(benchmarkBegin), static_cast<TestCase::BenchmarkEnd>(benchmarkEnd), TestCaseType(int(benchmarkUnits)));
        }

        template<class Derived> void addInstancedBenchmarks(std::initializer_list<void(Derived::*)()> benchmarks, std::size_t batchCount, std::size_t instanceCount, BenchmarkType benchmarkType = BenchmarkType::Default) {
            addInstancedBenchmarks<Derived>(benchmarks, batchCount, instanceCount, nullptr, nullptr, benchmarkType);
        }

        template<class Derived> void addInstancedBenchmarks(std::initializer_list<void(Derived::*)()> benchmarks, std::size_t batchCount, std::size_t instanceCount, void(Derived::*setup)(), void(Derived::*teardown)(), BenchmarkType benchmarkType = BenchmarkType::Default) {
            addCustomInstancedBenchmarks<Derived>(benchmarks, batchCount, instanceCount, setup, teardown, nullptr, nullptr, BenchmarkUnits(int(benchmarkType)));
        }

        template<class Derived> void addCustomInstancedBenchmarks(std::initializer_list<void(Derived::*)()> benchmarks, std::size_t batchCount, std::size_t instanceCount, void(Derived::*benchmarkBegin)(), std::uint64_t(Derived::*benchmarkEnd)(), BenchmarkUnits benchmarkUnits) {
            addCustomInstancedBenchmarks<Derived>(benchmarks, batchCount, instanceCount, nullptr, nullptr, benchmarkBegin, benchmarkEnd, benchmarkUnits);
        }

        template<class Derived> void addCustomInstancedBenchmarks(std::initializer_list<void(Derived::*)()> benchmarks, std::size_t batchCount, std::size_t instanceCount, void(Derived::*setup)(), void(Derived::*teardown)(), void(Derived::*benchmarkBegin)(), std::uint64_t(Derived::*benchmarkEnd)(), BenchmarkUnits benchmarkUnits) {
            _testCases.reserve(_testCases.size() + benchmarks.size());
            for(auto benchmark: benchmarks) for(std::size_t i = 0; i != instanceCount; ++i)
                _testCases.emplace_back(i, batchCount, static_cast<TestCase::Function>(benchmark), static_cast<TestCase::Function>(setup), static_cast<TestCase::Function>(teardown), static_cast<TestCase::BenchmarkBegin>(benchmarkBegin), static_cast<TestCase::BenchmarkEnd>(benchmarkEnd), TestCaseType(int(benchmarkUnits)));
        }

        std::size_t testCaseId() const { return _testCaseId; }

        std::size_t testCaseInstanceId() const { return _testCaseInstanceId; }

        std::size_t testCaseRepeatId() const { return _testCaseRepeatId; }

        void setTestName(const std::string& name);
        void setTestName(std::string&& name);

        void setTestCaseName(const std::string& name);
        void setTestCaseName(std::string&& name);

        void setTestCaseDescription(const std::string& description);
        void setTestCaseDescription(std::string&& description);

        void setBenchmarkName(const std::string& name);
        void setBenchmarkName(std::string&& name);

    protected:
        ~Tester();

    public:
        static void registerArguments(int& argc, char** argv);

        static void registerArguments(int& argc, const char** argv) {
            registerArguments(argc, const_cast<char**>(argv));
        }

        int exec();

        int exec(std::ostream* logOutput, std::ostream* errorOutput);

        template<class T> void compare(const char* actual, const T& actualValue, const char* expected, const T& expectedValue) {
            compareAs<T, T, T>(actual, actualValue, expected, expectedValue);
        }

        template<class Actual, class Expected> void compare(const char* actual, const Actual& actualValue, const char* expected, const Expected& expectedValue) {
            compareAs<typename Implementation::CommonType<Actual, Expected>::Type, Actual, Expected>(actual, actualValue, expected, expectedValue);
        }

        template<template<class> class T, class Actual, class Expected> void compareAs(const char* actual, const Actual& actualValue, const char* expected, const Expected& expectedValue) {
            compareAs<T<typename Implementation::CommonType<Actual, Expected>::Type>, Actual, Expected>(actual, actualValue, expected, expectedValue);
        }

        template<class T, class U, class V> void compareAs(const char* actual, const U& actualValue, const char* expected, const V& expectedValue) {
            compareWith(Comparator<T>(), actual, actualValue, expected, expectedValue);
        }

        template<class T, class U, class V> void compareWith(Comparator<T>& comparator, const char* actual, const U& actualValue, const char* expected, const V& expectedValue);
        template<class T, class U, class V> void compareWith(Comparator<T>&& comparator, const char* actual, const U& actualValue, const char* expected, const V& expectedValue) {
            return compareWith<T, U, V>(comparator, actual, actualValue, expected, expectedValue);
        }

        template<class T> void verify(const char* expression, T&& value);

        void registerTest(const char* filename, const char* name);

        void skip(const std::string& message);

    protected:
        class CORRADE_TESTSUITE_EXPORT ExpectedFailure {
            public:
                explicit ExpectedFailure(Tester& instance, std::string message, bool enabled = true);

                template<class T> explicit ExpectedFailure(Tester& instance, std::string message, T&& enabled): ExpectedFailure{instance, message, enabled ? true : false} {}

                ~ExpectedFailure();

                std::string message() const;

            private:
                Tester& _instance;
                std::string _message;
        };

        void registerTestCase(const char* name, int line);

    private:
        class Exception {};
        class SkipException {};

        enum class TestCaseType {
            Test = 0,
            DefaultBenchmark = int(BenchmarkType::Default),
            WallTimeBenchmark = int(BenchmarkType::WallTime),
            CpuTimeBenchmark = int(BenchmarkType::CpuTime),
            CpuCyclesBenchmark = int(BenchmarkType::CpuCycles),
            CustomTimeBenchmark = int(BenchmarkUnits::Nanoseconds),
            CustomCycleBenchmark = int(BenchmarkUnits::Cycles),
            CustomInstructionBenchmark = int(BenchmarkUnits::Instructions),
            CustomMemoryBenchmark = int(BenchmarkUnits::Bytes),
            CustomCountBenchmark = int(BenchmarkUnits::Count)
        };

        struct TestCase {
            typedef void (Tester::*Function)();
            typedef void (Tester::*BenchmarkBegin)();
            typedef std::uint64_t (Tester::*BenchmarkEnd)();

            explicit TestCase(std::size_t instanceId, std::size_t repeatCount, Function test, Function setup, Function teardown): instanceId{instanceId}, repeatCount{repeatCount}, test{test}, setup{setup}, teardown{teardown}, benchmarkBegin{}, benchmarkEnd{}, type{TestCaseType::Test} {}

            explicit TestCase(std::size_t instanceId, std::size_t repeatCount, Function test, Function setup, Function teardown, BenchmarkBegin benchmarkBegin, BenchmarkEnd benchmarkEnd, TestCaseType type): instanceId{instanceId}, repeatCount{repeatCount}, test{test}, setup{setup}, teardown{teardown}, benchmarkBegin{benchmarkBegin}, benchmarkEnd{benchmarkEnd}, type{type} {}

            std::size_t instanceId, repeatCount;
            Function test, setup, teardown;
            BenchmarkBegin benchmarkBegin;
            BenchmarkEnd benchmarkEnd;
            TestCaseType type;
        };

    protected:
        class CORRADE_TESTSUITE_EXPORT BenchmarkRunner {
            public:
                explicit BenchmarkRunner(Tester& instance, TestCase::BenchmarkBegin begin, TestCase::BenchmarkEnd end): _instance(instance), _end{end} {
                    (_instance.*begin)();
                }

                ~BenchmarkRunner() {
                    _instance._benchmarkResult = (_instance.*_end)();
                }

                const char* begin() const { return nullptr; }
                const char* end() const { return reinterpret_cast<char*>(_instance._benchmarkBatchSize); }

            private:
                Tester& _instance;
                TestCase::BenchmarkEnd _end;
        };

        BenchmarkRunner createBenchmarkRunner(std::size_t batchSize);

    private:
        static int* _argc;
        static char** _argv;

        void verifyInternal(const char* expression, bool value);
        void printTestCaseLabel(Debug& out, const char* status, Debug::Color statusColor, Debug::Color labelColor);

        void wallTimeBenchmarkBegin();
        std::uint64_t wallTimeBenchmarkEnd();

        void cpuTimeBenchmarkBegin();
        std::uint64_t cpuTimeBenchmarkEnd();

        void cpuCyclesBenchmarkBegin();
        std::uint64_t cpuCyclesBenchmarkEnd();

        Debug::Flags _useColor;
        std::ostream *_logOutput, *_errorOutput;
        std::vector<TestCase> _testCases;
        std::string _testFilename, _testName, _testCaseName,
            _testCaseDescription, _benchmarkName, _expectFailMessage;
        std::size_t _testCaseId{}, _testCaseInstanceId{~std::size_t{}},
            _testCaseRepeatId{}, _benchmarkBatchSize{}, _testCaseLine{},
            _checkCount{};

        std::uint64_t _benchmarkBegin{};
        std::uint64_t _benchmarkResult{};
        TestCase* _testCase = nullptr;
        bool _expectedFailuresDisabled{};
        ExpectedFailure* _expectedFailure{};
        TesterConfiguration _configuration;
};

#ifdef CORRADE_TESTSUITE_TARGET_XCTEST
#define CORRADE_TEST_MAIN(Class)                                            \
    int CORRADE_VISIBILITY_EXPORT corradeTestMain(int, char**);             \
    int corradeTestMain(int argc, char** argv) {                            \
        Corrade::TestSuite::Tester::registerArguments(argc, argv);          \
        Class t;                                                            \
        t.registerTest(__FILE__, #Class);                                   \
        return t.exec();                                                    \
    }
#else
#define CORRADE_TEST_MAIN(Class)                                            \
    int main(int argc, char** argv) {                                       \
        Corrade::TestSuite::Tester::registerArguments(argc, argv);          \
        Class t;                                                            \
        t.registerTest(__FILE__, #Class);                                   \
        return t.exec();                                                    \
    }
#endif

#ifndef CORRADE_TARGET_ANDROID
#define _CORRADE_REGISTER_TEST_CASE()                                       \
    Tester::registerTestCase(__func__, __LINE__);
#else
#define _CORRADE_REGISTER_TEST_CASE()                                       \
    Tester::registerTestCase(__FUNCTION__, __LINE__);
#endif

#define CORRADE_VERIFY(expression)                                          \
    do {                                                                    \
        _CORRADE_REGISTER_TEST_CASE();                                      \
        Tester::verify(#expression, expression);                            \
    } while(false)

#define CORRADE_COMPARE(actual, expected)                                   \
    do {                                                                    \
        _CORRADE_REGISTER_TEST_CASE();                                      \
        Tester::compare(#actual, actual, #expected, expected);              \
    } while(false)

#define CORRADE_COMPARE_AS(actual, expected, ...)                           \
    do {                                                                    \
        _CORRADE_REGISTER_TEST_CASE();                                      \
        Tester::compareAs<__VA_ARGS__>(#actual, actual, #expected, expected); \
    } while(false)

#define CORRADE_COMPARE_WITH(actual, expected, comparatorInstance)          \
    do {                                                                    \
        _CORRADE_REGISTER_TEST_CASE();                                      \
        Tester::compareWith(comparatorInstance.comparator(), #actual, actual, #expected, expected); \
    } while(false)

#define CORRADE_EXPECT_FAIL(message)                                        \
    Tester::ExpectedFailure _CORRADE_HELPER_PASTE(expectedFailure, __LINE__)(*this, message)

#define CORRADE_EXPECT_FAIL_IF(condition, message)                          \
    Tester::ExpectedFailure _CORRADE_HELPER_PASTE(expectedFailure, __LINE__)(*this, message, condition)

#define CORRADE_SKIP(message)                                               \
    do {                                                                    \
        _CORRADE_REGISTER_TEST_CASE();                                      \
        Tester::skip(message);                                              \
    } while(false)

#ifndef _MSC_VER
#define CORRADE_BENCHMARK(batchSize)                                        \
    _CORRADE_REGISTER_TEST_CASE();                                          \
    for(CORRADE_UNUSED auto&& _CORRADE_HELPER_PASTE(benchmarkIteration, __func__): Tester::createBenchmarkRunner(batchSize))
#else
#define CORRADE_BENCHMARK(batchSize)                                        \
    _CORRADE_REGISTER_TEST_CASE();                                          \
    for(                                                                    \
        __pragma(warning(push)) __pragma(warning(disable: 4189))            \
        CORRADE_UNUSED auto&& _CORRADE_HELPER_PASTE(benchmarkIteration, __func__): Tester::createBenchmarkRunner(batchSize) \
        __pragma(warning(pop))                                              \
    )
#endif

template<class T, class U, class V> void Tester::compareWith(Comparator<T>& comparator, const char* actual, const U& actualValue, const char* expected, const V& expectedValue) {
    ++_checkCount;

    const typename Implementation::ComparatorTraits<T>::ActualType& actualValueInExpectedActualType = actualValue;
    const typename Implementation::ComparatorTraits<T>::ExpectedType& expectedValueInExpectedExpectedType = expectedValue;

    bool equal = comparator(actualValueInExpectedActualType, expectedValueInExpectedExpectedType);
    if(!_expectedFailure) {
        if(equal) return;
    } else if(!equal) {
        Debug out{_logOutput, _useColor};
        printTestCaseLabel(out, " XFAIL", Debug::Color::Yellow, Debug::Color::Default);
        out << "at" << _testFilename << "on line"
            << _testCaseLine << Debug::newline << "       " << _expectedFailure->message()
            << actual << "and" << expected << "failed the comparison.";
        return;
    }

    Error out{_errorOutput, _useColor};
    printTestCaseLabel(out, _expectedFailure ? " XPASS" : "  FAIL", Debug::Color::Red, Debug::Color::Default);
    out << "at" << _testFilename << "on line"
        << _testCaseLine << Debug::newline << "       ";
    if(!_expectedFailure) comparator.printErrorMessage(out, actual, expected);
    else out << actual << "and" << expected << "were expected to fail the comparison.";
    throw Exception();
}

template<class T> void Tester::verify(const char* expression, T&& value) {
    if(value) verifyInternal(expression, true);
    else verifyInternal(expression, false);
}

}}

#endif
#ifndef Corrade_TestSuite_Compare_Container_h
#define Corrade_TestSuite_Compare_Container_h

namespace Corrade { namespace TestSuite {

namespace Compare {

template<class> class Container {};

template<class T> class SortedContainer: public Container<T> {};

}

template<class T> class Comparator<Compare::Container<T>> {
    public:
        bool operator()(const T& actual, const T& expected);

        void printErrorMessage(Utility::Error& e, const char* actual, const char* expected) const;

    private:
        const T* _actualContents;
        const T* _expectedContents;
};

template<class T> bool Comparator<Compare::Container<T>>::operator()(const T& actual, const T& expected) {
    _actualContents = &actual;
    _expectedContents = &expected;

    if(_actualContents->size() != _expectedContents->size()) return false;

    Comparator<typename std::decay<decltype((*_actualContents)[0])>::type> comparator;
    for(std::size_t i = 0; i != _actualContents->size(); ++i)
        if(!comparator((*_actualContents)[i], (*_expectedContents)[i])) return false;

    return true;
}

template<class T> void Comparator<Compare::Container<T>>::printErrorMessage(Utility::Error& e, const char* actual, const char* expected) const {
    e << "Containers" << actual << "and" << expected << "have different";
    if(_actualContents->size() != _expectedContents->size())
        e << "size, actual" << _actualContents->size() << "but" << _expectedContents->size() << "expected. Actual contents:\n       ";
    else
        e << "contents, actual:\n       ";

    e << *_actualContents << Utility::Debug::newline << "        but expected\n       " << *_expectedContents << Utility::Debug::newline << "       ";

    Comparator<typename std::decay<decltype((*_actualContents)[0])>::type> comparator;
    for(std::size_t i = 0, end = std::max(_actualContents->size(), _expectedContents->size()); i != end; ++i) {
        if(_actualContents->size() > i && _expectedContents->size() > i &&
            comparator((*_actualContents)[i], (*_expectedContents)[i])) continue;

        if(_actualContents->size() <= i)
            e << "Expected has" << (*_expectedContents)[i];
        else if(_expectedContents->size() <= i)
            e << "Actual has" << (*_actualContents)[i];
        else
            e << "Actual" << (*_actualContents)[i] << "but" << (*_expectedContents)[i] << "expected";

        e << "on position" << i << Utility::Debug::nospace << ".";
        break;
    }
}

template<class T> class Comparator<Compare::SortedContainer<T>>: public Comparator<Compare::Container<T>> {
    public:
        bool operator()(const T& actual, const T& expected);

    private:
        T _actualSorted, _expectedSorted;
};

template<class T> bool Comparator<Compare::SortedContainer<T>>::operator()(const T& actual, const T& expected) {
    _actualSorted = actual;
    _expectedSorted = expected;

    std::sort(_actualSorted.begin(), _actualSorted.end());
    std::sort(_expectedSorted.begin(), _expectedSorted.end());

    return Comparator<Compare::Container<T>>::operator()(_actualSorted, _expectedSorted);
}

}}

#endif
#ifndef Corrade_TestSuite_Compare_File_h
#define Corrade_TestSuite_Compare_File_h

namespace Corrade { namespace TestSuite {

namespace Compare { class File; }

template<> class CORRADE_TESTSUITE_EXPORT Comparator<Compare::File> {
    public:
        explicit Comparator(std::string pathPrefix = {});

        bool operator()(const std::string& actualFilename, const std::string& expectedFilename);

        void printErrorMessage(Utility::Error& e, const char* actual, const char* expected) const;

    private:
        enum class State {
            Success,
            ReadError
        };

        State _actualState, _expectedState;
        std::string _pathPrefix, _actualFilename, _expectedFilename,
            _actualContents, _expectedContents;
};

namespace Compare {

class CORRADE_TESTSUITE_EXPORT File {
    public:
        explicit File(const std::string& pathPrefix = {});

        Comparator<Compare::File> comparator();

    private:
        Comparator<Compare::File> _c;
};

}

}}

#endif
#ifndef Corrade_TestSuite_Compare_FileToString_h
#define Corrade_TestSuite_Compare_FileToString_h

namespace Corrade { namespace TestSuite {

namespace Compare {

class FileToString {};

}

template<> class CORRADE_TESTSUITE_EXPORT Comparator<Compare::FileToString> {
    public:
        Comparator();

        bool operator()(const std::string& filename, const std::string& expectedContents);

        void printErrorMessage(Utility::Error& e, const char* actual, const char* expected) const;

    private:
        enum class State {
            Success,
            ReadError
        };

        State _state;
        std::string _filename,
            _actualContents, _expectedContents;
};

}}

#endif
#ifndef Corrade_TestSuite_Compare_Numeric_h
#define Corrade_TestSuite_Compare_Numeric_h

namespace Corrade { namespace TestSuite {

namespace Compare {

template<class T> class Less {};

template<class T> class LessOrEqual {};

template<class T> class GreaterOrEqual {};

template<class T> class Greater {};

template<class T> class Around {
    public:
        explicit Around(T epsilon): _c{epsilon} {}

        Comparator<Compare::Around<T>> comparator() { return _c; }

    private:
        Comparator<Compare::Around<T>> _c;
};

template<class T> inline Around<T> around(T epsilon) { return Around<T>{epsilon}; }

}

template<class T> class Comparator<Compare::Less<T>> {
    public:
        bool operator()(const T& actual, const T& expected) {
            _actualValue = &actual;
            _expectedValue = &expected;
            return bool(*_actualValue < *_expectedValue);
        }

        void printErrorMessage(Utility::Error& e, const char* actual, const char* expected) const {
            e << "Value" << actual << "is not less than" << expected
              << Utility::Debug::nospace << ", actual is" << *_actualValue
              << "but expected <" << *_expectedValue;
        }

    private:
        const T* _actualValue;
        const T* _expectedValue;
};

template<class T> class Comparator<Compare::LessOrEqual<T>> {
    public:
        bool operator()(const T& actual, const T& expected) {
            _actualValue = &actual;
            _expectedValue = &expected;
            return bool(*_actualValue <= *_expectedValue);
        }

        void printErrorMessage(Utility::Error& e, const char* actual, const char* expected) const {
            e << "Value" << actual << "is not less than or equal to" << expected
              << Utility::Debug::nospace << ", actual is" << *_actualValue
              << "but expected <=" << *_expectedValue;
        }

    private:
        const T* _actualValue;
        const T* _expectedValue;
};

template<class T> class Comparator<Compare::GreaterOrEqual<T>> {
    public:
        bool operator()(const T& actual, const T& expected) {
            _actualValue = &actual;
            _expectedValue = &expected;
            return bool(*_actualValue >= *_expectedValue);
        }

        void printErrorMessage(Utility::Error& e, const char* actual, const char* expected) const {
            e << "Value" << actual << "is not greater than or equal to" << expected
              << Utility::Debug::nospace << ", actual is" << *_actualValue
              << "but expected >=" << *_expectedValue;
        }

    private:
        const T* _actualValue;
        const T* _expectedValue;
};

template<class T> class Comparator<Compare::Greater<T>> {
    public:
        bool operator()(const T& actual, const T& expected) {
            _actualValue = &actual;
            _expectedValue = &expected;
            return bool(*_actualValue > *_expectedValue);
        }

        void printErrorMessage(Utility::Error& e, const char* actual, const char* expected) const {
            e << "Value" << actual << "is not greater than" << expected
              << Utility::Debug::nospace << ", actual is" << *_actualValue
              << "but expected >" << *_expectedValue;
        }

    private:
        const T* _actualValue;
        const T* _expectedValue;
};

template<class T> class Comparator<Compare::Around<T>> {
    public:
        explicit Comparator(T epsilon): _epsilon(epsilon) {}

        bool operator()(const T& actual, const T& expected) {
            _actualValue = &actual;
            _expectedValue = &expected;
            return *_actualValue >= *_expectedValue - _epsilon &&
                   *_expectedValue + _epsilon >= *_actualValue;
        }

        void printErrorMessage(Utility::Error& e, const char* actual, const char* expected) const {
            e << "Value" << actual << "is not around" << expected
              << Utility::Debug::nospace << ", actual is" << *_actualValue
              << "but" << *_expectedValue - _epsilon << "<= expected <="
              << *_expectedValue + _epsilon;
        }

    private:
        T _epsilon;
        const T* _actualValue;
        const T* _expectedValue;
};

}}

#endif
#ifndef Corrade_TestSuite_Compare_StringToFile_h
#define Corrade_TestSuite_Compare_StringToFile_h

namespace Corrade { namespace TestSuite {

namespace Compare {

class StringToFile {};

}

template<> class CORRADE_TESTSUITE_EXPORT Comparator<Compare::StringToFile> {
    public:
        Comparator();

        bool operator()(const std::string& actualContents, const std::string& filename);

        void printErrorMessage(Utility::Error& e, const char* actual, const char* expected) const;

    private:
        enum class State {
            Success,
            ReadError
        };

        State _state;
        std::string _filename,
            _actualContents, _expectedContents;
};

}}

#endif
#ifdef CORRADE_TESTSUITE_IMPLEMENTATION
#ifndef Corrade_Containers_ArrayView_h
#define Corrade_Containers_ArrayView_h

namespace Corrade { namespace Containers {

namespace Implementation {
    template<class, class> struct ArrayViewConverter;
    template<class> struct ErasedArrayViewConverter;
}

template<class T> class ArrayView {
    public:
        typedef T Type;

        constexpr /*implicit*/ ArrayView(std::nullptr_t) noexcept: _data(nullptr), _size(0) {}

        constexpr /*implicit*/ ArrayView() noexcept: _data(nullptr), _size(0) {}

        constexpr /*implicit*/ ArrayView(T* data, std::size_t size) noexcept: _data(data), _size(size) {}

        template<class U, std::size_t size, class = typename std::enable_if<std::is_convertible<U*, T*>::value>::type>
        constexpr /*implicit*/ ArrayView(U(&data)[size]) noexcept: _data{data}, _size{size} {
            static_assert(sizeof(T) == sizeof(U), "type sizes are not compatible");
        }

        template<class U, class = typename std::enable_if<std::is_convertible<U*, T*>::value>::type>
        constexpr /*implicit*/ ArrayView(ArrayView<U> view) noexcept: _data{view}, _size{view.size()} {
            static_assert(sizeof(T) == sizeof(U), "type sizes are not compatible");
        }

        template<std::size_t size, class U, class = typename std::enable_if<std::is_convertible<U*, T*>::value>::type>
        constexpr /*implicit*/ ArrayView(StaticArrayView<size, U> view) noexcept: _data{view}, _size{size} {
            static_assert(sizeof(U) == sizeof(T), "type sizes are not compatible");
        }

        template<class U, class = decltype(Implementation::ArrayViewConverter<T, typename std::decay<U&&>::type>::from(std::declval<U&&>()))> constexpr /*implicit*/ ArrayView(U&& other) noexcept: ArrayView{Implementation::ArrayViewConverter<T, typename std::decay<U&&>::type>::from(std::forward<U>(other))} {}

        template<class U, class = decltype(Implementation::ArrayViewConverter<T, U>::to(std::declval<ArrayView<T>>()))> constexpr /*implicit*/ operator U() const {
            return Implementation::ArrayViewConverter<T, U>::to(*this);
        }

        #ifndef CORRADE_MSVC2017_COMPATIBILITY
        constexpr explicit operator bool() const { return _data; }
        #endif

        constexpr /*implicit*/ operator T*() const { return _data; }

        constexpr T* data() const { return _data; }

        constexpr std::size_t size() const { return _size; }

        constexpr bool empty() const { return !_size; }

        constexpr T* begin() const { return _data; }
        constexpr T* cbegin() const { return _data; }

        constexpr T* end() const { return _data+_size; }
        constexpr T* cend() const { return _data+_size; }

        T& front() const;

        T& back() const;

        constexpr ArrayView<T> slice(T* begin, T* end) const;

        constexpr ArrayView<T> slice(std::size_t begin, std::size_t end) const;

        template<std::size_t viewSize> constexpr StaticArrayView<viewSize, T> slice(T* begin) const;

        template<std::size_t viewSize> constexpr StaticArrayView<viewSize, T> slice(std::size_t begin) const;

        constexpr ArrayView<T> prefix(T* end) const {
            return end ? slice(_data, end) : nullptr;
        }

        constexpr ArrayView<T> prefix(std::size_t end) const {
            return slice(0, end);
        }

        template<std::size_t viewSize> constexpr StaticArrayView<viewSize, T> prefix() const {
            return slice<viewSize>(_data);
        }

        constexpr ArrayView<T> suffix(T* begin) const {
            return _data && !begin ? nullptr : slice(begin, _data + _size);
        }

        constexpr ArrayView<T> suffix(std::size_t begin) const {
            return slice(begin, _size);
        }

    private:
        T* _data;
        std::size_t _size;
};

template<> class ArrayView<const void> {
    public:
        typedef const void Type;

        constexpr /*implicit*/ ArrayView(std::nullptr_t) noexcept: _data(nullptr), _size(0) {}

        constexpr /*implicit*/ ArrayView() noexcept: _data(nullptr), _size(0) {}

        constexpr /*implicit*/ ArrayView(const void* data, std::size_t size) noexcept: _data(data), _size(size) {}

        template<class T> constexpr /*implicit*/ ArrayView(const T* data, std::size_t size) noexcept: _data(data), _size(size*sizeof(T)) {}

        template<class T, std::size_t size> constexpr /*implicit*/ ArrayView(T(&data)[size]) noexcept: _data(data), _size(size*sizeof(T)) {}

        template<class T> constexpr /*implicit*/ ArrayView(const ArrayView<T>& array) noexcept: _data(array), _size(array.size()*sizeof(T)) {}

        template<std::size_t size, class T> constexpr /*implicit*/ ArrayView(const StaticArrayView<size, T>& array) noexcept: _data{array}, _size{size*sizeof(T)} {}

        template<class T, class = decltype(Implementation::ErasedArrayViewConverter<const T>::from(std::declval<const T&>()))> constexpr /*implicit*/ ArrayView(const T& other) noexcept: ArrayView{Implementation::ErasedArrayViewConverter<const T>::from(other)} {}

        #ifndef CORRADE_MSVC2017_COMPATIBILITY
        constexpr explicit operator bool() const { return _data; }
        #endif

        constexpr /*implicit*/ operator const void*() const { return _data; }

        constexpr const void* data() const { return _data; }

        constexpr std::size_t size() const { return _size; }

        constexpr bool empty() const { return !_size; }

    private:
        const void* _data;
        std::size_t _size;
};

template<class T> constexpr ArrayView<T> arrayView(T* data, std::size_t size) {
    return ArrayView<T>{data, size};
}

template<std::size_t size, class T> constexpr ArrayView<T> arrayView(T(&data)[size]) {
    return ArrayView<T>{data};
}

template<std::size_t size, class T> constexpr ArrayView<T> arrayView(StaticArrayView<size, T> view) {
    return ArrayView<T>{view};
}

template<class T> constexpr ArrayView<T> arrayView(ArrayView<T> view) {
    return view;
}

template<class T, class U = decltype(Implementation::ErasedArrayViewConverter<typename std::remove_reference<T&&>::type>::from(std::declval<T&&>()))> constexpr U arrayView(T&& other) {
    return Implementation::ErasedArrayViewConverter<typename std::remove_reference<T&&>::type>::from(std::forward<T>(other));
}

template<class U, class T> ArrayView<U> arrayCast(ArrayView<T> view) {
    static_assert(std::is_standard_layout<T>::value, "the source type is not standard layout");
    static_assert(std::is_standard_layout<U>::value, "the target type is not standard layout");
    const std::size_t size = view.size()*sizeof(T)/sizeof(U);
    CORRADE_ASSERT(size*sizeof(U) == view.size()*sizeof(T),
        "Containers::arrayCast(): can't reinterpret" << view.size() << sizeof(T) << Utility::Debug::nospace << "-byte items into a" << sizeof(U) << Utility::Debug::nospace << "-byte type", {});
    return {reinterpret_cast<U*>(view.begin()), size};
}

template<class T> constexpr std::size_t arraySize(ArrayView<T> view) {
    return view.size();
}

template<std::size_t size_, class T> constexpr std::size_t arraySize(StaticArrayView<size_, T>) {
    return size_;
}

template<std::size_t size_, class T> constexpr std::size_t arraySize(T(&)[size_]) {
    return size_;
}

namespace Implementation {
    template<std::size_t, class, class> struct StaticArrayViewConverter;
    template<class> struct ErasedStaticArrayViewConverter;
}

template<std::size_t size_, class T> class StaticArrayView {
    public:
        typedef T Type;

        enum: std::size_t {
            Size = size_
        };

        constexpr /*implicit*/ StaticArrayView(std::nullptr_t) noexcept: _data(nullptr) {}

        constexpr /*implicit*/ StaticArrayView() noexcept: _data(nullptr) {}

        template<class U, class = typename std::enable_if<std::is_pointer<U>::value && !std::is_same<U, T(&)[size_]>::value>::type> constexpr explicit StaticArrayView(U data)
        noexcept: _data(data) {}

        template<class U, class = typename std::enable_if<std::is_convertible<U*, T*>::value>::type>
        constexpr /*implicit*/ StaticArrayView(U(&data)[size_]) noexcept: _data{data} {}

        template<class U, class = typename std::enable_if<std::is_convertible<U*, T*>::value>::type>
        constexpr /*implicit*/ StaticArrayView(StaticArrayView<size_, U> view) noexcept: _data{view} {
            static_assert(sizeof(T) == sizeof(U), "type sizes are not compatible");
        }

        template<class U, class = decltype(Implementation::StaticArrayViewConverter<size_, T, typename std::decay<U&&>::type>::from(std::declval<U&&>()))> constexpr /*implicit*/ StaticArrayView(U&& other) noexcept: StaticArrayView{Implementation::StaticArrayViewConverter<size_, T, typename std::decay<U&&>::type>::from(std::forward<U>(other))} {}

        template<class U, class = decltype(Implementation::StaticArrayViewConverter<size_, T, U>::to(std::declval<StaticArrayView<size_, T>>()))> constexpr /*implicit*/ operator U() const {
            return Implementation::StaticArrayViewConverter<size_, T, U>::to(*this);
        }

        #ifndef CORRADE_MSVC2017_COMPATIBILITY
        constexpr explicit operator bool() const { return _data; }
        #endif

        constexpr /*implicit*/ operator T*() const { return _data; }

        constexpr T* data() const { return _data; }

        constexpr std::size_t size() const { return size_; }

        constexpr bool empty() const { return !size_; }

        constexpr T* begin() const { return _data; }
        constexpr T* cbegin() const { return _data; }

        constexpr T* end() const { return _data + size_; }
        constexpr T* cend() const { return _data + size_; }

        T& front() const;

        T& back() const;

        constexpr ArrayView<T> slice(T* begin, T* end) const {
            return ArrayView<T>(*this).slice(begin, end);
        }
        constexpr ArrayView<T> slice(std::size_t begin, std::size_t end) const {
            return ArrayView<T>(*this).slice(begin, end);
        }

        template<std::size_t viewSize> constexpr StaticArrayView<viewSize, T> slice(T* begin) const {
            return ArrayView<T>(*this).template slice<viewSize>(begin);
        }
        template<std::size_t viewSize> constexpr StaticArrayView<viewSize, T> slice(std::size_t begin) const {
            return ArrayView<T>(*this).template slice<viewSize>(begin);
        }

        constexpr ArrayView<T> prefix(T* end) const {
            return ArrayView<T>(*this).prefix(end);
        }
        constexpr ArrayView<T> prefix(std::size_t end) const {
            return ArrayView<T>(*this).prefix(end);
        }

        template<std::size_t viewSize> constexpr StaticArrayView<viewSize, T> prefix() const;

        constexpr ArrayView<T> suffix(T* begin) const {
            return ArrayView<T>(*this).suffix(begin);
        }
        constexpr ArrayView<T> suffix(std::size_t begin) const {
            return ArrayView<T>(*this).suffix(begin);
        }

    private:
        T* _data;
};

template<std::size_t size, class T> constexpr StaticArrayView<size, T> staticArrayView(T* data) {
    return StaticArrayView<size, T>{data};
}

template<std::size_t size, class T> constexpr StaticArrayView<size, T> staticArrayView(T(&data)[size]) {
    return StaticArrayView<size, T>{data};
}

template<std::size_t size, class T> constexpr StaticArrayView<size, T> staticArrayView(StaticArrayView<size, T> view) {
    return view;
}

template<class T, class U = decltype(Implementation::ErasedStaticArrayViewConverter<typename std::remove_reference<T&&>::type>::from(std::declval<T&&>()))> constexpr U staticArrayView(T&& other) {
    return Implementation::ErasedStaticArrayViewConverter<typename std::remove_reference<T&&>::type>::from(std::forward<T>(other));
}

template<class U, std::size_t size, class T> StaticArrayView<size*sizeof(T)/sizeof(U), U> arrayCast(StaticArrayView<size, T> view) {
    static_assert(std::is_standard_layout<T>::value, "the source type is not standard layout");
    static_assert(std::is_standard_layout<U>::value, "the target type is not standard layout");
    constexpr const std::size_t newSize = size*sizeof(T)/sizeof(U);
    static_assert(newSize*sizeof(U) == size*sizeof(T),
        "type sizes are not compatible");
    return StaticArrayView<newSize, U>{reinterpret_cast<U*>(view.begin())};
}

template<class U, std::size_t size, class T> StaticArrayView<size*sizeof(T)/sizeof(U), U> arrayCast(T(&data)[size]) {
    return arrayCast<U>(StaticArrayView<size, T>{data});
}

template<class T> T& ArrayView<T>::front() const {
    CORRADE_ASSERT(_size, "Containers::ArrayView::front(): view is empty", _data[0]);
    return _data[0];
}

template<class T> T& ArrayView<T>::back() const {
    CORRADE_ASSERT(_size, "Containers::ArrayView::back(): view is empty", _data[_size - 1]);
    return _data[_size - 1];
}

template<class T> constexpr ArrayView<T> ArrayView<T>::slice(T* begin, T* end) const {
    return CORRADE_CONSTEXPR_ASSERT(_data <= begin && begin <= end && end <= _data + _size,
            "Containers::ArrayView::slice(): slice ["
            << Utility::Debug::nospace << begin - _data
            << Utility::Debug::nospace << ":"
            << Utility::Debug::nospace << end - _data
            << Utility::Debug::nospace << "] out of range for" << _size
            << "elements"),
        ArrayView<T>{begin, std::size_t(end - begin)};
}

template<class T> constexpr ArrayView<T> ArrayView<T>::slice(std::size_t begin, std::size_t end) const {
    return CORRADE_CONSTEXPR_ASSERT(begin <= end && end <= _size,
            "Containers::ArrayView::slice(): slice ["
            << Utility::Debug::nospace << begin
            << Utility::Debug::nospace << ":"
            << Utility::Debug::nospace << end
            << Utility::Debug::nospace << "] out of range for" << _size
            << "elements"),
        ArrayView<T>{_data + begin, end - begin};
}

template<std::size_t size_, class T> T& StaticArrayView<size_, T>::front() const {
    static_assert(size_, "view is empty");
    return _data[0];
}

template<std::size_t size_, class T> T& StaticArrayView<size_, T>::back() const {
    static_assert(size_, "view is empty");
    return _data[size_ - 1];
}

template<class T> template<std::size_t viewSize> constexpr StaticArrayView<viewSize, T> ArrayView<T>::slice(T* begin) const {
    return CORRADE_CONSTEXPR_ASSERT(_data <= begin && begin + viewSize <= _data + _size,
            "Containers::ArrayView::slice(): slice ["
            << Utility::Debug::nospace << begin - _data
            << Utility::Debug::nospace << ":"
            << Utility::Debug::nospace << begin + viewSize - _data
            << Utility::Debug::nospace << "] out of range for" << _size
            << "elements"),
        StaticArrayView<viewSize, T>{begin};
}

template<class T> template<std::size_t viewSize> constexpr StaticArrayView<viewSize, T> ArrayView<T>::slice(std::size_t begin) const {
    return CORRADE_CONSTEXPR_ASSERT(begin + viewSize <= _size,
            "Containers::ArrayView::slice(): slice ["
            << Utility::Debug::nospace << begin
            << Utility::Debug::nospace << ":"
            << Utility::Debug::nospace << begin + viewSize
            << Utility::Debug::nospace << "] out of range for" << _size
            << "elements"),
        StaticArrayView<viewSize, T>{_data + begin};
}

template<std::size_t size_, class T> template<std::size_t viewSize> constexpr StaticArrayView<viewSize, T> StaticArrayView<size_, T>::prefix() const {
    static_assert(viewSize <= size_, "prefix size too large");
    return StaticArrayView<viewSize, T>{_data};
}

}}

#endif
#ifndef Corrade_Containers_Array_h
#define Corrade_Containers_Array_h

namespace Corrade { namespace Containers {

namespace Implementation {
    template<class T> struct DefaultDeleter {
        T operator()() const { return T{}; }
    };
    template<class T> struct DefaultDeleter<void(*)(T*, std::size_t)> {
        void(*operator()() const)(T*, std::size_t) { return nullptr; }
    };

    template<class T, class D> struct CallDeleter {
        void operator()(D deleter, T* data, std::size_t size) const {
            deleter(data, size);
        }
    };
    template<class T> struct CallDeleter<T, void(*)(T*, std::size_t)> {
        void operator()(void(*deleter)(T*, std::size_t), T* data, std::size_t size) const {
            if(deleter) deleter(data, size);
            else delete[] data;
        }
    };

    template<class T> void noInitDeleter(T* data, std::size_t size) {
        if(data) for(T *it = data, *end = data + size; it != end; ++it)
            it->~T();
        delete[] reinterpret_cast<char*>(data);
    }
}

template<class T, class D>
class Array {
    public:
        typedef T Type;
        typedef D Deleter;

        template<class U, class V = typename std::enable_if<std::is_same<std::nullptr_t, U>::value>::type> /*implicit*/ Array(U) noexcept:
            _data{nullptr}, _size{0}, _deleter(Implementation::DefaultDeleter<D>{}()) {}

        /*implicit*/ Array() noexcept: _data(nullptr), _size(0), _deleter(Implementation::DefaultDeleter<D>{}()) {}

        explicit Array(DefaultInitT, std::size_t size): _data{size ? new T[size] : nullptr}, _size{size}, _deleter{nullptr} {}

        explicit Array(ValueInitT, std::size_t size): _data{size ? new T[size]() : nullptr}, _size{size}, _deleter{nullptr} {}

        explicit Array(NoInitT, std::size_t size): _data{size ? reinterpret_cast<T*>(new char[size*sizeof(T)]) : nullptr}, _size{size}, _deleter{Implementation::noInitDeleter} {}

        template<class... Args> explicit Array(DirectInitT, std::size_t size, Args&&... args);

        explicit Array(InPlaceInitT, std::initializer_list<T> list);

        explicit Array(std::size_t size): Array{DefaultInit, size} {}

        explicit Array(T* data, std::size_t size, D deleter = Implementation::DefaultDeleter<D>{}()): _data{data}, _size{size}, _deleter(deleter) {}

        ~Array() { Implementation::CallDeleter<T, D>{}(_deleter, _data, _size); }

        Array(const Array<T, D>&) = delete;

        Array(Array<T, D>&& other) noexcept;

        Array<T, D>& operator=(const Array<T, D>&) = delete;

        Array<T, D>& operator=(Array<T, D>&&) noexcept;

        template<class U, class = decltype(Implementation::ArrayViewConverter<T, U>::to(std::declval<ArrayView<T>>()))> /*implicit*/ operator U() {
            return Implementation::ArrayViewConverter<T, U>::to(*this);
        }

        template<class U, class = decltype(Implementation::ArrayViewConverter<const T, U>::to(std::declval<ArrayView<const T>>()))> constexpr /*implicit*/ operator U() const {
            return Implementation::ArrayViewConverter<const T, U>::to(*this);
        }

        #ifndef CORRADE_MSVC2017_COMPATIBILITY
        explicit operator bool() const { return _data; }
        #endif

        template<class U, class = typename std::enable_if<!std::is_void<U>::value && std::is_convertible<T*, U*>::value>::type>
        /*implicit*/ operator ArrayView<U>() noexcept {
            static_assert(sizeof(T) == sizeof(U), "type sizes are not compatible");
            return {_data, _size};
        }

        template<class U, class = typename std::enable_if<std::is_convertible<T*, U*>::value || std::is_convertible<T*, const U*>::value>::type>
        /*implicit*/ operator ArrayView<const U>() const noexcept {
            static_assert(sizeof(T) == sizeof(U), "type sizes are not compatible");
            return {_data, _size};
        }

        /*implicit*/ operator ArrayView<const void>() const noexcept {
            return {_data, _size};
        }

        /*implicit*/ operator T*() & { return _data; }

        /*implicit*/ operator const T*() const & { return _data; }

        T* data() { return _data; }
        const T* data() const { return _data; }

        D deleter() const { return _deleter; }

        std::size_t size() const { return _size; }

        bool empty() const { return !_size; }

        T* begin() { return _data; }
        const T* begin() const { return _data; }
        const T* cbegin() const { return _data; }

        T* end() { return _data+_size; }
        const T* end() const { return _data+_size; }
        const T* cend() const { return _data+_size; }

        T& front();
        const T& front() const;

        T& back();
        const T& back() const;

        ArrayView<T> slice(T* begin, T* end) {
            return ArrayView<T>(*this).slice(begin, end);
        }
        ArrayView<const T> slice(const T* begin, const T* end) const {
            return ArrayView<const T>(*this).slice(begin, end);
        }
        ArrayView<T> slice(std::size_t begin, std::size_t end) {
            return ArrayView<T>(*this).slice(begin, end);
        }
        ArrayView<const T> slice(std::size_t begin, std::size_t end) const {
            return ArrayView<const T>(*this).slice(begin, end);
        }

        template<std::size_t size> StaticArrayView<size, T> slice(T* begin) {
            return ArrayView<T>(*this).template slice<size>(begin);
        }
        template<std::size_t size> StaticArrayView<size, const T> slice(const T* begin) const {
            return ArrayView<const T>(*this).template slice<size>(begin);
        }
        template<std::size_t size> StaticArrayView<size, T> slice(std::size_t begin) {
            return ArrayView<T>(*this).template slice<size>(begin);
        }
        template<std::size_t size> StaticArrayView<size, const T> slice(std::size_t begin) const {
            return ArrayView<const T>(*this).template slice<size>(begin);
        }

        ArrayView<T> prefix(T* end) {
            return ArrayView<T>(*this).prefix(end);
        }
        ArrayView<const T> prefix(const T* end) const {
            return ArrayView<const T>(*this).prefix(end);
        }
        ArrayView<T> prefix(std::size_t end) {
            return ArrayView<T>(*this).prefix(end);
        }
        ArrayView<const T> prefix(std::size_t end) const {
            return ArrayView<const T>(*this).prefix(end);
        }

        template<std::size_t viewSize> StaticArrayView<viewSize, T> prefix() {
            return ArrayView<T>(*this).template prefix<viewSize>();
        }
        template<std::size_t viewSize> StaticArrayView<viewSize, const T> prefix() const {
            return ArrayView<const T>(*this).template prefix<viewSize>();
        }

        ArrayView<T> suffix(T* begin) {
            return ArrayView<T>(*this).suffix(begin);
        }
        ArrayView<const T> suffix(const T* begin) const {
            return ArrayView<const T>(*this).suffix(begin);
        }
        ArrayView<T> suffix(std::size_t begin) {
            return ArrayView<T>(*this).suffix(begin);
        }
        ArrayView<const T> suffix(std::size_t begin) const {
            return ArrayView<const T>(*this).suffix(begin);
        }

        T* release();

    private:
        T* _data;
        std::size_t _size;
        D _deleter;
};

template<class T, class D> inline ArrayView<T> arrayView(Array<T, D>& array) {
    return ArrayView<T>{array};
}

template<class T, class D> inline ArrayView<const T> arrayView(const Array<T, D>& array) {
    return ArrayView<const T>{array};
}

template<class U, class T, class D> inline ArrayView<U> arrayCast(Array<T, D>& array) {
    return arrayCast<U>(arrayView(array));
}

template<class U, class T, class D> inline ArrayView<const U> arrayCast(const Array<T, D>& array) {
    return arrayCast<const U>(arrayView(array));
}

template<class T> std::size_t arraySize(const Array<T>& view) {
    return view.size();
}

template<class T, class D> inline Array<T, D>::Array(Array<T, D>&& other) noexcept: _data{other._data}, _size{other._size}, _deleter{other._deleter} {
    other._data = nullptr;
    other._size = 0;
}

template<class T, class D> template<class ...Args> Array<T, D>::Array(DirectInitT, std::size_t size, Args&&... args): Array{NoInit, size} {
    for(std::size_t i = 0; i != size; ++i)
        new(_data + i) T{std::forward<Args>(args)...};
}

template<class T, class D> Array<T, D>::Array(InPlaceInitT, std::initializer_list<T> list): Array{NoInit, list.size()} {
    std::size_t i = 0;
    for(const T& item: list) new(_data + i++) T{item};
}

template<class T, class D> inline Array<T, D>& Array<T, D>::operator=(Array<T, D>&& other) noexcept {
    using std::swap;
    swap(_data, other._data);
    swap(_size, other._size);
    swap(_deleter, other._deleter);
    return *this;
}

template<class T, class D> const T& Array<T, D>::front() const {
    CORRADE_ASSERT(_size, "Containers::Array::front(): array is empty", _data[0]);
    return _data[0];
}

template<class T, class D> const T& Array<T, D>::back() const {
    CORRADE_ASSERT(_size, "Containers::Array::back(): array is empty", _data[_size - 1]);
    return _data[_size - 1];
}

template<class T, class D> T& Array<T, D>::front() {
    return const_cast<T&>(static_cast<const Array<T, D>&>(*this).front());
}

template<class T, class D> T& Array<T, D>::back() {
    return const_cast<T&>(static_cast<const Array<T, D>&>(*this).back());
}

template<class T, class D> inline T* Array<T, D>::release() {
    T* const data = _data;
    _data = nullptr;
    _size = 0;
    return data;
}

}}

#endif
#ifndef Corrade_TestSuite_Implementation_BenchmarkCounters_h
#define Corrade_TestSuite_Implementation_BenchmarkCounters_h

namespace Corrade { namespace TestSuite { namespace Implementation {

inline std::uint64_t wallTime() {
    return  std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
}

inline std::uint64_t cpuTime() {
    return std::clock()*1000000000/CLOCKS_PER_SEC;
}

inline std::uint64_t rdtsc() {
    return 0;
}

}}}

#endif
#ifndef Corrade_TestSuite_Implementation_BenchmarkStats_h
#define Corrade_TestSuite_Implementation_BenchmarkStats_h

namespace Corrade { namespace TestSuite { namespace Implementation {

inline std::tuple<double, double, Utility::Debug::Color> calculateStats(const Containers::ArrayView<const std::uint64_t> measurements, const std::size_t batchSize, const double yellowThreshold, const double redThreshold) {
    if(measurements.empty() || !batchSize)
        return std::make_tuple(std::numeric_limits<double>::quiet_NaN(), std::numeric_limits<double>::quiet_NaN(), Utility::Debug::Color::Red);

    double mean{};
    for(std::uint64_t v: measurements)
        mean += double(v)/double(batchSize*measurements.size());

    double variance{};
    for(std::uint64_t v: measurements) {
        double dev = double(v)/batchSize - mean;
        variance += dev*dev/(measurements.size() - 1);
    }

    const double stddev = std::sqrt(variance);

    Utility::Debug::Color color;
    const double absavg = std::abs(mean);
    if(stddev >= absavg*redThreshold)
        color = Utility::Debug::Color::Red;
    else if(stddev >= absavg*yellowThreshold)
        color = Utility::Debug::Color::Yellow;
    else
        color = Utility::Debug::Color::Default;

    return std::make_tuple(mean, stddev, color);
}

inline void printValue(Utility::Debug& out, const double mean, const double stddev, const Utility::Debug::Color color, const double divisor, const char* const unitPrefix, const char* const unit) {
    std::ostringstream meanFormatter, stddevFormatter;
    meanFormatter << std::right << std::fixed << std::setprecision(2) << std::setw(6) << mean/divisor;
    stddevFormatter << std::left << std::fixed << std::setprecision(2) << std::setw(6) << stddev/divisor;

    if(mean != mean) {
        out << Utility::Debug::boldColor(Utility::Debug::Color::Red)
            << "(no data)       " << Utility::Debug::resetColor << unit;

    } else if(stddev != stddev) {
        out << Utility::Debug::boldColor(Utility::Debug::Color::Green)
            << meanFormatter.str() << Utility::Debug::resetColor << "        "
            << unitPrefix << Utility::Debug::nospace << unit;

    } else {
        out << Utility::Debug::boldColor(Utility::Debug::Color::Green)
            << meanFormatter.str() << "±" << Utility::Debug::boldColor(color)
            << stddevFormatter.str() << Utility::Debug::resetColor
            << unitPrefix << Utility::Debug::nospace << unit;
    }
}

inline void printTime(Utility::Debug& out, const double mean, const double stddev, const Utility::Debug::Color color) {
    const double max = std::max(mean, stddev);

    if(max >= 1000000000.0)
        printValue(out, mean, stddev, color, 1000000000.0, " ", "s");
    else if(max >= 1000000.0)
        printValue(out, mean, stddev, color, 1000000.0, "m", "s");
    else if(max >= 1000.0)
        printValue(out, mean, stddev, color, 1000.0, "µ", "s");
    else
        printValue(out, mean, stddev, color, 1.0, "n", "s");
}

inline void printCount(Utility::Debug& out, const double mean, const double stddev, const Utility::Debug::Color color, double multiplier, const char* const unit) {
    double max = std::max(mean, stddev);

    if(max >= multiplier*multiplier*multiplier)
        printValue(out, mean, stddev, color, multiplier*multiplier*multiplier, "G", unit);
    else if(max >= multiplier*multiplier)
        printValue(out, mean, stddev, color, multiplier*multiplier, "M", unit);
    else if(max >= multiplier)
        printValue(out, mean, stddev, color, multiplier, "k", unit);
    else
        printValue(out, mean, stddev, color, 1.0, " ", unit);
}

inline void printStats(Utility::Debug& out, const double mean, const double stddev, const Utility::Debug::Color color, const Tester::BenchmarkUnits unit) {
    switch(unit) {
        case Tester::BenchmarkUnits::Nanoseconds:
            printTime(out, mean, stddev, color);
            return;
        case Tester::BenchmarkUnits::Cycles:
            printCount(out, mean, stddev, color, 1000.0, "C");
            return;
        case Tester::BenchmarkUnits::Instructions:
            printCount(out, mean, stddev, color, 1000.0, "I");
            return;
        case Tester::BenchmarkUnits::Bytes:
            printCount(out, mean, stddev, color, 1024.0, "B");
            return;
        case Tester::BenchmarkUnits::Count:
            printCount(out, mean, stddev, color, 1000.0, " ");
            return;
    }

    CORRADE_ASSERT_UNREACHABLE();
}

}}}

#endif
#ifndef Corrade_Utility_ConfigurationValue_h
#define Corrade_Utility_ConfigurationValue_h

namespace Corrade { namespace Utility {

enum class ConfigurationValueFlag: std::uint8_t {
    Oct = 1 << 0,
    Hex = 1 << 1,
    Scientific = 1 << 2,
    Uppercase = 1 << 3
};

typedef Containers::EnumSet<ConfigurationValueFlag> ConfigurationValueFlags;

CORRADE_ENUMSET_OPERATORS(ConfigurationValueFlags)

template<class T> struct ConfigurationValue {
    ConfigurationValue() = delete;

};

namespace Implementation {
    template<class T> struct CORRADE_UTILITY_EXPORT BasicConfigurationValue {
        BasicConfigurationValue() = delete;

        static std::string toString(const T& value, ConfigurationValueFlags flags);
        static T fromString(const std::string& stringValue, ConfigurationValueFlags flags);
    };
}

template<> struct ConfigurationValue<short>: public Implementation::BasicConfigurationValue<short> {};

template<> struct ConfigurationValue<unsigned short>: public Implementation::BasicConfigurationValue<unsigned short> {};

template<> struct ConfigurationValue<int>: public Implementation::BasicConfigurationValue<int> {};

template<> struct ConfigurationValue<unsigned int>: public Implementation::BasicConfigurationValue<unsigned int> {};

template<> struct ConfigurationValue<long>: public Implementation::BasicConfigurationValue<long> {};

template<> struct ConfigurationValue<unsigned long>: public Implementation::BasicConfigurationValue<unsigned long> {};

template<> struct ConfigurationValue<long long>: public Implementation::BasicConfigurationValue<long long> {};

template<> struct ConfigurationValue<unsigned long long>: public Implementation::BasicConfigurationValue<unsigned long long> {};

template<> struct ConfigurationValue<float>: public Implementation::BasicConfigurationValue<float> {};

template<> struct ConfigurationValue<double>: public Implementation::BasicConfigurationValue<double> {};

template<> struct ConfigurationValue<long double>: public Implementation::BasicConfigurationValue<long double> {};

template<> struct ConfigurationValue<std::string>: public Implementation::BasicConfigurationValue<std::string> {};

template<> struct CORRADE_UTILITY_EXPORT ConfigurationValue<bool> {
    ConfigurationValue() = delete;

    static bool fromString(const std::string& value, ConfigurationValueFlags flags);
    static std::string toString(bool value, ConfigurationValueFlags flags);
};

template<> struct CORRADE_UTILITY_EXPORT ConfigurationValue<char32_t> {
    ConfigurationValue() = delete;

    static char32_t fromString(const std::string& value, ConfigurationValueFlags);
    static std::string toString(char32_t value, ConfigurationValueFlags);
};

}}

#endif
#ifndef Corrade_Utility_Arguments_h
#define Corrade_Utility_Arguments_h

namespace Corrade { namespace Utility {

class CORRADE_UTILITY_EXPORT Arguments {
    public:
        static std::vector<std::string> environment();

        explicit Arguments();

        explicit Arguments(const std::string& prefix);

        Arguments(const Arguments&) = delete;

        Arguments(Arguments&& other) noexcept;

        Arguments& operator=(const Arguments&) = delete;

        Arguments& operator=(Arguments&& other) noexcept;

        ~Arguments();

        std::string prefix() const;

        bool isParsed() const { return _isParsed; }

        Arguments& addArgument(std::string key);

        Arguments& addNamedArgument(char shortKey, std::string key);

        Arguments& addNamedArgument(std::string key) {
            return addNamedArgument('\0', std::move(key));
        }

        Arguments& addOption(char shortKey, std::string key, std::string defaultValue = std::string());

        Arguments& addOption(std::string key, std::string defaultValue = std::string()) {
            return addOption('\0', std::move(key), std::move(defaultValue));
        }

        Arguments& addBooleanOption(char shortKey, std::string key);

        Arguments& addBooleanOption(std::string key) {
            return addBooleanOption('\0', std::move(key));
        }

        Arguments& addSkippedPrefix(std::string prefix, std::string help = {});

        #ifndef CORRADE_TARGET_WINDOWS_RT
        Arguments& setFromEnvironment(const std::string& key, std::string environmentVariable);
        Arguments& setFromEnvironment(const std::string& key);
        #else
        template<class T, class U> Arguments& setFromEnvironment(T&&, U&&) {
            return *this;
        }
        template<class T> Arguments& setFromEnvironment(T&&) {
            return *this;
        }
        #endif

        Arguments& setCommand(std::string name);

        Arguments& setGlobalHelp(std::string help);

        Arguments& setHelp(const std::string& key, std::string help, std::string helpKey = {});

        void parse(int argc, const char** argv);

        void parse(int argc, char** argv) {
            parse(argc, const_cast<const char**>(argv));
        }

        void parse(int argc, std::nullptr_t argv) {
            parse(argc, static_cast<const char**>(argv));
        }

        bool tryParse(int argc, const char** argv);

        bool tryParse(int argc, char** argv) {
            return tryParse(argc, const_cast<const char**>(argv));
        }

        bool tryParse(int argc, std::nullptr_t argv) {
            return tryParse(argc, static_cast<const char**>(argv));
        }

        std::string usage() const;

        std::string help() const;

        template<class T = std::string> T value(const std::string& key, ConfigurationValueFlags flags = {}) const;

        bool isSet(const std::string& key) const;

    private:
        struct CORRADE_UTILITY_LOCAL Entry;

        bool CORRADE_UTILITY_LOCAL skippedPrefix(const std::string& key) const;
        bool CORRADE_UTILITY_LOCAL verifyKey(const std::string& key) const;
        bool CORRADE_UTILITY_LOCAL verifyKey(char shortKey) const;
        std::vector<Entry>::iterator CORRADE_UTILITY_LOCAL find(const std::string& key);
        std::vector<Entry>::const_iterator CORRADE_UTILITY_LOCAL find(const std::string& key) const;
        std::vector<Entry>::iterator CORRADE_UTILITY_LOCAL find(char shortKey);
        std::vector<Entry>::iterator CORRADE_UTILITY_LOCAL findNextArgument(std::vector<Entry>::iterator start);

        std::string CORRADE_UTILITY_LOCAL keyName(const Entry& entry) const;

        std::string valueInternal(const std::string& key) const;

        bool _isParsed{false};
        std::string _prefix;
        std::string _command;
        std::string _help;
        std::vector<Entry> _entries;
        std::vector<std::string> _values;
        std::vector<std::pair<std::string, std::string>> _skippedPrefixes;
        std::vector<bool> _booleans;
};

template<> inline std::string Arguments::value(const std::string& key, ConfigurationValueFlags) const {
    return valueInternal(key);
}

template<class T> T Arguments::value(const std::string& key, ConfigurationValueFlags flags) const {
    std::string value = valueInternal(key);
    return value.empty() ? T() : ConfigurationValue<T>::fromString(value, flags);
}

}}

#endif
#ifndef Corrade_Utility_String_h
#define Corrade_Utility_String_h

namespace Corrade { namespace Utility {

namespace String {

namespace Implementation {
    CORRADE_UTILITY_EXPORT void ltrimInPlace(std::string& string, Containers::ArrayView<const char> characters);
    CORRADE_UTILITY_EXPORT void rtrimInPlace(std::string& string, Containers::ArrayView<const char> characters);
    CORRADE_UTILITY_EXPORT void trimInPlace(std::string& string, Containers::ArrayView<const char> characters);

    CORRADE_UTILITY_EXPORT std::string ltrim(std::string string, Containers::ArrayView<const char> characters);
    CORRADE_UTILITY_EXPORT std::string rtrim(std::string string, Containers::ArrayView<const char> characters);
    CORRADE_UTILITY_EXPORT std::string trim(std::string string, Containers::ArrayView<const char> characters);

    CORRADE_UTILITY_EXPORT std::vector<std::string> splitWithoutEmptyParts(const std::string& string, Containers::ArrayView<const char> delimiters);

    CORRADE_UTILITY_EXPORT bool beginsWith(Containers::ArrayView<const char> string, Containers::ArrayView<const char> prefix);
    CORRADE_UTILITY_EXPORT bool endsWith(Containers::ArrayView<const char> string, Containers::ArrayView<const char> suffix);

    CORRADE_UTILITY_EXPORT std::string stripPrefix(std::string string, Containers::ArrayView<const char> suffix);
    CORRADE_UTILITY_EXPORT std::string stripSuffix(std::string string, Containers::ArrayView<const char> suffix);

    CORRADE_UTILITY_EXPORT std::string replaceFirst(std::string string, Containers::ArrayView<const char> search, Containers::ArrayView<const char> replace);
    CORRADE_UTILITY_EXPORT std::string replaceAll(std::string string, Containers::ArrayView<const char> search, Containers::ArrayView<const char> replace);
}

inline std::string fromArray(const char* string) {
    return string ? std::string{string} : std::string{};
}

inline std::string fromArray(const char* string, std::size_t length) {
    return string ? std::string{string, length} : std::string{};
}

inline std::string ltrim(std::string string, const std::string& characters) {
    return Implementation::ltrim(std::move(string), {characters.data(), characters.size()});
}

template<std::size_t size> inline std::string ltrim(std::string string, const char(&characters)[size]) {
    return Implementation::ltrim(std::move(string), {characters, size - 1});
}

CORRADE_UTILITY_EXPORT std::string ltrim(std::string string);

inline std::string rtrim(std::string string, const std::string& characters) {
    return Implementation::rtrim(std::move(string), {characters.data(), characters.size()});
}

template<std::size_t size> inline std::string rtrim(std::string string, const char(&characters)[size]) {
    return Implementation::rtrim(std::move(string), {characters, size - 1});
}

CORRADE_UTILITY_EXPORT std::string rtrim(std::string string);

inline std::string trim(std::string string, const std::string& characters) {
    return Implementation::trim(std::move(string), {characters.data(), characters.size()});
}

template<std::size_t size> inline std::string trim(std::string string, const char(&characters)[size]) {
    return Implementation::trim(std::move(string), {characters, size - 1});
}

CORRADE_UTILITY_EXPORT std::string trim(std::string string);

inline void ltrimInPlace(std::string& string, const std::string& characters) {
    Implementation::ltrimInPlace(string, {characters.data(), characters.size()});
}

template<std::size_t size> inline void ltrimInPlace(std::string& string, const char(&characters)[size]) {
    Implementation::ltrimInPlace(string, {characters, size - 1});
}

CORRADE_UTILITY_EXPORT void ltrimInPlace(std::string& string);

inline void rtrimInPlace(std::string& string, const std::string& characters) {
    Implementation::rtrimInPlace(string, {characters.data(), characters.size()});
}

template<std::size_t size> inline void rtrimInPlace(std::string& string, const char(&characters)[size]) {
    Implementation::rtrimInPlace(string, {characters, size - 1});
}

CORRADE_UTILITY_EXPORT void rtrimInPlace(std::string& string);

inline void trimInPlace(std::string& string, const std::string& characters) {
    return Implementation::trimInPlace(string, {characters.data(), characters.size()});
}

template<std::size_t size> inline void trimInPlace(std::string& string, const char(&characters)[size]) {
    return Implementation::trimInPlace(string, {characters, size - 1});
}

CORRADE_UTILITY_EXPORT void trimInPlace(std::string& string);

CORRADE_UTILITY_EXPORT std::vector<std::string> split(const std::string& string, char delimiter);

CORRADE_UTILITY_EXPORT std::vector<std::string> splitWithoutEmptyParts(const std::string& string, char delimiter);

inline std::vector<std::string> splitWithoutEmptyParts(const std::string& string, const std::string& delimiters) {
    return Implementation::splitWithoutEmptyParts(string, {delimiters.data(), delimiters.size()});
}

template<std::size_t size> inline std::vector<std::string> splitWithoutEmptyParts(const std::string& string, const char(&delimiters)[size]) {
    return Implementation::splitWithoutEmptyParts(string, {delimiters, size - 1});
}

CORRADE_UTILITY_EXPORT std::vector<std::string> splitWithoutEmptyParts(const std::string& string);

CORRADE_UTILITY_EXPORT std::string join(const std::vector<std::string>& strings, char delimiter);

CORRADE_UTILITY_EXPORT std::string joinWithoutEmptyParts(const std::vector<std::string>& strings, char delimiter);

CORRADE_UTILITY_EXPORT std::string lowercase(std::string string);

CORRADE_UTILITY_EXPORT std::string uppercase(std::string string);

inline bool beginsWith(const std::string& string, const std::string& prefix) {
    return Implementation::beginsWith({string.data(), string.size()}, {prefix.data(), prefix.size()});
}

template<std::size_t size> inline bool beginsWith(const std::string& string, const char(&prefix)[size]) {
    return Implementation::beginsWith({string.data(), string.size()}, {prefix, size - 1});
}

inline bool beginsWith(const std::string& string, char prefix) {
    return !string.empty() && string[0] == prefix;
}

template<std::size_t size> inline bool viewBeginsWith(Containers::ArrayView<const char> string, const char(&prefix)[size]) {
    return Implementation::beginsWith(string, {prefix, size - 1});
}

inline bool viewBeginsWith(Containers::ArrayView<const char> string, char prefix) {
    return !string.empty() && string[0] == prefix;
}

inline bool endsWith(const std::string& string, const std::string& suffix) {
    return Implementation::endsWith({string.data(), string.size()}, {suffix.data(), suffix.size()});
}

template<std::size_t size> inline bool endsWith(const std::string& string, const char(&suffix)[size]) {
    return Implementation::endsWith({string.data(), string.size()}, {suffix, size - 1});
}

inline bool endsWith(const std::string& string, char suffix) {
    return !string.empty() && string[string.size() - 1] == suffix;
}

template<std::size_t size> inline bool viewEndsWith(Containers::ArrayView<const char> string, const char(&suffix)[size]) {
    return Implementation::endsWith(string, {suffix, size - 1});
}

inline bool viewEndsWith(Containers::ArrayView<const char> string, char suffix) {
    return !string.empty() && string[string.size() - 1] == suffix;
}

inline std::string stripPrefix(std::string string, const std::string& prefix) {
    return Implementation::stripPrefix(std::move(string), {prefix.data(), prefix.size()});
}

template<std::size_t size> inline std::string stripPrefix(std::string string, const char(&prefix)[size]) {
    return Implementation::stripPrefix(std::move(string), {prefix, size - 1});
}

inline std::string stripPrefix(std::string string, char prefix) {
    return Implementation::stripPrefix(std::move(string), {&prefix, 1});
}

inline std::string stripSuffix(std::string string, const std::string& suffix) {
    return Implementation::stripSuffix(std::move(string), {suffix.data(), suffix.size()});
}

template<std::size_t size> inline std::string stripSuffix(std::string string, const char(&suffix)[size]) {
    return Implementation::stripSuffix(std::move(string), {suffix, size - 1});
}

inline std::string stripSuffix(std::string string, char suffix) {
    return Implementation::stripSuffix(std::move(string), {&suffix, 1});
}

inline std::string replaceFirst(std::string string, const std::string& search, const std::string& replace) {
    return Implementation::replaceFirst(std::move(string), {search.data(), search.size()}, {replace.data(), replace.size()});
}

template<std::size_t searchSize, std::size_t replaceSize> inline std::string replaceFirst(std::string string, const char(&search)[searchSize], const char(&replace)[replaceSize]) {
    return Implementation::replaceFirst(std::move(string), {search, searchSize - 1}, {replace, replaceSize - 1});
}

template<std::size_t searchSize> inline std::string replaceFirst(std::string string, const char(&search)[searchSize], const std::string& replace) {
    return Implementation::replaceFirst(std::move(string), {search, searchSize - 1}, {replace.data(), replace.size()});
}

template<std::size_t replaceSize> inline std::string replaceFirst(std::string string, const std::string& search, const char(&replace)[replaceSize]) {
    return Implementation::replaceFirst(std::move(string), {search.data(), search.size()}, {replace, replaceSize - 1});
}

inline std::string replaceAll(std::string string, const std::string& search, const std::string& replace) {
    return Implementation::replaceAll(std::move(string), {search.data(), search.size()}, {replace.data(), replace.size()});
}

template<std::size_t searchSize, std::size_t replaceSize> inline std::string replaceAll(std::string string, const char(&search)[searchSize], const char(&replace)[replaceSize]) {
    return Implementation::replaceAll(std::move(string), {search, searchSize - 1}, {replace, replaceSize - 1});
}

template<std::size_t searchSize> inline std::string replaceAll(std::string string, const char(&search)[searchSize], const std::string& replace) {
    return Implementation::replaceAll(std::move(string), {search, searchSize - 1}, {replace.data(), replace.size()});
}

template<std::size_t replaceSize> inline std::string replaceAll(std::string string, const std::string& search, const char(&replace)[replaceSize]) {
    return Implementation::replaceAll(std::move(string), {search.data(), search.size()}, {replace, replaceSize - 1});
}

}}}

#endif
namespace Corrade { namespace TestSuite {

namespace {
    inline int digitCount(int number) {
        int digits = 0;
        while(number != 0) number /= 10, digits++;
        return digits;
    }

    constexpr const char PaddingString[] = "0000000000";
}

int* Tester::_argc = nullptr;
char** Tester::_argv = nullptr;

void Tester::registerArguments(int& argc, char** const argv) {
    _argc = &argc;
    _argv = argv;
}

Tester::Tester(TesterConfiguration configuration): _logOutput{nullptr}, _errorOutput{nullptr}, _testCaseLine{0}, _checkCount{0}, _expectedFailure{nullptr}, _configuration{std::move(configuration)} {
    CORRADE_ASSERT(_argc, "TestSuite::Tester: command-line arguments not available", );
}

Tester::~Tester() {
    _argc = nullptr;
    _argv = nullptr;
}

int Tester::exec() { return exec(&std::cout, &std::cerr); }

int Tester::exec(std::ostream* const logOutput, std::ostream* const errorOutput) {
    Utility::Arguments args;
    for(auto&& prefix: _configuration.skippedArgumentPrefixes())
        args.addSkippedPrefix(prefix);
    args.addOption('c', "color", "auto").setHelp("color", "colored output", "on|off|auto")
            .setFromEnvironment("color", "CORRADE_TEST_COLOR")
        .addOption("skip").setHelp("skip", "skip test cases with given numbers", "\"N1 N2...\"")
        .addBooleanOption("skip-tests").setHelp("skip-tests", "skip all tests")
            .setFromEnvironment("skip-tests", "CORRADE_TEST_SKIP_TESTS")
        .addBooleanOption("skip-benchmarks").setHelp("skip-benchmarks", "skip all benchmarks")
            .setFromEnvironment("skip-benchmarks", "CORRADE_TEST_SKIP_BENCHMARKS")
        .addOption("only").setHelp("only", "run only test cases with given numbers", "\"N1 N2...\"")
        .addBooleanOption("shuffle").setHelp("shuffle", "randomly shuffle test case order")
            .setFromEnvironment("shuffle", "CORRADE_TEST_SHUFFLE")
        .addOption("repeat-every", "1").setHelp("repeat-every", "repeat every test case N times", "N")
            .setFromEnvironment("repeat-every", "CORRADE_TEST_REPEAT_EVERY")
        .addOption("repeat-all", "1").setHelp("repeat-all", "repeat all test cases N times", "N")
            .setFromEnvironment("repeat-all", "CORRADE_TEST_REPEAT_ALL")
        .addBooleanOption("abort-on-fail").setHelp("abort-on-fail", "abort after first failure")
            .setFromEnvironment("abort-on-fail", "CORRADE_TEST_ABORT_ON_FAIL")
        .addBooleanOption("no-xfail").setHelp("no-xfail", "disallow expected failures")
            .setFromEnvironment("no-xfail", "CORRADE_TEST_NO_XFAIL")
        .addOption("benchmark", "wall-time").setHelp("benchmark", "default benchmark type", "TYPE")
            .setFromEnvironment("benchmark", "CORRADE_BENCHMARK")
        .addOption("benchmark-discard", "1").setHelp("benchmark-discard", "discard first N measurements of each benchmark", "N")
            .setFromEnvironment("benchmark-discard", "CORRADE_BENCHMARK_DISCARD")
        .addOption("benchmark-yellow", "0.05").setHelp("benchmark-yellow", "deviation threshold for marking benchmark yellow", "N")
            .setFromEnvironment("benchmark-yellow", "CORRADE_BENCHMARK_YELLOW")
        .addOption("benchmark-red", "0.25").setHelp("benchmark-red", "deviation threshold for marking benchmark red", "N")
            .setFromEnvironment("benchmark-red", "CORRADE_BENCHMARK_RED")
        .setGlobalHelp(R"(Corrade TestSuite executable. By default runs test cases in order in which they
were added and exits with non-zero code if any of them failed. Supported
benchmark types:
  wall-time     wall time spent
  cpu-time      CPU time spent
  cpu-cycles    CPU cycles spent (x86 only, gives zero result elsewhere))")
        .parse(*_argc, _argv);

    _logOutput = logOutput;
    _errorOutput = errorOutput;

    if(args.value("color") == "on" || args.value("color") == "ON")
        _useColor = Debug::Flags{};
    else if(args.value("color") == "off" || args.value("color") == "OFF")
        _useColor = Debug::Flag::DisableColors;
    else _useColor = Debug::isTty(logOutput) && Debug::isTty(errorOutput) ?
            Debug::Flags{} : Debug::Flag::DisableColors;

    TestCaseType defaultBenchmarkType{};
    if(args.value("benchmark") == "wall-time")
        defaultBenchmarkType = TestCaseType::WallTimeBenchmark;
    else if(args.value("benchmark") == "cpu-time")
        defaultBenchmarkType = TestCaseType::CpuTimeBenchmark;
    else if(args.value("benchmark") == "cpu-cycles")
        defaultBenchmarkType = TestCaseType::CpuCyclesBenchmark;
    else Utility::Fatal{} << "Unknown benchmark type" << args.value("benchmark")
        << Utility::Debug::nospace << ", use one of wall-time, cpu-time or cpu-cycles";

    std::vector<std::pair<int, TestCase>> usedTestCases;

    _expectedFailuresDisabled = args.isSet("no-xfail");

    if(args.isSet("skip-tests"))
        for(TestCase& testCase: _testCases)
            if(testCase.type == TestCaseType::Test) testCase.test = nullptr;

    if(args.isSet("skip-benchmarks"))
        for(TestCase& testCase: _testCases)
            if(testCase.type != TestCaseType::Test) testCase.test = nullptr;

    if(!args.value("skip").empty()) {
        const std::vector<std::string> skip = Utility::String::split(args.value("skip"), ' ');
        for(auto&& n: skip) {
            #ifndef CORRADE_TARGET_ANDROID
            const std::size_t index = std::stoi(n);
            #else
            const std::size_t index = std::strtoul(n.data(), nullptr, 10);
            #endif
            if(index - 1 >= _testCases.size()) continue;
            _testCases[index - 1].test = nullptr;
        }
    }

    if(!args.value("only").empty()) {
        const std::vector<std::string> only = Utility::String::split(args.value("only"), ' ');
        for(auto&& n: only) {
            #ifndef CORRADE_TARGET_ANDROID
            const std::size_t index = std::stoi(n);
            #else
            const std::size_t index = std::strtoul(n.data(), nullptr, 10);
            #endif
            if(index - 1 >= _testCases.size() || !_testCases[index - 1].test) continue;
            usedTestCases.emplace_back(index, _testCases[index - 1]);
        }

    } else for(std::size_t i = 0; i != _testCases.size(); ++i) {
        if(!_testCases[i].test) continue;
        usedTestCases.emplace_back(i + 1, _testCases[i]);
    }

    const std::size_t repeatAllCount = args.value<std::size_t>("repeat-all");
    const std::size_t repeatEveryCount = args.value<std::size_t>("repeat-every");
    if(!repeatAllCount || !repeatEveryCount)
        Utility::Fatal() << "You have to repeat at least once";

    const std::size_t originalTestCaseCount = usedTestCases.size();
    usedTestCases.reserve(usedTestCases.size()*repeatAllCount);
    for(std::size_t i = 0; i != repeatAllCount - 1; ++i)
        usedTestCases.insert(usedTestCases.end(), usedTestCases.begin(), usedTestCases.begin() + originalTestCaseCount);

    if(args.isSet("shuffle"))
        std::shuffle(usedTestCases.begin(), usedTestCases.end(), std::minstd_rand{std::random_device{}()});

    unsigned int errorCount = 0,
        noCheckCount = 0;

    if(usedTestCases.empty()) {
        if(args.isSet("skip-tests") && !args.isSet("skip-benchmarks")) {
            Debug(logOutput, _useColor)
                << Debug::boldColor(Debug::Color::Default) << "No remaining benchmarks to run in"
                << _testName << Debug::nospace << ".";
            return 0;
        }

        if(!args.isSet("skip-tests") && args.isSet("skip-benchmarks")) {
            Debug(logOutput, _useColor)
                << Debug::boldColor(Debug::Color::Default) << "No remaining tests to run in"
                << _testName << Debug::nospace << ".";
            return 0;
        }

        Error(errorOutput, _useColor) << Debug::boldColor(Debug::Color::Red) << "No test cases to run in" << _testName << Debug::nospace << "!";
        return 2;
    }

    Debug(logOutput, _useColor) << Debug::boldColor(Debug::Color::Default) << "Starting" << _testName << "with" << usedTestCases.size() << "test cases...";

    for(std::pair<int, TestCase> testCase: usedTestCases) {
        Debug resetDebugRedirect{&std::cout};
        Error resetErrorRedirect{&std::cerr};
        Utility::Warning resetWarningRedirect{&std::cerr};

        if(testCase.second.type == TestCaseType::DefaultBenchmark)
            testCase.second.type = defaultBenchmarkType;

        BenchmarkUnits benchmarkUnits = BenchmarkUnits::Count;
        switch(testCase.second.type) {
            case TestCaseType::DefaultBenchmark:
                CORRADE_ASSERT_UNREACHABLE();

            case TestCaseType::Test:
                break;

            case TestCaseType::WallTimeBenchmark:
                testCase.second.benchmarkBegin = &Tester::wallTimeBenchmarkBegin;
                testCase.second.benchmarkEnd = &Tester::wallTimeBenchmarkEnd;
                benchmarkUnits = BenchmarkUnits::Nanoseconds;
                break;

            case TestCaseType::CpuTimeBenchmark:
                testCase.second.benchmarkBegin = &Tester::cpuTimeBenchmarkBegin;
                testCase.second.benchmarkEnd = &Tester::cpuTimeBenchmarkEnd;
                benchmarkUnits = BenchmarkUnits::Nanoseconds;
                break;

            case TestCaseType::CpuCyclesBenchmark:
                testCase.second.benchmarkBegin = &Tester::cpuCyclesBenchmarkBegin;
                testCase.second.benchmarkEnd = &Tester::cpuCyclesBenchmarkEnd;
                benchmarkUnits = BenchmarkUnits::Cycles;
                break;

            case TestCaseType::CustomTimeBenchmark:
            case TestCaseType::CustomCycleBenchmark:
            case TestCaseType::CustomInstructionBenchmark:
            case TestCaseType::CustomMemoryBenchmark:
            case TestCaseType::CustomCountBenchmark:
                benchmarkUnits = BenchmarkUnits(int(testCase.second.type));
                _benchmarkName = "";
                break;
        }

        _testCaseId = testCase.first;
        _testCaseInstanceId = testCase.second.instanceId;
        if(testCase.second.instanceId == ~std::size_t{})
            _testCaseDescription = {};
        else {
            #ifndef CORRADE_TARGET_ANDROID
            _testCaseDescription = std::to_string(testCase.second.instanceId);
            #else
            std::ostringstream out;
            out << testCase.second.instanceId;
            _testCaseDescription = out.str();
            #endif
        }

        const std::size_t repeatCount = testCase.second.repeatCount*repeatEveryCount;

        Containers::Array<std::uint64_t> measurements{testCase.second.type != TestCaseType::Test ? repeatCount : 0};

        bool aborted = false, skipped = false;
        for(std::size_t i = 0; i != repeatCount && !aborted; ++i) {
            if(testCase.second.setup)
                (this->*testCase.second.setup)();

            _testCaseRepeatId = repeatCount == 1 ? ~std::size_t{} : i;
            _testCaseLine = 0;
            _testCaseName.clear();
            _testCase = &testCase.second;
            _benchmarkBatchSize = 0;
            _benchmarkResult = 0;

            try {
                (this->*testCase.second.test)();
            } catch(const Exception&) {
                ++errorCount;
                aborted = true;
            } catch(const SkipException&) {
                aborted = true;
                skipped = true;
            }

            _testCase = nullptr;

            if(testCase.second.teardown)
                (this->*testCase.second.teardown)();

            if(testCase.second.benchmarkEnd)
                measurements[i] = _benchmarkResult;
        }

        if(!aborted) {
            if(!_testCaseLine) {
                Debug out{logOutput, _useColor};
                printTestCaseLabel(out, "     ?", Debug::Color::Yellow, Debug::Color::Yellow);
                ++noCheckCount;

            } else if(testCase.second.type == TestCaseType::Test || _expectedFailure) {
                Debug out{logOutput, _useColor};
                printTestCaseLabel(out,
                    _expectedFailure ? " XFAIL" : "    OK",
                    _expectedFailure ? Debug::Color::Yellow : Debug::Color::Default,
                    Debug::Color::Default);
                if(_expectedFailure) out << Debug::newline << "       " << _expectedFailure->message();

            } else {
                Debug out{logOutput, _useColor};

                const char* padding = PaddingString + sizeof(PaddingString) - digitCount(_testCases.size()) + digitCount(_testCaseId) - 1;

                out << Debug::boldColor(Debug::Color::Default) << " BENCH"
                    << Debug::color(Debug::Color::Blue) << "[" << Debug::nospace
                    << Debug::boldColor(Debug::Color::Cyan) << padding
                    << Debug::nospace << _testCaseId << Debug::nospace
                    << Debug::color(Debug::Color::Blue) << "]";

                const std::size_t discardMeasurements = measurements.empty() ? 0 :
                        std::min(measurements.size() - 1, args.value<std::size_t>("benchmark-discard"));

                double mean, stddev;
                Utility::Debug::Color color;
                std::tie(mean, stddev, color) = Implementation::calculateStats(measurements.suffix(discardMeasurements), _benchmarkBatchSize, args.value<double>("benchmark-yellow"), args.value<double>("benchmark-red"));

                Implementation::printStats(out, mean, stddev, color, benchmarkUnits);

                out << Debug::boldColor(Debug::Color::Default)
                    << (_testCaseName.empty() ? "<unknown>" : _testCaseName)
                    << Debug::nospace;

                if(!_testCaseDescription.empty()) {
                    out << "("
                        << Debug::nospace
                        << Debug::resetColor << _testCaseDescription
                        << Debug::nospace << Debug::boldColor(Debug::Color::Default)
                        << ")";
                } else out << "()";

                out << Debug::nospace << "@" << Debug::nospace
                    << measurements.size() - discardMeasurements
                    << Debug::nospace << "x" << Debug::nospace << _benchmarkBatchSize
                    << Debug::resetColor;
                if(!_benchmarkName.empty())
                    out << "(" << Utility::Debug::nospace << _benchmarkName
                        << Utility::Debug::nospace << ")";
            }

        } else if(args.isSet("abort-on-fail") && !skipped) {
            Debug out{logOutput, _useColor};
            out << Debug::boldColor(Debug::Color::Red) << "Aborted"
                << Debug::boldColor(Debug::Color::Default) << _testName
                << Debug::boldColor(Debug::Color::Red) << "after first failure"
                << Debug::boldColor(Debug::Color::Default) << "out of"
                << _checkCount << "checks so far.";
            if(noCheckCount)
                out << Debug::boldColor(Debug::Color::Yellow) << noCheckCount << "test cases didn't contain any checks!";

            return 1;
        }
    }

    Debug d(logOutput, _useColor);
    d << Debug::boldColor(Debug::Color::Default) << "Finished" << _testName << "with";
    if(errorCount) d << Debug::boldColor(Debug::Color::Red);
    d << errorCount << "errors";
    if(errorCount) d << Debug::boldColor(Debug::Color::Default);
    d << "out of" << _checkCount << "checks.";
    if(noCheckCount)
        d << Debug::boldColor(Debug::Color::Yellow) << noCheckCount << "test cases didn't contain any checks!";

    return errorCount != 0 || noCheckCount != 0;
}

void Tester::printTestCaseLabel(Debug& out, const char* const status, const Debug::Color statusColor, const Debug::Color labelColor) {
    const char* padding = PaddingString + sizeof(PaddingString) - digitCount(_testCases.size()) + digitCount(_testCaseId) - 1;

    out << Debug::boldColor(statusColor) << status
        << Debug::color(Debug::Color::Blue) << "[" << Debug::nospace
        << Debug::boldColor(Debug::Color::Cyan) << padding
        << Debug::nospace << _testCaseId << Debug::nospace
        << Debug::color(Debug::Color::Blue) << "]"
        << Debug::boldColor(labelColor)
        << (_testCaseName.empty() ? "<unknown>" : _testCaseName)
        << Debug::nospace;

    if(!_testCaseDescription.empty()) {
        out << "("
            << Debug::nospace
            << Debug::resetColor << _testCaseDescription
            << Debug::nospace << Debug::boldColor(labelColor)
            << ")";
    } else out << "()";

    if(_testCaseRepeatId != ~std::size_t{})
        out << Debug::nospace << "@" << Debug::nospace << _testCaseRepeatId + 1;

    out << Debug::resetColor;
}

void Tester::verifyInternal(const char* expression, bool expressionValue) {
    ++_checkCount;

    if(!_expectedFailure) {
        if(expressionValue) return;
    } else if(!expressionValue) {
        Debug out{_logOutput, _useColor};
        printTestCaseLabel(out, " XFAIL", Debug::Color::Yellow, Debug::Color::Default);
        out << "at" << _testFilename << "on line" << _testCaseLine
            << Debug::newline << "       " << _expectedFailure->message()
            << "Expression" << expression << "failed.";
        return;
    }

    Error out{_errorOutput, _useColor};
    printTestCaseLabel(out, _expectedFailure ? " XPASS" : "  FAIL", Debug::Color::Red, Debug::Color::Default);
    out << "at" << _testFilename << "on line" << _testCaseLine
        << Debug::newline << "        Expression" << expression;
    if(!_expectedFailure) out << "failed.";
    else out << "was expected to fail.";
    throw Exception();
}

void Tester::registerTest(const char* filename, const char* name) {
    _testFilename = std::move(filename);
    if(_testName.empty()) _testName = std::move(name);
}

void Tester::skip(const std::string& message) {
    Debug out{_logOutput, _useColor};
    printTestCaseLabel(out, "  SKIP", Debug::Color::Default, Debug::Color::Default);
    out << Debug::newline << "       " << message;
    throw SkipException();
}

void Tester::setTestName(const std::string& name) {
    _testName = name;
}

void Tester::setTestName(std::string&& name) {
    _testName = std::move(name);
}

void Tester::setTestCaseName(const std::string& name) {
    _testCaseName = name;
}

void Tester::setTestCaseName(std::string&& name) {
    _testCaseName = std::move(name);
}

void Tester::setTestCaseDescription(const std::string& description) {
    _testCaseDescription = description;
}

void Tester::setTestCaseDescription(std::string&& description) {
    _testCaseDescription = std::move(description);
}

void Tester::setBenchmarkName(const std::string& name) {
    _benchmarkName = name;
}

void Tester::setBenchmarkName(std::string&& name) {
    _benchmarkName = std::move(name);
}

void Tester::registerTestCase(const char* name, int line) {
    CORRADE_ASSERT(_testCase,
        "TestSuite::Tester: using verification macros outside of test cases is not allowed", );

    if(_testCaseName.empty()) _testCaseName = std::move(name);
    _testCaseLine = line;
}

Tester::BenchmarkRunner Tester::createBenchmarkRunner(const std::size_t batchSize) {
    CORRADE_ASSERT(_testCase,
        "TestSuite::Tester: using benchmark macros outside of test cases is not allowed",
        (BenchmarkRunner{*this, nullptr, nullptr}));

    _benchmarkBatchSize = batchSize;
    return BenchmarkRunner{*this, _testCase->benchmarkBegin, _testCase->benchmarkEnd};
}

void Tester::wallTimeBenchmarkBegin() {
    _benchmarkName = "wall time";
    _benchmarkBegin = Implementation::wallTime();
}

std::uint64_t Tester::wallTimeBenchmarkEnd() {
    return Implementation::wallTime() - _benchmarkBegin;
}

void Tester::cpuTimeBenchmarkBegin() {
    _benchmarkName = "CPU time";
    _benchmarkBegin = Implementation::cpuTime();
}

std::uint64_t Tester::cpuTimeBenchmarkEnd() {
    return Implementation::cpuTime() - _benchmarkBegin;
}

void Tester::cpuCyclesBenchmarkBegin() {
    _benchmarkName = "CPU cycles";
    _benchmarkBegin = Implementation::rdtsc();
}

std::uint64_t Tester::cpuCyclesBenchmarkEnd() {
    return Implementation::rdtsc() - _benchmarkBegin;
}

Tester::TesterConfiguration::TesterConfiguration() = default;

Tester::ExpectedFailure::ExpectedFailure(Tester& instance, std::string message, const bool enabled): _instance(instance), _message(std::move(message)) {
    if(enabled && !instance._expectedFailuresDisabled) _instance._expectedFailure = this;
}

Tester::ExpectedFailure::~ExpectedFailure() {
    _instance._expectedFailure = nullptr;
}

std::string Tester::ExpectedFailure::message() const { return _message; }

}}
#ifndef Corrade_Utility_Directory_h
#define Corrade_Utility_Directory_h

namespace Corrade { namespace Utility {

namespace Directory {

#ifdef CORRADE_TARGET_UNIX
class CORRADE_UTILITY_EXPORT MapDeleter;
#endif

enum class Flag: unsigned char {
    SkipDotAndDotDot = 1 << 0,

    SkipFiles = 1 << 1,

    SkipDirectories = 1 << 2,

    SkipSpecial = 1 << 3,

    SortAscending = (1 << 4) | (1 << 5),

    SortDescending = 1 << 5
};

typedef Containers::EnumSet<Flag> Flags;

CORRADE_ENUMSET_OPERATORS(Flags)

CORRADE_UTILITY_EXPORT std::string fromNativeSeparators(std::string path);

CORRADE_UTILITY_EXPORT std::string toNativeSeparators(std::string path);

CORRADE_UTILITY_EXPORT std::string path(const std::string& filename);

CORRADE_UTILITY_EXPORT std::string filename(const std::string& filename);

CORRADE_UTILITY_EXPORT std::string join(const std::string& path, const std::string& filename);

CORRADE_UTILITY_EXPORT std::vector<std::string> list(const std::string& path, Flags flags = Flags());

CORRADE_UTILITY_EXPORT bool mkpath(const std::string& path);

CORRADE_UTILITY_EXPORT bool rm(const std::string& path);

CORRADE_UTILITY_EXPORT bool move(const std::string& oldPath, const std::string& newPath);

CORRADE_UTILITY_EXPORT bool isSandboxed();

CORRADE_UTILITY_EXPORT std::string executableLocation();

CORRADE_UTILITY_EXPORT std::string home();

CORRADE_UTILITY_EXPORT std::string configurationDir(const std::string& name);

CORRADE_UTILITY_EXPORT std::string tmp();

CORRADE_UTILITY_EXPORT bool exists(const std::string& filename);

CORRADE_UTILITY_EXPORT Containers::Array<char> read(const std::string& filename);

CORRADE_UTILITY_EXPORT std::string readString(const std::string& filename);

CORRADE_UTILITY_EXPORT bool write(const std::string& filename, Containers::ArrayView<const void> data);

CORRADE_UTILITY_EXPORT bool writeString(const std::string& filename, const std::string& data);

CORRADE_UTILITY_EXPORT bool append(const std::string& filename, Containers::ArrayView<const void> data);

CORRADE_UTILITY_EXPORT bool appendString(const std::string& filename, const std::string& data);

CORRADE_UTILITY_EXPORT bool copy(const std::string& from, const std::string& to);

#ifdef CORRADE_TARGET_UNIX
CORRADE_UTILITY_EXPORT Containers::Array<char, MapDeleter> map(const std::string& filename, std::size_t size);

CORRADE_UTILITY_EXPORT Containers::Array<const char, MapDeleter> mapRead(const std::string& filename);
#endif

#ifdef CORRADE_TARGET_UNIX
class CORRADE_UTILITY_EXPORT MapDeleter {
    public:
        constexpr explicit MapDeleter(): _fd{} {}
        constexpr explicit MapDeleter(int fd) noexcept: _fd{fd} {}
        void operator()(const char* data, std::size_t size);
    private:
        int _fd;
};
#else
#endif

}}}

#endif
namespace Corrade { namespace TestSuite {

Comparator<Compare::File>::Comparator(std::string pathPrefix): _actualState{State::ReadError}, _expectedState{State::ReadError}, _pathPrefix{std::move(pathPrefix)} {}

bool Comparator<Compare::File>::operator()(const std::string& actualFilename, const std::string& expectedFilename) {
    _actualFilename = Utility::Directory::join(_pathPrefix, actualFilename);
    _expectedFilename = Utility::Directory::join(_pathPrefix, expectedFilename);

    if(!Utility::Directory::exists(_actualFilename))
        return false;

    _actualState = State::Success;

    if(!Utility::Directory::exists(_expectedFilename))
        return false;

    _actualContents = Utility::Directory::readString(_actualFilename);
    _expectedContents = Utility::Directory::readString(_expectedFilename);
    _expectedState = State::Success;

    return _actualContents == _expectedContents;
}

void Comparator<Compare::File>::printErrorMessage(Utility::Error& e, const char* actual, const char* expected) const {
    if(_actualState != State::Success) {
        e << "Actual file" << actual << "(" + _actualFilename + ")" << "cannot be read.";
        return;
    }

    if(_expectedState != State::Success) {
        e << "Expected file" << expected << "(" + _expectedFilename + ")" << "cannot be read.";
        return;
    }

    e << "Files" << actual << "and" << expected << "have different";
    if(_actualContents.size() != _expectedContents.size())
        e << "size, actual" << _actualContents.size() << "but" << _expectedContents.size() << "expected.";
    else
        e << "contents.";

    for(std::size_t i = 0, end = std::max(_actualContents.size(), _expectedContents.size()); i != end; ++i) {
        if(_actualContents.size() > i && _expectedContents.size() > i && _actualContents[i] == _expectedContents[i]) continue;

        if(_actualContents.size() <= i)
            e << "Expected has character" << std::string() + _expectedContents[i];
        else if(_expectedContents.size() <= i)
            e << "Actual has character" << std::string() + _actualContents[i];
        else
            e << "Actual character" << std::string() + _actualContents[i] << "but" << std::string() + _expectedContents[i] << "expected";

        e << "on position" << i << Utility::Debug::nospace << ".";
        break;
    }
}

namespace Compare {

File::File(const std::string& pathPrefix): _c{pathPrefix} {}

Comparator<File> File::comparator() { return _c; }

}

}}
namespace Corrade { namespace TestSuite {

Comparator<Compare::FileToString>::Comparator(): _state(State::ReadError) {}

bool Comparator<Compare::FileToString>::operator()(const std::string& filename, const std::string& expectedContents) {
    _filename = filename;

    if(!Utility::Directory::exists(filename)) return false;

    _actualContents = Utility::Directory::readString(filename);
    _expectedContents = expectedContents;
    _state = State::Success;

    return _actualContents == expectedContents;
}

void Comparator<Compare::FileToString>::printErrorMessage(Utility::Error& e, const char* actual, const char* expected) const {
    if(_state != State::Success) {
        e << "File" << actual << "(" + _filename + ")" << "cannot be read.";
        return;
    }

    e << "Files" << actual << "and" << expected << "have different";
    if(_actualContents.size() != _expectedContents.size())
        e << "size, actual" << _actualContents.size() << "but" << _expectedContents.size() << "expected.";
    else
        e << "contents.";

    for(std::size_t i = 0, end = std::max(_actualContents.size(), _expectedContents.size()); i != end; ++i) {
        if(_actualContents.size() > i && _expectedContents.size() > i && _actualContents[i] == _expectedContents[i]) continue;

        if(_actualContents.size() <= i)
            e << "Expected has character" << std::string() + _expectedContents[i];
        else if(_expectedContents.size() <= i)
            e << "Actual has character" << std::string() + _actualContents[i];
        else
            e << "Actual character" << std::string() + _actualContents[i] << "but" << std::string() + _expectedContents[i] << "expected";

        e << "on position" << i << Utility::Debug::nospace << ".";
        break;
    }

}

}}
namespace Corrade { namespace TestSuite { namespace Implementation {

template<class T> bool FloatComparator<T>::operator()(T actual, T expected) {
    if(actual == expected || (actual != actual && expected != expected)) return true;

    const T absA = std::abs(actual);
    const T absB = std::abs(expected);
    const T difference = std::abs(actual - expected);

    if((actual == T{} || expected == T{} || difference < FloatComparatorEpsilon<T>::epsilon())) {
        if(difference < FloatComparatorEpsilon<T>::epsilon()) return true;
    }

    else if(difference/(absA + absB) < FloatComparatorEpsilon<T>::epsilon())
        return true;

    _actualValue = actual;
    _expectedValue = expected;
    return false;
}

template<class T> void FloatComparator<T>::printErrorMessage(Utility::Error& e, const char* actual, const char* expected) const {
    e << "Floating-point values" << actual << "and" << expected << "are not the same, actual" << _actualValue << "but" << _expectedValue << "expected (delta" << _actualValue - _expectedValue << Utility::Debug::nospace << ").";
}

template class FloatComparator<float>;
template class FloatComparator<double>;
template class FloatComparator<long double>;

}}}
namespace Corrade { namespace TestSuite {

Comparator<Compare::StringToFile>::Comparator(): _state(State::ReadError) {}

bool Comparator<Compare::StringToFile>::operator()(const std::string& actualContents, const std::string& filename) {
    _filename = filename;

    if(!Utility::Directory::exists(filename)) return false;

    _expectedContents = Utility::Directory::readString(filename);
    _actualContents = actualContents;
    _state = State::Success;

    return actualContents == _expectedContents;
}

void Comparator<Compare::StringToFile>::printErrorMessage(Utility::Error& e, const char* actual, const char* expected) const {
    if(_state != State::Success) {
        e << "File" << expected << "(" + _filename + ")" << "cannot be read.";
        return;
    }

    e << "Files" << actual << "and" << expected << "have different";
    if(_actualContents.size() != _expectedContents.size())
        e << "size, actual" << _actualContents.size() << "but" << _expectedContents.size() << "expected.";
    else
        e << "contents.";

    for(std::size_t i = 0, end = std::max(_actualContents.size(), _expectedContents.size()); i != end; ++i) {
        if(_actualContents.size() > i && _expectedContents.size() > i && _actualContents[i] == _expectedContents[i]) continue;

        if(_actualContents.size() <= i)
            e << "Expected has character" << std::string() + _expectedContents[i];
        else if(_expectedContents.size() <= i)
            e << "Actual has character" << std::string() + _actualContents[i];
        else
            e << "Actual character" << std::string() + _actualContents[i] << "but" << std::string() + _expectedContents[i] << "expected";

        e << "on position" << i << Utility::Debug::nospace << ".";
        break;
    }

}

}}
#ifdef CORRADE_TARGET_UNIX
extern char **environ;
#else
#endif

namespace Corrade { namespace Utility {

namespace {
    enum class Type: std::uint8_t {
        Argument,
        NamedArgument,
        Option,
        BooleanOption
    };

    inline std::string uppercaseKey(std::string key) {
        for(char& i: key) {
            if(i >= 'a' && i <= 'z')
                i = 'A' + i - 'a';
            else if(i == '-')
                i = '_';
        }

        return key;
    }
}

struct Arguments::Entry {
    Entry(Type type, char shortKey, std::string key, std::string helpKey, std::string defaultValue, std::size_t id);

    Type type;
    char shortKey;
    std::string key, help, helpKey, defaultValue;
    #ifndef CORRADE_TARGET_WINDOWS_RT
    std::string environment;
    #endif
    std::size_t id;
};

Arguments::Entry::Entry(Type type, char shortKey, std::string key, std::string helpKey, std::string defaultValue, std::size_t id): type(type), shortKey(shortKey), key(std::move(key)), defaultValue(std::move(defaultValue)), id(id) {
    if(type == Type::NamedArgument || type == Type::Option)
        this->helpKey = this->key + ' ' + uppercaseKey(helpKey);
    else this->helpKey = std::move(helpKey);

    CORRADE_INTERNAL_ASSERT(type == Type::Option || this->defaultValue.empty());
}

std::vector<std::string> Arguments::environment() {
    std::vector<std::string> list;

    #ifdef CORRADE_TARGET_UNIX
    for(char** e = environ; *e; ++e)
        list.emplace_back(*e);

    #else
    #endif

    return list;
}

Arguments::Arguments(const std::string& prefix): _prefix{prefix + '-'} {
    addBooleanOption("help");
    setHelp("help", "display this help message and exit");
}

Arguments::Arguments() {
    addBooleanOption('h', "help");
    setHelp("help", "display this help message and exit");
}

Arguments::Arguments(Arguments&& other) noexcept: _isParsed{std::move(other._isParsed)}, _prefix{std::move(other._prefix)}, _command{std::move(other._command)}, _help{std::move(other._help)}, _entries{std::move(other._entries)}, _values{std::move(other._values)}, _skippedPrefixes{std::move(other._skippedPrefixes)}, _booleans{std::move(other._booleans)} {
    other._isParsed = false;
}

Arguments& Arguments::operator=(Arguments&& other) noexcept {
    std::swap(other._prefix, _prefix);
    std::swap(other._command, _command);
    std::swap(other._help, _help);
    std::swap(other._entries, _entries);
    std::swap(other._values, _values);
    std::swap(other._skippedPrefixes, _skippedPrefixes);
    std::swap(other._booleans, _booleans);
    std::swap(other._isParsed, _isParsed);

    return *this;
}

Arguments::~Arguments() = default;

std::string Arguments::prefix() const {
    return _prefix.empty() ? std::string{} : _prefix.substr(0, _prefix.size() - 1);
}

Arguments& Arguments::addArgument(std::string key) {
    CORRADE_ASSERT(_prefix.empty(),
        "Utility::Arguments::addArgument(): argument" << key << "not allowed in prefixed version", *this);

    CORRADE_ASSERT(!key.empty(), "Utility::Arguments::addArgument(): key must not be empty", *this);

    CORRADE_ASSERT(find(key) == _entries.end(), "Utility::Arguments::addArgument(): the key" << key << "is already used", *this);

    _isParsed = false;

    std::string helpKey = key;
    _entries.emplace_back(Type::Argument, '\0', std::move(key), std::move(helpKey), std::string(), _values.size());
    _values.emplace_back();
    return *this;
}

Arguments& Arguments::addNamedArgument(char shortKey, std::string key) {
    CORRADE_ASSERT(verifyKey(shortKey) && verifyKey(key),
        "Utility::Arguments::addNamedArgument(): invalid key" << key << "or its short variant", *this);

    CORRADE_ASSERT((!shortKey || find(shortKey) == _entries.end()) && find(_prefix + key) == _entries.end(),
        "Utility::Arguments::addNamedArgument(): the key" << key << "or its short version is already used", *this);

    CORRADE_ASSERT(_prefix.empty(),
        "Utility::Arguments::addNamedArgument(): argument" << key << "not allowed in prefixed version", *this);

    _isParsed = false;

    std::string helpKey = key;
    _entries.emplace_back(Type::NamedArgument, shortKey, std::move(key), std::move(helpKey), std::string(), _values.size());
    _values.emplace_back();
    return *this;
}

Arguments& Arguments::addOption(char shortKey, std::string key, std::string defaultValue) {
    CORRADE_ASSERT(verifyKey(shortKey) && verifyKey(key),
        "Utility::Arguments::addOption(): invalid key" << key << "or its short variant", *this);

    CORRADE_ASSERT((!shortKey || find(shortKey) == _entries.end()) && find(_prefix + key) == _entries.end(),
        "Utility::Arguments::addOption(): the key" << key << "or its short version is already used", *this);

    CORRADE_ASSERT(_prefix.empty() || shortKey == '\0',
        "Utility::Arguments::addOption(): short option" << std::string{shortKey} << "not allowed in prefixed version", *this);
    CORRADE_ASSERT(!skippedPrefix(key),
        "Utility::Arguments::addOption(): key" << key << "conflicts with skipped prefixes", *this);

    _isParsed = false;

    std::string helpKey;
    if(_prefix.empty())
        helpKey = key;
    else {
        std::string tmp = std::move(key);
        key = _prefix + tmp;
        helpKey = std::move(tmp);
    }
    _entries.emplace_back(Type::Option, shortKey, std::move(key), std::move(helpKey), std::move(defaultValue), _values.size());
    _values.emplace_back();
    return *this;
}

Arguments& Arguments::addBooleanOption(char shortKey, std::string key) {
    CORRADE_ASSERT(verifyKey(shortKey) && verifyKey(key),
        "Utility::Arguments::addBooleanOption(): invalid key" << key << "or its short variant", *this);

    CORRADE_ASSERT((!shortKey || find(shortKey) == _entries.end()) && find(key) == _entries.end(),
        "Utility::Arguments::addBooleanOption(): the key" << key << "or its short version is already used", *this);

    CORRADE_ASSERT(_prefix.empty() || key == "help",
        "Utility::Arguments::addBooleanOption(): boolean option" << key << "not allowed in prefixed version", *this);
    CORRADE_ASSERT(!skippedPrefix(key),
        "Utility::Arguments::addBooleanOption(): key" << key << "conflicts with skipped prefixes", *this);

    _isParsed = false;

    std::string helpKey;
    if(_prefix.empty())
        helpKey = key;
    else
        helpKey = key = _prefix + std::move(key);
    _entries.emplace_back(Type::BooleanOption, shortKey, std::move(key), std::move(helpKey), std::string(), _booleans.size());
    _booleans.push_back(false);
    return *this;
}

namespace {
    inline bool keyHasPrefix(const std::string& key, const std::string& prefix) {
        if(key.size() < prefix.size()) return false;
        return std::equal(prefix.begin(), prefix.end(), key.begin());
    }
}

Arguments& Arguments::addSkippedPrefix(std::string prefix, std::string help) {
    CORRADE_ASSERT(!skippedPrefix(prefix),
        "Utility::Arguments::addSkippedPrefix(): prefix" << prefix << "already added", *this);

    for(const Entry& entry: _entries)
        CORRADE_ASSERT(!keyHasPrefix(entry.key, prefix),
            "Utility::Arguments::addSkippedPrefix(): skipped prefix" << prefix << "conflicts with existing keys", *this);

    prefix += '-';

    _skippedPrefixes.emplace_back(std::move(prefix), std::move(help));
    return *this;
}

#ifndef CORRADE_TARGET_WINDOWS_RT
Arguments& Arguments::setFromEnvironment(const std::string& key, std::string environmentVariable) {
    auto found = find(_prefix + key);
    CORRADE_ASSERT(found != _entries.end(), "Utility::Arguments::setFromEnvironment(): key" << key << "doesn't exist", *this);
    CORRADE_ASSERT(found->type == Type::Option || found->type == Type::BooleanOption,
        "Utility::Arguments::setFromEnvironment(): only options can be set from environment", *this);

    found->environment = std::move(environmentVariable);
    return *this;
}

Arguments& Arguments::setFromEnvironment(const std::string& key) {
    return setFromEnvironment(key, uppercaseKey(_prefix + key));
}
#endif

Arguments& Arguments::setCommand(std::string name) {
    _command = std::move(name);
    return *this;
}

Arguments& Arguments::setGlobalHelp(std::string help) {
    CORRADE_ASSERT(_prefix.empty(),
        "Utility::Arguments::setGlobalHelp(): global help text only allowed in unprefixed version", *this);

    _help = std::move(help);
    return *this;
}

Arguments& Arguments::setHelp(const std::string& key, std::string help, std::string helpKey) {
    auto found = find(_prefix + key);
    CORRADE_ASSERT(found != _entries.end(), "Utility::Arguments::setHelp(): key" << key << "doesn't exist", *this);

    found->help = std::move(help);

    if(!helpKey.empty()) {
        CORRADE_ASSERT(found->type != Type::BooleanOption,
            "Utility::Arguments::setHelpKey(): help key can't be set for boolean option", *this);

        if(found->type == Type::NamedArgument || found->type == Type::Option)
            found->helpKey = _prefix + key + ' ' + std::move(helpKey);
        else found->helpKey = std::move(helpKey);
    }

    return *this;
}

void Arguments::parse(const int argc, const char** const argv) {
    const bool status = tryParse(argc, argv);

    if(_booleans.at(find(_prefix + "help")->id)) {
        Debug() << help();
        std::exit(0);
    }

    if(!status) {
        Error() << usage();
        std::exit(1);
    }
}

bool Arguments::tryParse(const int argc, const char** const argv) {
    CORRADE_INTERNAL_ASSERT(!argv == !argc);

    if(_command.empty() && argv && argc >= 1) _command = argv[0];

    for(auto&& boolean: _booleans) boolean = false;
    for(const Entry& entry: _entries) {
        if(entry.type == Type::BooleanOption) continue;

        CORRADE_INTERNAL_ASSERT(entry.id < _values.size());
        _values[entry.id] = entry.defaultValue;
    }

    #ifndef CORRADE_TARGET_WINDOWS_RT
    for(const Entry& entry: _entries) {
        if(entry.environment.empty()) continue;

        const char* const env = std::getenv(entry.environment.data());

        if(!env) continue;

        if(entry.type == Type::BooleanOption) {
            CORRADE_INTERNAL_ASSERT(entry.id < _booleans.size());
            _booleans[entry.id] = String::uppercase(
                env
                ) == "ON";
        } else {
            CORRADE_INTERNAL_ASSERT(entry.id < _values.size());
            _values[entry.id] =
                env
                ;
        }

    }
    #endif

    std::vector<Entry>::iterator valueFor = _entries.end();
    bool optionsAllowed = true;
    std::vector<Entry>::iterator nextArgument = _entries.begin();
    std::vector<bool> parsedArguments(_entries.size());

    for(int i = 1; i < argc; ++i) {
        if(valueFor != _entries.end()) {
            CORRADE_INTERNAL_ASSERT(valueFor->type != Type::BooleanOption);
            CORRADE_INTERNAL_ASSERT(valueFor->id < _values.size());
            _values[valueFor->id] = argv[i];
            parsedArguments[valueFor-_entries.begin()] = true;
            valueFor = _entries.end();
            continue;
        }

        const std::size_t len = std::strlen(argv[i]);

        if(optionsAllowed && len != 0 && argv[i][0] == '-') {
            std::vector<Entry>::iterator found = _entries.end();

            if(len == 2) {
                if(!_prefix.empty()) continue;

                const char key = argv[i][1];

                if(key == '-') {
                    optionsAllowed = false;
                    continue;
                }

                if(!verifyKey(key)) {
                    Error() << "Invalid command-line argument" << std::string("-") + key;
                    return false;
                }

                found = find(key);
                if(found == _entries.end()) {
                    Error() << "Unknown command-line argument" << std::string("-") + key;
                    return false;
                }

            } else if(len > 2) {
                if(std::strncmp(argv[i], "--", 2) == 0) {
                    const std::string key = argv[i]+2;

                    if(!_prefix.empty() && !keyHasPrefix(key, _prefix))
                        continue;

                    bool ignore = false;
                    for(auto&& prefix: _skippedPrefixes) {
                        if(!keyHasPrefix(key, prefix.first)) continue;

                        ignore = true;
                        if(key != prefix.first + "help") ++i;
                        break;
                    }
                    if(ignore) continue;

                    if(!verifyKey(key)) {
                        Error() << "Invalid command-line argument" << std::string("--") + key;
                        return false;
                    }

                    found = find(key);
                    if(found == _entries.end()) {
                        Error() << "Unknown command-line argument" << std::string("--") + key;
                        return false;
                    }

                } else {
                    if(!_prefix.empty()) continue;

                    Error() << "Invalid command-line argument" << argv[i] << std::string("(did you mean -") + argv[i] + "?)";
                    return false;
                }
            }

            CORRADE_INTERNAL_ASSERT(found != _entries.end());
            if(found->type == Type::BooleanOption) {
                CORRADE_INTERNAL_ASSERT(found->id < _booleans.size());
                _booleans[found->id] = true;
                parsedArguments[found-_entries.begin()] = true;

            } else valueFor = found;

        } else {
            if(!_prefix.empty()) continue;

            const auto found = findNextArgument(nextArgument);
            if(found == _entries.end()) {
                Error() << "Superfluous command-line argument" << argv[i];
                return false;
            }

            _values[found->id] = argv[i];
            parsedArguments[found-_entries.begin()] = true;
            nextArgument = found+1;
        }
    }

    if(valueFor != _entries.end()) {
        Error() << "Missing value for command-line argument" << keyName(*valueFor);
        return false;
    }

    bool success = true;

    for(std::size_t i = 0; i != _entries.size(); ++i) {
        const Entry& entry = _entries[i];

        if(entry.type == Type::BooleanOption || entry.type == Type::Option)
            continue;

        if(parsedArguments[i] != true) {
            Error() << "Missing command-line argument" << keyName(_entries[i]);
            success = false;
        }
    }

    _isParsed = success;

    return success;
}

std::string Arguments::usage() const {
    std::ostringstream out;
    out << "Usage:\n  " << (_command.empty() ? "./app" : _command);

    for(auto&& prefix: _skippedPrefixes)
        out << " [--" << prefix.first << "...]";

    bool hasArguments = false;
    for(const Entry& entry: _entries) {
        if(entry.type == Type::Argument) {
            hasArguments = true;
            continue;
        }

        out << ' ';

        if(entry.type == Type::Option || entry.type == Type::BooleanOption)
            out << '[';

        if(entry.shortKey)
            out << '-' << entry.shortKey << '|';
        out << "--" << entry.helpKey;

        if(entry.type == Type::Option || entry.type == Type::BooleanOption)
            out << ']';
    }

    if(hasArguments) out << " [--]";

    for(const Entry& entry: _entries) {
        if(entry.type != Type::Argument) continue;

        out << ' ' << entry.helpKey;
    }

    if(!_prefix.empty()) out << " ...";

    out << '\n';

    return out.str();
}

std::string Arguments::help() const {
    std::ostringstream out;
    out << usage();

    if(!_help.empty())
        out << '\n' << _help << '\n';

    constexpr std::size_t maxKeyColumnWidth = 26;
    std::size_t keyColumnWidth = 10;
    for(auto&& prefix: _skippedPrefixes) {
        keyColumnWidth = std::max(prefix.first.size() + 5, keyColumnWidth);

        if(keyColumnWidth >= maxKeyColumnWidth) {
            keyColumnWidth = maxKeyColumnWidth;
            break;
        }
    }

    if(keyColumnWidth != maxKeyColumnWidth) for(const Entry& entry: _entries) {
        if(entry.type == Type::Argument || (entry.defaultValue.empty() && entry.help.empty()
            #ifndef CORRADE_TARGET_WINDOWS_RT
            && entry.environment.empty()
            #endif
        ))
            continue;

        std::size_t currentKeyColumnWidth = entry.helpKey.size();
        if(entry.type != Type::Argument) {
            currentKeyColumnWidth += 2;
            if(entry.shortKey) currentKeyColumnWidth += 4;
        }

        keyColumnWidth = std::max(currentKeyColumnWidth, keyColumnWidth);

        if(keyColumnWidth >= maxKeyColumnWidth) {
            keyColumnWidth = maxKeyColumnWidth;
            break;
        }
    }

    out << "\nArguments:\n";

    if(!_prefix.empty()) {
        out << "  " << std::left << std::setw(keyColumnWidth) << "..." << "  main application arguments\n"
            << std::string(keyColumnWidth + 4, ' ') << "(see -h or --help for details)\n";
    }

    for(const Entry& entry: _entries) {
        if(entry.type != Type::Argument || entry.help.empty()) continue;

        out << "  " << std::left << std::setw(keyColumnWidth) << entry.helpKey << "  " << entry.help << '\n';
    }

    for(const Entry& entry: _entries) {
        if(entry.type == Type::Argument || (entry.defaultValue.empty() && entry.help.empty()
            #ifndef CORRADE_TARGET_WINDOWS_RT
            && entry.environment.empty()
            #endif
        ))
            continue;

        out << "  ";
        if(entry.shortKey)
            out << '-' << entry.shortKey << ", ";
        out << "--" << std::left << std::setw(keyColumnWidth - (entry.shortKey ? 6 : 2)) << entry.helpKey << "  ";

        if(!entry.help.empty()) out << entry.help << '\n';

        #ifndef CORRADE_TARGET_WINDOWS_RT
        if(!entry.environment.empty()) {
            if(!entry.help.empty()) out << std::string(keyColumnWidth + 4, ' ');
            out << "(environment: " << entry.environment;
            if(entry.type == Type::BooleanOption) out << "=ON|OFF";
            out << ")\n";
        }
        #endif

        if(!entry.defaultValue.empty()) {
            if(!entry.help.empty()) out << std::string(keyColumnWidth + 4, ' ');
            out << "(default: " << entry.defaultValue << ")\n";
        }
    }

    for(auto&& prefix: _skippedPrefixes) {
        out << "  --" << std::left << std::setw(keyColumnWidth) << prefix.first + "...  ";
        if(!prefix.second.empty()) out << prefix.second << '\n' << std::string(keyColumnWidth + 4, ' ');
        out << "(see --" << prefix.first << "help for details)\n";
    }

    return out.str();
}

std::string Arguments::valueInternal(const std::string& key) const {
    const auto found = find(_prefix + key);
    CORRADE_ASSERT(found != _entries.end(), "Utility::Arguments::value(): key" << key << "not found", {});
    CORRADE_ASSERT(found->type != Type::BooleanOption,
        "Utility::Arguments::value(): cannot use this function for boolean option" << key, {});
    CORRADE_INTERNAL_ASSERT(found->id < _values.size());
    CORRADE_ASSERT(_isParsed, "Utility::Arguments::value(): arguments were not successfully parsed yet", {});
    return _values[found->id];
}

bool Arguments::isSet(const std::string& key) const {
    const auto found = find(_prefix + key);
    CORRADE_ASSERT(found != _entries.end(), "Utility::Arguments::value(): key" << key << "not found", false);
    CORRADE_ASSERT(found->type == Type::BooleanOption,
        "Utility::Arguments::isSet(): cannot use this function for non-boolean value" << key, false);
    CORRADE_INTERNAL_ASSERT(found->id < _booleans.size());
    CORRADE_ASSERT(_isParsed, "Utility::Arguments::isSet(): arguments were not successfully parsed yet", {});
    return _booleans[found->id];
}

bool Arguments::skippedPrefix(const std::string& key) const {
    for(auto&& prefix: _skippedPrefixes)
        if(keyHasPrefix(key, prefix.first)) return true;

    return false;
}

bool Arguments::verifyKey(const std::string& key) const {
    static constexpr const char allowed[] { "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-" };

    return key.size() > 1 && key.find_first_not_of(allowed) == std::string::npos;
}

bool Arguments::verifyKey(char shortKey) const {
    static constexpr const char allowedShort[] { "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789" };

    return !shortKey || std::strchr(allowedShort, shortKey) != nullptr;
}

auto Arguments::find(const std::string& key) -> std::vector<Entry>::iterator {
    for(auto it = _entries.begin(); it != _entries.end(); ++it)
        if(it->key == key) return it;

    return _entries.end();
}

auto Arguments::find(const std::string& key) const -> std::vector<Entry>::const_iterator {
    for(auto it = _entries.begin(); it != _entries.end(); ++it)
        if(it->key == key) return it;

    return _entries.end();
}

auto Arguments::find(const char shortKey) -> std::vector<Entry>::iterator {
    for(auto it = _entries.begin(); it != _entries.end(); ++it)
        if(it->shortKey == shortKey) return it;

    return _entries.end();
}

auto Arguments::findNextArgument(const std::vector<Entry>::iterator start) -> std::vector<Entry>::iterator {
    for(auto it = start; it != _entries.end(); ++it)
        if(it->type == Type::Argument) return it;

    return _entries.end();
}

inline std::string Arguments::keyName(const Entry& entry) const {
    return entry.type == Type::Argument ? entry.helpKey : "--" + entry.key;
}

}}
namespace Corrade { namespace Utility {

namespace Implementation {
    template<class T> std::string BasicConfigurationValue<T>::toString(const T& value, ConfigurationValueFlags flags) {
        std::ostringstream stream;

        if(flags & ConfigurationValueFlag::Hex)
            stream.setf(std::istringstream::hex, std::istringstream::basefield);
        else if(flags & ConfigurationValueFlag::Oct)
            stream.setf(std::istringstream::oct, std::istringstream::basefield);
        else if(flags & ConfigurationValueFlag::Scientific)
            stream.setf(std::istringstream::scientific, std::istringstream::floatfield);

        if(flags & ConfigurationValueFlag::Uppercase)
            stream.setf(std::istringstream::uppercase);

        stream << value;
        return stream.str();
    }

    template<class T> T BasicConfigurationValue<T>::fromString(const std::string& stringValue, ConfigurationValueFlags flags) {
        if(stringValue.empty()) return T{};

        std::istringstream stream{stringValue};

        if(flags & ConfigurationValueFlag::Hex)
            stream.setf(std::istringstream::hex, std::istringstream::basefield);
        else if(flags & ConfigurationValueFlag::Oct)
            stream.setf(std::istringstream::oct, std::istringstream::basefield);
        else if(flags & ConfigurationValueFlag::Scientific)
            stream.setf(std::istringstream::scientific, std::istringstream::floatfield);

        if(flags & ConfigurationValueFlag::Uppercase)
            stream.setf(std::istringstream::uppercase);

        T value;
        stream >> value;
        return value;
    }

    template struct BasicConfigurationValue<short>;
    template struct BasicConfigurationValue<unsigned short>;
    template struct BasicConfigurationValue<int>;
    template struct BasicConfigurationValue<unsigned int>;
    template struct BasicConfigurationValue<long>;
    template struct BasicConfigurationValue<unsigned long>;
    template struct BasicConfigurationValue<long long>;
    template struct BasicConfigurationValue<unsigned long long>;
    template struct BasicConfigurationValue<float>;
    template struct BasicConfigurationValue<double>;
    template struct BasicConfigurationValue<long double>;
    template struct BasicConfigurationValue<std::string>;
}

bool ConfigurationValue<bool>::fromString(const std::string& value, ConfigurationValueFlags) {
    return value == "1" || value == "yes" || value == "y" || value == "true";
}
std::string ConfigurationValue<bool>::toString(const bool value, ConfigurationValueFlags) {
    return value ? "true" : "false";
}

char32_t ConfigurationValue<char32_t>::fromString(const std::string& value, ConfigurationValueFlags) {
    return char32_t(ConfigurationValue<unsigned long long>::fromString(value, ConfigurationValueFlag::Hex|ConfigurationValueFlag::Uppercase));
}
std::string ConfigurationValue<char32_t>::toString(const char32_t value, ConfigurationValueFlags) {
    return ConfigurationValue<unsigned long long>::toString(value, ConfigurationValueFlag::Hex|ConfigurationValueFlag::Uppercase);
}

}}
#ifdef CORRADE_TARGET_UNIX

#else
#endif

namespace Corrade { namespace Utility {

namespace {

template<class T> inline void toStream(std::ostream& s, const T& value) {
    s << value;
}

template<> inline void toStream<Implementation::DebugOstreamFallback>(std::ostream& s, const Implementation::DebugOstreamFallback& value) {
    value.apply(s);
}

}

std::ostream* Debug::_globalOutput = &std::cout;
std::ostream* Warning::_globalWarningOutput = &std::cerr;
std::ostream* Error::_globalErrorOutput = &std::cerr;

#ifndef CORRADE_UTILITY_USE_ANSI_COLORS
Debug::Color Debug::_globalColor = Debug::Color::Default;
bool Debug::_globalColorBold = false;
#endif

template<Debug::Color c, bool bold> Debug::Modifier Debug::colorInternal() {
    return [](Debug& debug) {
        if(!debug._output || (debug._flags & InternalFlag::DisableColors)) return;

        debug._flags |= InternalFlag::ColorWritten|InternalFlag::ValueWritten;
        _globalColor = c;
        _globalColorBold = bold;
        constexpr const char code[] = { '\033', '[', bold ? '1' : '0', ';', '3', '0' + char(c), 'm', '\0' };
        *debug._output << code;
    };
}

inline void Debug::resetColorInternal() {
    if(!_output || !(_flags & InternalFlag::ColorWritten)) return;

    _flags &= ~InternalFlag::ColorWritten;
    _flags |= InternalFlag::ValueWritten;
    if(_previousColor != Color::Default || _previousColorBold) {
        const char code[] = { '\033', '[', _previousColorBold ? '1' : '0', ';', '3', char('0' + char(_previousColor)), 'm', '\0' };
        *_output << code;
    } else *_output << "\033[0m";

    _globalColor = _previousColor;
    _globalColorBold = _previousColorBold;
}

auto Debug::color(Color color) -> Modifier {
    switch(color) {
        #define _c(color) case Color::color: return colorInternal<Color::color, false>();
        _c(Black)
        _c(Red)
        _c(Green)
        _c(Yellow)
        _c(Blue)
        _c(Magenta)
        _c(Cyan)
        _c(White)
        #ifndef CORRADE_UTILITY_USE_ANSI_COLORS
        _c(Default)
        #endif
        #undef _c
    }

    return [](Debug&) {};
}

auto Debug::boldColor(Color color) -> Modifier {
    switch(color) {
        #define _c(color) case Color::color: return colorInternal<Color::color, true>();
        _c(Black)
        _c(Red)
        _c(Green)
        _c(Yellow)
        _c(Blue)
        _c(Magenta)
        _c(Cyan)
        _c(White)
        #ifndef CORRADE_UTILITY_USE_ANSI_COLORS
        _c(Default)
        #endif
        #undef _c
    }

    return [](Debug&) {};
}

void Debug::resetColor(Debug& debug) {
    debug.resetColorInternal();
}

std::ostream* Debug::output() { return _globalOutput; }
std::ostream* Warning::output() { return _globalWarningOutput; }
std::ostream* Error::output() { return _globalErrorOutput; }

bool Debug::isTty(std::ostream* const output) {
    #if defined(CORRADE_UTILITY_USE_ANSI_COLORS) || defined(CORRADE_TARGET_UNIX)
    return
        #ifdef _MSC_VER
        #pragma warning(push)
        #pragma warning(disable: 4996)
        #endif
        ((output == &std::cout && isatty(1)) ||
         (output == &std::cerr && isatty(2)))
        #ifdef _MSC_VER
        #pragma warning(pop)
        #endif
        ;

    #else
    return false;
    #endif
}

bool Debug::isTty() { return isTty(_globalOutput); }
bool Warning::isTty() { return Debug::isTty(_globalWarningOutput); }
bool Error::isTty() { return Debug::isTty(_globalErrorOutput); }

Debug::Debug(std::ostream* const output, const Flags flags): _flags{InternalFlag(static_cast<unsigned char>(flags))|InternalFlag::NoSpaceBeforeNextValue} {
    _previousGlobalOutput = _globalOutput;
    _globalOutput = _output = output;

    _previousColor = _globalColor;
    _previousColorBold = _globalColorBold;
}

Warning::Warning(std::ostream* const output, const Flags flags): Debug{flags} {
    _previousGlobalWarningOutput = _globalWarningOutput;
    _globalWarningOutput = _output = output;
}

Error::Error(std::ostream* const output, const Flags flags): Debug{flags} {
    _previousGlobalErrorOutput = _globalErrorOutput;
    _globalErrorOutput = _output = output;
}

Debug::Debug(const Flags flags): Debug{_globalOutput, flags} {}
Warning::Warning(const Flags flags): Warning{_globalWarningOutput, flags} {}
Error::Error(const Flags flags): Error{_globalErrorOutput, flags} {}

void Debug::cleanupOnDestruction() {
    resetColorInternal();

    if(_output && (_flags & InternalFlag::ValueWritten) && !(_flags & InternalFlag::NoNewlineAtTheEnd))
        *_output << std::endl;

    _globalOutput = _previousGlobalOutput;
}

Debug::~Debug() {
    cleanupOnDestruction();
}

Warning::~Warning() {
    _globalWarningOutput = _previousGlobalWarningOutput;
}

void Error::cleanupOnDestruction() {
    _globalErrorOutput = _previousGlobalErrorOutput;
}

Error::~Error() {
    cleanupOnDestruction();
}

Fatal::~Fatal() {
    Error::cleanupOnDestruction();
    Debug::cleanupOnDestruction();

    std::exit(_exitCode);
}

template<class T> Debug& Debug::print(const T& value) {
    if(!_output) return *this;

    if(_flags & InternalFlag::NoSpaceBeforeNextValue)
        _flags &= ~InternalFlag::NoSpaceBeforeNextValue;
    else *_output << ' ';

    toStream(*_output, value);

    _flags |= InternalFlag::ValueWritten;
    return *this;
}

Debug& Debug::operator<<(const std::string& value) { return print(value); }

Debug& Debug::operator<<(const void* const value) {
    std::ostringstream o;
    o << "0x" << std::hex << reinterpret_cast<std::uintptr_t>(value);
    return print(o.str());
}

Debug& Debug::operator<<(const char* value) { return print(value); }
Debug& Debug::operator<<(bool value) { return print(value ? "true" : "false"); }
Debug& Debug::operator<<(int value) { return print(value); }
Debug& Debug::operator<<(long value) { return print(value); }
Debug& Debug::operator<<(long long value) { return print(value); }
Debug& Debug::operator<<(unsigned value) { return print(value); }
Debug& Debug::operator<<(unsigned long value) { return print(value); }
Debug& Debug::operator<<(unsigned long long value) { return print(value); }
Debug& Debug::operator<<(float value) {
    if(!_output) return *this;
    *_output << std::setprecision(6);
    return print(value);
}
Debug& Debug::operator<<(double value) {
    if(!_output) return *this;
    *_output << std::setprecision(15);
    return print(value);
}
Debug& Debug::operator<<(long double value) {
    if(!_output) return *this;
    *_output << std::setprecision(18);
    return print(value);
}

Debug& Debug::operator<<(char32_t value) {
    std::ostringstream o;
    o << "U+" << std::hex << std::uppercase << std::setw(4) << std::setfill('0') << value;
    return print(o.str());
}

Debug& Debug::operator<<(const char32_t* value) {
    return *this << std::u32string(value);
}

Debug& Debug::operator<<(std::nullptr_t) {
    return print("nullptr");
}

Debug& Debug::operator<<(Implementation::DebugOstreamFallback&& value) {
    return print(value);
}

Debug& operator<<(Debug& debug, Debug::Color value) {
    switch(value) {
        #define _c(value) case Debug::Color::value: return debug << "Debug::Color::" #value;
        _c(Black)
        _c(Red)
        _c(Green)
        _c(Yellow)
        _c(Blue)
        _c(Magenta)
        _c(Cyan)
        _c(White)
        #ifndef CORRADE_UTILITY_USE_ANSI_COLORS
        _c(Default)
        #endif
        #undef _c
    }

    return debug << "Debug::Color(" << Debug::nospace << reinterpret_cast<void*>(static_cast<unsigned char>(char(value))) << Debug::nospace << ")";
}

}}
#ifndef Corrade_Containers_ScopeGuard_h
#define Corrade_Containers_ScopeGuard_h

namespace Corrade { namespace Containers {

class ScopeGuard {
    public:
        template<class T, class Deleter> explicit ScopeGuard(T handle, Deleter deleter);

        template<class Deleter> explicit ScopeGuard(Deleter deleter);

        #ifdef CORRADE_MSVC2015_COMPATIBILITY
        template<class T, class U> explicit ScopeGuard(T handle, U(*deleter)(T));
        template<class U> explicit ScopeGuard(U(*deleter)());
        #endif

        ScopeGuard(const ScopeGuard&) = delete;

        ScopeGuard(ScopeGuard&&) = delete;

        ScopeGuard& operator=(const ScopeGuard&) = delete;

        ScopeGuard& operator=(ScopeGuard&&) = delete;

        void release() { _deleterWrapper = nullptr; }

        ~ScopeGuard() {
            if(_deleterWrapper) _deleterWrapper(&_deleter, &_handle);
        }

    private:
        void(*_deleterWrapper)(void(**)(), void**);
        void(*_deleter)();
        void* _handle;
};

template<class T, class Deleter> ScopeGuard::ScopeGuard(T handle, Deleter deleter): _deleter{
    #ifndef CORRADE_MSVC2015_COMPATIBILITY
    reinterpret_cast<void(*)()>(+deleter)
    #else
    reinterpret_cast<void(*)()>(static_cast<void(*)(T)>(deleter))
    #endif
}, _handle{reinterpret_cast<void*>(handle)} {
    static_assert(sizeof(T) <= sizeof(void*), "handle too big to store");
    _deleterWrapper = [](void(**deleter)(), void** handle) {
        (*reinterpret_cast<Deleter*>(deleter))(*reinterpret_cast<T*>(handle));
    };
}

template<class Deleter> ScopeGuard::ScopeGuard(Deleter deleter): _deleter{
    #ifndef CORRADE_MSVC2015_COMPATIBILITY
    reinterpret_cast<void(*)()>(+deleter)
    #else
    reinterpret_cast<void(*)()>(static_cast<void(*)()>(deleter))
    #endif
}, _handle{nullptr} {
    _deleterWrapper = [](void(**deleter)(), void**) {
        (*reinterpret_cast<Deleter*>(deleter))();
    };
}

#ifdef CORRADE_MSVC2015_COMPATIBILITY
template<class T, class U> ScopeGuard::ScopeGuard(T handle, U(*deleter)(T)): _deleter{reinterpret_cast<void(*)()>(deleter)}, _handle{reinterpret_cast<void*>(handle)} {
    static_assert(sizeof(T) <= sizeof(void*), "handle too big to store");
    _deleterWrapper = [](void(**deleter)(), void** handle) {
        (*reinterpret_cast<U(**)(T)>(deleter))(*reinterpret_cast<T*>(handle));
    };
}

template<class U> ScopeGuard::ScopeGuard(U(*deleter)()): _deleter{reinterpret_cast<void(*)()>(deleter)}, _handle{nullptr} {
    _deleterWrapper = [](void(**deleter)(), void**) {
        (*reinterpret_cast<U(**)()>(deleter))();
    };
}
#endif

}}

#endif
#ifdef CORRADE_TARGET_UNIX
#define _FILE_OFFSET_BITS 64
#endif

#ifdef __MINGW32__
#undef __STRICT_ANSI__
#endif

namespace Corrade { namespace Utility { namespace Directory {

std::string fromNativeSeparators(std::string path) {
    return path;
}

std::string toNativeSeparators(std::string path) {
    return path;
}

std::string path(const std::string& filename) {
    if(!filename.empty() && filename.back() == '/')
        return filename.substr(0, filename.size()-1);

    std::size_t pos = filename.find_last_of('/');

    if(pos == std::string::npos) return {};

    return filename.substr(0, pos);
}

std::string filename(const std::string& filename) {
    std::size_t pos = filename.find_last_of('/');

    if(pos == std::string::npos) return filename;

    return filename.substr(pos+1);
}

std::string join(const std::string& path, const std::string& filename) {
    if(path.empty()) return filename;

    if(!filename.empty() && filename[0] == '/')
        return filename;

    if(path.back() != '/')
        return path + '/' + filename;

    return path + filename;
}

bool mkpath(const std::string& path) {
    if(path.empty()) return false;

    if(path.back() == '/')
        return mkpath(path.substr(0, path.size()-1));

    if(exists(path)) return true;

    const std::string parentPath = Directory::path(path);
    if(!parentPath.empty() && !exists(parentPath) && !mkpath(parentPath)) return false;

    #ifdef CORRADE_TARGET_UNIX
    const int ret = mkdir(path.data(), 0777);
    return ret == 0;

    #else
    Warning() << "Utility::Directory::mkdir(): not implemented on this platform";
    return false;
    #endif
}

bool rm(const std::string& path) {

    return std::remove(path.data()) == 0;
}

bool move(const std::string& oldPath, const std::string& newPath) {
    return
        std::rename(oldPath.data(), newPath.data())
        == 0;
}

bool exists(const std::string& filename) {
    #ifdef CORRADE_TARGET_UNIX
    return access(filename.data(), F_OK) == 0;

    #else
    static_cast<void>(filename);
    Warning() << "Utility::Directory::exists(): not implemented on this platform";
    return false;
    #endif
}

bool isSandboxed() {
    #if defined(CORRADE_TARGET_IOS) || defined(CORRADE_TARGET_ANDROID) || defined(CORRADE_TARGET_WINDOWS_RT)
    return true;
    #else
    return false;
    #endif
}

std::string executableLocation() {
    #ifdef __linux__
    constexpr const char self[]{"/proc/self/exe"};
    std::string path(4, '\0');
    ssize_t size;
    while((size = readlink(self, &path[0], path.size())) == ssize_t(path.size()))
        path.resize(path.size()*2);

    CORRADE_INTERNAL_ASSERT(size > 0);

    path.resize(size);
    return path;

    #else
    return std::string{};
    #endif
}

std::string home() {
    #ifdef CORRADE_TARGET_UNIX
    if(const char* const h = std::getenv("HOME"))
        return h;
    return std::string{};

    #else
    Warning() << "Utility::Directory::home(): not implemented on this platform";
    return {};
    #endif
}

std::string configurationDir(const std::string& applicationName) {
    #ifdef __unix__
    const std::string lowercaseApplicationName = String::lowercase(applicationName);
    if(const char* const config = std::getenv("XDG_CONFIG_HOME"))
        return join(config, lowercaseApplicationName);

    const std::string home = Directory::home();
    return home.empty() ? std::string{} : join(home, ".config/" + lowercaseApplicationName);

    #else
    static_cast<void>(applicationName);
    Warning() << "Utility::Directory::configurationDir(): not implemented on this platform";
    return {};
    #endif
}

std::string tmp() {
    #ifdef CORRADE_TARGET_UNIX

    #ifdef CORRADE_TARGET_ANDROID
    return "/data/local/tmp";
    #endif

    return "/tmp";

    #else
    Warning() << "Utility::Directory::tmp(): not implemented on this platform";
    return {};
    #endif
}

std::vector<std::string> list(const std::string& path, Flags flags) {
    std::vector<std::string> list;

    #ifdef CORRADE_TARGET_UNIX
    DIR* directory = opendir(path.data());
    if(!directory) return list;

    dirent* entry;
    while((entry = readdir(directory)) != nullptr) {
        if((flags >= Flag::SkipDirectories) && entry->d_type == DT_DIR)
            continue;
        if((flags >= Flag::SkipFiles) && entry->d_type == DT_REG)
            continue;
        if((flags >= Flag::SkipSpecial) && entry->d_type != DT_DIR && entry->d_type != DT_REG)
            continue;

        std::string file{entry->d_name};
        if((flags >= Flag::SkipDotAndDotDot) && (file == "." || file == ".."))
            continue;

        list.push_back(std::move(file));
    }

    closedir(directory);

    #else
    Warning() << "Utility::Directory::list(): not implemented on this platform";
    static_cast<void>(path);
    #endif

    if(flags >= Flag::SortAscending)
        std::sort(list.begin(), list.end());
    else if(flags >= Flag::SortDescending)
        std::sort(list.rbegin(), list.rend());

    return list;
}

Containers::Array<char> read(const std::string& filename) {
    std::FILE* const f = std::fopen(filename.data(), "rb");
    if(!f) {
        Error{} << "Utility::Directory::read(): can't open" << filename;
        return nullptr;
    }

    Containers::ScopeGuard exit{f, std::fclose};

    #ifdef CORRADE_TARGET_UNIX
    if(
        lseek(fileno(f), 0, SEEK_END) == -1
    ) {
        std::string data;
        char buffer[4096];
        std::size_t count;
        do {
            count = std::fread(buffer, 1, Containers::arraySize(buffer), f);
            data.append(buffer, count);
        } while(count);

        Containers::Array<char> out{data.size()};
        std::copy(data.begin(), data.end(), out.begin());
        return out;
    }
    #else
    #endif

    std::fseek(f, 0, SEEK_END);
    #ifdef CORRADE_TARGET_UNIX
    const std::size_t size = ftello(f);
    #else
    const std::size_t size = std::ftell(f);
    #endif
    std::rewind(f);

    Containers::Array<char> out{size};
    CORRADE_INTERNAL_ASSERT(std::fread(out, 1, size, f) == size);
    return out;
}

std::string readString(const std::string& filename) {
    const auto data = read(filename);

    return {data, data.size()};
}

bool write(const std::string& filename, const Containers::ArrayView<const void> data) {
    std::FILE* const f = std::fopen(filename.data(), "wb");
    if(!f) {
        Error{} << "Utility::Directory::write(): can't open" << filename;
        return false;
    }

    Containers::ScopeGuard exit{f, std::fclose};

    std::fwrite(data, 1, data.size(), f);
    return true;
}

bool writeString(const std::string& filename, const std::string& data) {
    static_assert(sizeof(std::string::value_type) == 1, "std::string doesn't have 8-bit characters");
    return write(filename, {data.data(), data.size()});
}

bool append(const std::string& filename, const Containers::ArrayView<const void> data) {
    std::FILE* const f = std::fopen(filename.data(), "ab");
    if(!f) {
        Error{} << "Utility::Directory::append(): can't open" << filename;
        return false;
    }

    Containers::ScopeGuard exit{f, std::fclose};

    std::fwrite(data, 1, data.size(), f);
    return true;
}

bool appendString(const std::string& filename, const std::string& data) {
    static_assert(sizeof(std::string::value_type) == 1, "std::string doesn't have 8-bit characters");
    return append(filename, {data.data(), data.size()});
}

bool copy(const std::string& from, const std::string& to) {
    std::FILE* const in = std::fopen(from.data(), "rb");
    std::FILE* const out = std::fopen(to.data(), "wb");
    if(!in) {
        Error{} << "Utility::Directory::copy(): can't open" << from;
        return false;
    }
    if(!out) {
        Error{} << "Utility::Directory::copy(): can't open" << to;
        return false;
    }

    #if defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 200112L
    posix_fadvise(fileno(in), 0, 0, POSIX_FADV_SEQUENTIAL);
    #endif

    Containers::ScopeGuard exitIn{in, std::fclose};
    Containers::ScopeGuard exitOut{out, std::fclose};

    char buffer[128*1024];
    std::size_t count;
    do {
        count = std::fread(buffer, 1, Containers::arraySize(buffer), in);
        std::fwrite(buffer, 1, count, out);
    } while(count);

    return true;
}

#ifdef CORRADE_TARGET_UNIX
void MapDeleter::operator()(const char* const data, const std::size_t size) {
    if(data && munmap(const_cast<char*>(data), size) == -1)
        Error() << "Utility::Directory: can't unmap memory-mapped file";
    if(_fd) close(_fd);
}

Containers::Array<char, MapDeleter> map(const std::string& filename, std::size_t size) {
    const int fd = open(filename.data(), O_RDWR|O_CREAT|O_TRUNC, mode_t(0600));
    if(fd == -1) {
        Error() << "Utility::Directory::map(): can't open" << filename;
        return nullptr;
    }

    if(lseek(fd, size - 1, SEEK_SET) == -1) {
        close(fd);
        Error() << "Utility::Directory::map(): can't seek to resize the file";
        return nullptr;
    }

    if(::write(fd, "", 1) != 1) {
        close(fd);
        Error() << "Utility::Directory::map(): can't write to resize the file";
        return nullptr;
    }

    char* data = reinterpret_cast<char*>(mmap(nullptr, size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0));
    if(data == MAP_FAILED) {
        close(fd);
        Error() << "Utility::Directory::map(): can't map the file";
        return nullptr;
    }

    return Containers::Array<char, MapDeleter>{data, size, MapDeleter{fd}};
}

Containers::Array<const char, MapDeleter> mapRead(const std::string& filename) {
    const int fd = open(filename.data(), O_RDONLY);
    if(fd == -1) {
        Error() << "Utility::Directory::mapRead(): can't open" << filename;
        return nullptr;
    }

    const off_t currentPos = lseek(fd, 0, SEEK_CUR);
    const std::size_t size = lseek(fd, 0, SEEK_END);
    lseek(fd, currentPos, SEEK_SET);

    const char* data = reinterpret_cast<const char*>(mmap(nullptr, size, PROT_READ, MAP_SHARED, fd, 0));
    if(data == MAP_FAILED) {
        close(fd);
        Error() << "Utility::Directory::mapRead(): can't map the file";
        return nullptr;
    }

    return Containers::Array<const char, MapDeleter>{data, size, MapDeleter{fd}};
}
#else
#endif

}}}
namespace Corrade { namespace Utility { namespace String {

namespace Implementation {

void ltrimInPlace(std::string& string, const Containers::ArrayView<const char> characters) {
    string.erase(0, string.find_first_not_of(characters, 0, characters.size()));
}

void rtrimInPlace(std::string& string, const Containers::ArrayView<const char> characters) {
    string.erase(string.find_last_not_of(characters, std::string::npos, characters.size())+1);
}

void trimInPlace(std::string& string, const Containers::ArrayView<const char> characters) {
    rtrimInPlace(string, characters);
    ltrimInPlace(string, characters);
}

std::string ltrim(std::string string, const Containers::ArrayView<const char> characters) {
    ltrimInPlace(string, characters);
    return string;
}

std::string rtrim(std::string string, const Containers::ArrayView<const char> characters) {
    rtrimInPlace(string, characters);
    return string;
}

std::string trim(std::string string, const Containers::ArrayView<const char> characters) {
    trimInPlace(string, characters);
    return string;
}

std::vector<std::string> splitWithoutEmptyParts(const std::string& string, const Containers::ArrayView<const char> delimiters) {
    std::vector<std::string> parts;
    std::size_t oldpos = 0, pos = std::string::npos;

    while((pos = string.find_first_of(delimiters, oldpos, delimiters.size())) != std::string::npos) {
        if(pos != oldpos)
            parts.push_back(string.substr(oldpos, pos-oldpos));

        oldpos = pos+1;
    }

    if(!string.empty() && (oldpos < string.size()))
        parts.push_back(string.substr(oldpos));

    return parts;
}

bool beginsWith(Containers::ArrayView<const char> string, const Containers::ArrayView<const char> prefix) {
    if(string.size() < prefix.size()) return false;

    return std::strncmp(string, prefix, prefix.size()) == 0;
}

bool endsWith(Containers::ArrayView<const char> string, const Containers::ArrayView<const char> suffix) {
    if(string.size() < suffix.size()) return false;

    return std::strncmp(string + string.size() - suffix.size(), suffix, suffix.size()) == 0;
}

std::string stripPrefix(std::string string, const Containers::ArrayView<const char> prefix) {
    CORRADE_ASSERT(beginsWith({string.data(), string.size()}, prefix),
        "Utility::String::stripPrefix(): string doesn't begin with given prefix", {});
    string.erase(0, prefix.size());
    return string;
}

std::string stripSuffix(std::string string, const Containers::ArrayView<const char> suffix) {
    CORRADE_ASSERT(endsWith({string.data(), string.size()}, suffix),
        "Utility::String::stripSuffix(): string doesn't end with given suffix", {});
    string.erase(string.size() - suffix.size());
    return string;
}

std::string replaceFirst(std::string string, const Containers::ArrayView<const char> search, const Containers::ArrayView<const char> replace) {
    const std::size_t found = string.find(search, 0, search.size());
    if(found != std::string::npos)
        string.replace(found, search.size(), replace, replace.size());
    return string;
}

std::string replaceAll(std::string string, const Containers::ArrayView<const char> search, const Containers::ArrayView<const char> replace) {
    CORRADE_ASSERT(!search.empty(), "Utility::String::replaceAll(): empty search string would cause an infinite loop", {});
    std::size_t found = 0;
    while((found = string.find(search, found, search.size())) != std::string::npos) {
        string.replace(found, search.size(), replace, replace.size());
        found += replace.size();
    }
    return string;
}

}

namespace {
    constexpr const char Whitespace[] = " \t\f\v\r\n";
}

std::string ltrim(std::string string) { return ltrim(std::move(string), Whitespace); }

std::string rtrim(std::string string) { return rtrim(std::move(string), Whitespace); }

std::string trim(std::string string) { return trim(std::move(string), Whitespace); }

void ltrimInPlace(std::string& string) { ltrimInPlace(string, Whitespace); }

void rtrimInPlace(std::string& string) { rtrimInPlace(string, Whitespace); }

void trimInPlace(std::string& string) { trimInPlace(string, Whitespace); }

std::vector<std::string> splitWithoutEmptyParts(const std::string& string) {
    return splitWithoutEmptyParts(string, Whitespace);
}

std::vector<std::string> split(const std::string& string, const char delimiter) {
    std::vector<std::string> parts;
    std::size_t oldpos = 0, pos = std::string::npos;

    while((pos = string.find(delimiter, oldpos)) != std::string::npos) {
        parts.push_back(string.substr(oldpos, pos-oldpos));
        oldpos = pos+1;
    }

    if(!string.empty())
        parts.push_back(string.substr(oldpos));

    return parts;
}

std::vector<std::string> splitWithoutEmptyParts(const std::string& string, const char delimiter) {
    std::vector<std::string> parts;
    std::size_t oldpos = 0, pos = std::string::npos;

    while((pos = string.find(delimiter, oldpos)) != std::string::npos) {
        if(pos != oldpos)
            parts.push_back(string.substr(oldpos, pos-oldpos));

        oldpos = pos+1;
    }

    if(!string.empty() && (oldpos < string.size()))
        parts.push_back(string.substr(oldpos));

    return parts;
}

std::string join(const std::vector<std::string>& strings, const char delimiter) {
    std::size_t size = 0;
    for(const auto& s: strings) size += s.size() + 1;
    if(size) --size;

    std::string result;
    result.reserve(size);

    for(const auto& s: strings) {
        result += s;
        if(result.size() != size) result += delimiter;
    }

    return result;
}

std::string joinWithoutEmptyParts(const std::vector<std::string>& strings, const char delimiter) {
    std::size_t size = 0;
    for(const auto& s: strings) if(!s.empty()) size += s.size() + 1;
    if(size) --size;

    std::string result;
    result.reserve(size);

    for(const auto& s: strings) {
        if(s.empty()) continue;

        result += s;
        if(result.size() != size) result += delimiter;
    }

    return result;
}

std::string lowercase(std::string string) {
    std::transform(string.begin(), string.end(), string.begin(), static_cast<int (*)(int)>(std::tolower));
    return string;
}

std::string uppercase(std::string string) {
    std::transform(string.begin(), string.end(), string.begin(), static_cast<int (*)(int)>(std::toupper));
    return string;
}

}}}
#endif
