/*
    Corrade::Containers::Optional
        — a lightweight alternative to std::optional

    https://doc.magnum.graphics/corrade/classCorrade_1_1Containers_1_1Optional.html

    This is a single-header library generated from the Corrade project. With
    the goal being easy integration, it's deliberately free of all comments
    to keep the file size small. More info, detailed changelogs and docs here:

    -   Project homepage — https://magnum.graphics/corrade/
    -   Documentation — https://doc.magnum.graphics/corrade/
    -   GitHub project page — https://github.com/mosra/corrade
    -   GitHub Singles repository — https://github.com/mosra/magnum-singles

    The STL compatibility bits are included as well --- opt-in by specifying
    `#define CORRADE_OPTIONAL_STL_COMPATIBILITY` before including the file.
    Including it multiple times with different macros defined works too.

    v2020.06-1502-g147e (2023-09-11)
    -   Fixes to the Utility::swap() helper to avoid ambiguity with std::swap()
    v2020.06-1454-gfc3b7 (2023-08-27)
    -   The InPlaceInit tag is moved from Containers to the root namespace
    -   The underlying type is exposed in a new Optional::Type typedef
    -   Working around false-positive uninitialized value warnings in GCC 10+
    -   Further workarounds for various compiler-specific issues and standard
        defects when using {}-initialization for aggregate types
    -   Removed dependency on <utility>, resulting in about ~600 preprocessed
        lines less
    v2020.06-0-g61d1b58c (2020-06-27)
    -   Working around various compiler-specific issues and standard defects
        when using {}-initialization for aggregate types
    v2019.10-0-g162d6a7d (2019-10-24)
    -   Minor simplifications in the internals
    v2019.01-107-g80d9f347 (2019-03-23)
    -   Including <cassert> only when needed
    v2018.10-232-ge927d7f3 (2019-01-28)
    -   Ability to "take" a value out of a r-value optional using operator*
    -   Opt-in compatibility with <optional> from C++17
    v2018.10-183-g4eb1adc0 (2019-01-23)
    -   Initial release

    Generated from Corrade v2020.06-1502-g147e (2023-09-11), 456 / 1879 LoC
*/

/*
    This file is part of Corrade.

    Copyright © 2007, 2008, 2009, 2010, 2011, 2012, 2013, 2014, 2015, 2016,
                2017, 2018, 2019, 2020, 2021, 2022, 2023
              Vladimír Vondruš <mosra@centrum.cz>

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

#include <new>
#include <type_traits>
#if !defined(CORRADE_ASSERT) && !defined(NDEBUG)
#include <cassert>
#endif

#if defined(_MSC_VER) && _MSC_VER < 1910
#define CORRADE_MSVC2015_COMPATIBILITY
#endif
#ifdef __GNUC__
#define CORRADE_TARGET_GCC
#endif
#ifdef __clang__
#define CORRADE_TARGET_CLANG
#endif

#ifndef Corrade_Tags_h
#define Corrade_Tags_h

namespace Corrade {

struct DefaultInitT {
    struct Init {};
    constexpr explicit DefaultInitT(Init) {}
};

struct ValueInitT {
    struct Init {};
    constexpr explicit ValueInitT(Init) {}
};

struct NoInitT {
    struct Init {};
    constexpr explicit NoInitT(Init) {}
};

struct NoCreateT {
    struct Init {};
    constexpr explicit NoCreateT(Init) {}
};

struct DirectInitT {
    struct Init {};
    constexpr explicit DirectInitT(Init) {}
};

struct InPlaceInitT {
    struct Init {};
    constexpr explicit InPlaceInitT(Init) {}
};

constexpr DefaultInitT DefaultInit{DefaultInitT::Init{}};

constexpr ValueInitT ValueInit{ValueInitT::Init{}};

constexpr NoInitT NoInit{NoInitT::Init{}};

constexpr NoCreateT NoCreate{NoCreateT::Init{}};

constexpr DirectInitT DirectInit{DirectInitT::Init{}};

constexpr InPlaceInitT InPlaceInit{InPlaceInitT::Init{}};

}

#endif
#ifndef Corrade_Utility_Move_h
#define Corrade_Utility_Move_h

#ifdef CORRADE_MSVC2015_COMPATIBILITY
#include <utility>
#endif

namespace Corrade { namespace Utility {

template<class T> constexpr T&& forward(typename std::remove_reference<T>::type& t) noexcept {
    return static_cast<T&&>(t);
}

template<class T> constexpr T&& forward(typename std::remove_reference<T>::type&& t) noexcept {
    static_assert(!std::is_lvalue_reference<T>::value, "T can't be a lvalue reference");
    return static_cast<T&&>(t);
}

template<class T> constexpr typename std::remove_reference<T>::type&& move(T&& t) noexcept {
    return static_cast<typename std::remove_reference<T>::type&&>(t);
}

#ifndef CORRADE_MSVC2015_COMPATIBILITY
template<class T> void swap(T& a, typename std::common_type<T>::type& b) noexcept(std::is_nothrow_move_constructible<T>::value && std::is_nothrow_move_assignable<T>::value) {
    T tmp = static_cast<T&&>(a);
    a = static_cast<T&&>(b);
    b = static_cast<T&&>(tmp);
}
#else
using std::swap;
#endif

#ifndef CORRADE_MSVC2015_COMPATIBILITY
template<std::size_t size, class T> void swap(T(&a)[size], typename std::common_type<T(&)[size]>::type b) noexcept(std::is_nothrow_move_constructible<T>::value && std::is_nothrow_move_assignable<T>::value) {
    for(std::size_t i = 0; i != size; ++i) {
        T tmp = static_cast<T&&>(a[i]);
        a[i] = static_cast<T&&>(b[i]);
        b[i] = static_cast<T&&>(tmp);
    }
}
#endif

}}

#endif
#ifndef Corrade_Containers_constructHelpers_h
#define Corrade_Containers_constructHelpers_h

namespace Corrade { namespace Containers { namespace Implementation {

template<class T, class First, class ...Next> inline void construct(T& value, First&& first, Next&& ...next) {
    new(&value) T{Utility::forward<First>(first), Utility::forward<Next>(next)...};
}
template<class T> inline void construct(T& value) {
    new(&value) T();
}

#if defined(CORRADE_TARGET_GCC) && !defined(CORRADE_TARGET_CLANG) && __GNUC__ < 5
template<class T> inline void construct(T& value, const T& b) {
    new(&value) T(b);
}
template<class T> inline void construct(T& value, T&& b) {
    new(&value) T(Utility::move(b));
}
#endif

}}}

#endif
#ifndef CORRADE_ASSERT
#ifdef NDEBUG
#define CORRADE_ASSERT(condition, message, returnValue) do {} while(false)
#else
#define CORRADE_ASSERT(condition, message, returnValue) assert(condition)
#endif
#endif
#ifndef CORRADE_DEBUG_ASSERT
#define CORRADE_DEBUG_ASSERT(condition, message, returnValue)               \
    CORRADE_ASSERT(condition, message, returnValue)
#endif
#ifndef Corrade_Containers_Optional_h
#define Corrade_Containers_Optional_h

namespace Corrade { namespace Containers {

namespace Implementation {
    template<class, class> struct OptionalConverter;
}

struct NullOptT {
    struct Init{};
    constexpr explicit NullOptT(Init) {}
};

constexpr NullOptT NullOpt{NullOptT::Init{}};

template<class T> class Optional {
    public:
        typedef T Type;

        /*implicit*/ Optional(NullOptT = NullOpt) noexcept: _set{false} {}

        /*implicit*/ Optional(const T& value) noexcept(std::is_nothrow_copy_constructible<T>::value): _set{true} {
            #if defined(CORRADE_TARGET_GCC) && !defined(CORRADE_TARGET_CLANG) && __GNUC__ < 5
            Implementation::construct(_value, value);
            #else
            new(&_value) T{value};
            #endif
        }

        /*implicit*/ Optional(T&& value) noexcept(std::is_nothrow_move_constructible<T>::value): _set{true} {
            #if defined(CORRADE_TARGET_GCC) && !defined(CORRADE_TARGET_CLANG) && __GNUC__ < 5
            Implementation::construct(_value, Utility::move(value));
            #else
            new(&_value) T{Utility::move(value)};
            #endif
        }

        template<class ...Args> /*implicit*/ Optional(Corrade::InPlaceInitT, Args&&... args) noexcept(std::is_nothrow_constructible<T, Args&&...>::value): _set{true} {
            Implementation::construct(_value, Utility::forward<Args>(args)...);
        }

        template<class U, class = decltype(Implementation::OptionalConverter<T, U>::from(std::declval<const U&>()))> explicit Optional(const U& other) noexcept(std::is_nothrow_copy_constructible<T>::value): Optional{Implementation::OptionalConverter<T, U>::from(other)} {}

        template<class U, class = decltype(Implementation::OptionalConverter<T, U>::from(std::declval<U&&>()))> explicit Optional(U&& other) noexcept(std::is_nothrow_move_constructible<T>::value): Optional{Implementation::OptionalConverter<T, U>::from(Utility::move(other))} {}

        Optional(const Optional<T>& other) noexcept(std::is_nothrow_copy_constructible<T>::value);

        Optional(Optional<T>&& other) noexcept(std::is_nothrow_move_constructible<T>::value);

        Optional<T>& operator=(const Optional<T>& other) noexcept(std::is_nothrow_copy_assignable<T>::value);

        Optional<T>& operator=(Optional<T>&& other) noexcept(std::is_nothrow_move_assignable<T>::value);

        template<class U, class = decltype(Implementation::OptionalConverter<T, U>::to(std::declval<const Optional<T>&>()))> explicit operator U() const & {
            return Implementation::OptionalConverter<T, U>::to(*this);
        }

        template<class U, class = decltype(Implementation::OptionalConverter<T, U>::to(std::declval<Optional<T>&&>()))> explicit operator U() && {
            return Implementation::OptionalConverter<T, U>::to(Utility::move(*this));
        }

        Optional<T>& operator=(NullOptT) noexcept;

        ~Optional() { if(_set) _value.~T(); }

        explicit operator bool() const { return _set; }

        bool operator==(const Optional<T>& other) const {
            return (!_set && !other._set) || (_set && other._set && _value == other._value);
        }

        bool operator!=(const Optional<T>& other) const { return !operator==(other); }

        bool operator==(NullOptT) const { return !_set; }

        bool operator!=(NullOptT) const { return _set; }

        bool operator==(const T& other) const {
            return _set ? _value == other : false;
        }

        bool operator!=(const T& other) const { return !operator==(other); }

        T* operator->() {
            CORRADE_DEBUG_ASSERT(_set, "Containers::Optional: the optional is empty", &_value);
            return &_value;
        }

        const T* operator->() const {
            CORRADE_DEBUG_ASSERT(_set, "Containers::Optional: the optional is empty", &_value);
            return &_value;
        }

        T& operator*() & {
            CORRADE_DEBUG_ASSERT(_set, "Containers::Optional: the optional is empty", _value);
            return _value;
        }

        T operator*() && {
            CORRADE_DEBUG_ASSERT(_set, "Containers::Optional: the optional is empty", Utility::move(_value));
            return Utility::move(_value);
        }

        const T& operator*() const & {
            CORRADE_DEBUG_ASSERT(_set, "Containers::Optional: the optional is empty", _value);
            return _value;
        }

        template<class ...Args> T& emplace(Args&&... args);

    private:
        union {
            T _value;
            #if defined(CORRADE_TARGET_GCC) && !defined(CORRADE_TARGET_CLANG) && __GNUC__ >= 10 && __OPTIMIZE__
            volatile char _gccStopSayingThisIsMaybeUninitialized[sizeof(T)];
            #endif
        };
        bool _set;
};

template<class T> bool operator==(NullOptT, const Optional<T>& b) { return b == NullOpt; }

template<class T> bool operator!=(NullOptT, const Optional<T>& b) { return b != NullOpt; }

template<class T> bool operator==(const T& a, const Optional<T>& b) { return b == a; }

template<class T> bool operator!=(const T& a, const Optional<T>& b) { return b != a; }

namespace Implementation {
    template<class T> struct DeducedOptionalConverter { typedef T Type; };
}

template<class T> inline
Optional<typename Implementation::DeducedOptionalConverter<typename std::decay<T>::type>::Type>
optional(T&& value) {
    return Optional<typename std::decay<T>::type>{Utility::forward<T>(value)};
}

template<class T, class ...Args> inline Optional<T> optional(Args&&... args) {
    return Optional<T>{Corrade::InPlaceInit, Utility::forward<Args>(args)...};
}

template<class T> inline auto optional(T&& other) -> decltype(Implementation::DeducedOptionalConverter<typename std::decay<T>::type>::from(Utility::forward<T>(other))) {
    return Implementation::DeducedOptionalConverter<typename std::decay<T>::type>::from(Utility::forward<T>(other));
}

template<class T> Optional<T>::Optional(const Optional<T>& other) noexcept(std::is_nothrow_copy_constructible<T>::value): _set(other._set) {
    if(_set)
        #if defined(CORRADE_TARGET_GCC) && !defined(CORRADE_TARGET_CLANG) && __GNUC__ < 5
        Implementation::construct(_value, other._value);
        #else
        new(&_value) T{other._value};
        #endif
}

template<class T> Optional<T>::Optional(Optional<T>&& other) noexcept(std::is_nothrow_move_constructible<T>::value): _set(other._set) {
    if(_set)
        #if defined(CORRADE_TARGET_GCC) && !defined(CORRADE_TARGET_CLANG) && __GNUC__ < 5
        Implementation::construct(_value, Utility::move(other._value));
        #else
        new(&_value) T{Utility::move(other._value)};
        #endif
}

template<class T> Optional<T>& Optional<T>::operator=(const Optional<T>& other) noexcept(std::is_nothrow_copy_assignable<T>::value) {
    if(_set) _value.~T();
    if((_set = other._set))
        #if defined(CORRADE_TARGET_GCC) && !defined(CORRADE_TARGET_CLANG) && __GNUC__ < 5
        Implementation::construct(_value, other._value);
        #else
        new(&_value) T{other._value};
        #endif
    return *this;
}

template<class T> Optional<T>& Optional<T>::operator=(Optional<T>&& other) noexcept(std::is_nothrow_move_assignable<T>::value) {
    if(_set && other._set) {
        using Utility::swap;
        swap(other._value, _value);
    } else {
        if(_set) _value.~T();
        if((_set = other._set))
            #if defined(CORRADE_TARGET_GCC) && !defined(CORRADE_TARGET_CLANG) && __GNUC__ < 5
            Implementation::construct(_value, Utility::move(other._value));
            #else
            new(&_value) T{Utility::move(other._value)};
            #endif
    }
    return *this;
}

template<class T> Optional<T>& Optional<T>::operator=(NullOptT) noexcept {
    if(_set) _value.~T();
    _set = false;
    return *this;
}

template<class T> template<class ...Args> T& Optional<T>::emplace(Args&&... args) {
    if(_set) _value.~T();
    Implementation::construct<T>(_value, Utility::forward<Args>(args)...);
    _set = true;
    return _value;
}

}}

#endif
#ifdef CORRADE_OPTIONAL_STL_COMPATIBILITY
#include <optional>
#ifndef Corrade_Containers_OptionalStl_h
#define Corrade_Containers_OptionalStl_h

namespace Corrade { namespace Containers { namespace Implementation {

template<class T> struct OptionalConverter<T, std::optional<T>> {
    static Optional<T> from(const std::optional<T>& other) {
        return other ? Optional<T>{*other} : Containers::NullOpt;
    }

    static Optional<T> from(std::optional<T>&& other) {
        return other ? Optional<T>{*Utility::move(other)} : Containers::NullOpt;
    }

    static std::optional<T> to(const Optional<T>& other) {
        return other ? std::optional<T>{*other} : std::nullopt;
    }

    static std::optional<T> to(Optional<T>&& other) {
        return other ? std::optional<T>{*Utility::move(other)} : std::nullopt;
    }
};

template<class T> struct DeducedOptionalConverter<std::optional<T>>: OptionalConverter<T, std::optional<T>> {};

}}}

#endif
#endif
