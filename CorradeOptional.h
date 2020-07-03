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

    Generated from Corrade v2020.06-0-g61d1b58c (2020-06-27), 358 / 2883 LoC
*/

/*
    This file is part of Corrade.

    Copyright © 2007, 2008, 2009, 2010, 2011, 2012, 2013, 2014, 2015, 2016,
                2017, 2018, 2019, 2020 Vladimír Vondruš <mosra@centrum.cz>

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
#include <utility>
#if !defined(CORRADE_ASSERT) && !defined(NDEBUG)
#include <cassert>
#endif

#ifdef __GNUC__
#define CORRADE_TARGET_GCC
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
#ifndef Corrade_Containers_constructHelpers_h
#define Corrade_Containers_constructHelpers_h

namespace Corrade { namespace Containers { namespace Implementation {

template<class T, class First, class ...Next> inline void construct(T& value, First&& first, Next&& ...next) {
    new(&value) T{std::forward<First>(first), std::forward<Next>(next)...};
}
template<class T> inline void construct(T& value) {
    new(&value) T();
}

#if defined(CORRADE_TARGET_GCC) && __GNUC__ < 5
template<class T> inline void construct(T& value, T&& b) {
    new(&value) T(std::move(b));
}
#endif

}}}

#endif
#ifndef CORRADE_ASSERT
#ifdef NDEBUG
#define CORRADE_ASSERT(condition, message, returnValue) do {} while(0)
#else
#define CORRADE_ASSERT(condition, message, returnValue) assert(condition)
#endif
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
        /*implicit*/ Optional(NullOptT = NullOpt) noexcept: _set{false} {}

        /*implicit*/ Optional(const T& value) noexcept(std::is_nothrow_copy_assignable<T>::value): _set{true} {
            new(&_value) T{value};
        }

        /*implicit*/ Optional(T&& value) noexcept(std::is_nothrow_move_assignable<T>::value): _set{true} {
            new(&_value) T{std::move(value)};
        }

        template<class ...Args> /*implicit*/ Optional(InPlaceInitT, Args&&... args) noexcept(std::is_nothrow_constructible<T, Args&&...>::value): _set{true} {
            Implementation::construct(_value, std::forward<Args>(args)...);
        }

        template<class U, class = decltype(Implementation::OptionalConverter<T, U>::from(std::declval<const U&>()))> explicit Optional(const U& other) noexcept(std::is_nothrow_copy_constructible<T>::value): Optional{Implementation::OptionalConverter<T, U>::from(other)} {}

        template<class U, class = decltype(Implementation::OptionalConverter<T, U>::from(std::declval<U&&>()))> explicit Optional(U&& other) noexcept(std::is_nothrow_move_constructible<T>::value): Optional{Implementation::OptionalConverter<T, U>::from(std::move(other))} {}

        Optional(const Optional<T>& other) noexcept(std::is_nothrow_copy_constructible<T>::value);

        Optional(Optional<T>&& other) noexcept(std::is_nothrow_move_constructible<T>::value);

        Optional<T>& operator=(const Optional<T>& other) noexcept(std::is_nothrow_copy_assignable<T>::value);

        Optional<T>& operator=(Optional<T>&& other) noexcept(std::is_nothrow_move_assignable<T>::value);

        template<class U, class = decltype(Implementation::OptionalConverter<T, U>::to(std::declval<const Optional<T>&>()))> explicit operator U() const & {
            return Implementation::OptionalConverter<T, U>::to(*this);
        }

        template<class U, class = decltype(Implementation::OptionalConverter<T, U>::to(std::declval<Optional<T>&&>()))> explicit operator U() && {
            return Implementation::OptionalConverter<T, U>::to(std::move(*this));
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
            CORRADE_ASSERT(_set, "Containers::Optional: the optional is empty", &_value);
            return &_value;
        }

        const T* operator->() const {
            CORRADE_ASSERT(_set, "Containers::Optional: the optional is empty", &_value);
            return &_value;
        }

        T& operator*() & {
            CORRADE_ASSERT(_set, "Containers::Optional: the optional is empty", _value);
            return _value;
        }

        T&& operator*() && {
            CORRADE_ASSERT(_set, "Containers::Optional: the optional is empty", std::move(_value));
            return std::move(_value);
        }

        const T& operator*() const & {
            CORRADE_ASSERT(_set, "Containers::Optional: the optional is empty", _value);
            return _value;
        }

        #if !defined(__GNUC__) || defined(__clang__) || __GNUC__ > 4
        const T&& operator*() const && {
            CORRADE_ASSERT(_set, "Containers::Optional: the optional is empty", std::move(_value));
            return std::move(_value);
        }
        #endif

        template<class ...Args> T& emplace(Args&&... args);

    private:
        union {
            T _value;
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
    return Optional<typename std::decay<T>::type>{std::forward<T>(value)};
}

template<class T, class ...Args> inline Optional<T> optional(Args&&... args) {
    return Optional<T>{InPlaceInit, std::forward<Args>(args)...};
}

template<class T> inline auto optional(T&& other) -> decltype(Implementation::DeducedOptionalConverter<typename std::decay<T>::type>::from(std::forward<T>(other))) {
    return Implementation::DeducedOptionalConverter<typename std::decay<T>::type>::from(std::forward<T>(other));
}

template<class T> Optional<T>::Optional(const Optional<T>& other) noexcept(std::is_nothrow_copy_constructible<T>::value): _set(other._set) {
    if(_set) new(&_value) T{other._value};
}

template<class T> Optional<T>::Optional(Optional<T>&& other) noexcept(std::is_nothrow_move_constructible<T>::value): _set(other._set) {
    if(_set) new(&_value) T{std::move(other._value)};
}

template<class T> Optional<T>& Optional<T>::operator=(const Optional<T>& other) noexcept(std::is_nothrow_copy_assignable<T>::value) {
    if(_set) _value.~T();
    if((_set = other._set)) new(&_value) T{other._value};
    return *this;
}

template<class T> Optional<T>& Optional<T>::operator=(Optional<T>&& other) noexcept(std::is_nothrow_move_assignable<T>::value) {
    if(_set && other._set) {
        using std::swap;
        swap(other._value, _value);
    } else {
        if(_set) _value.~T();
        if((_set = other._set)) new(&_value) T{std::move(other._value)};
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
    _set = true;
    Implementation::construct<T>(_value, std::forward<Args>(args)...);
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
        return other ? Optional<T>{*std::move(other)} : Containers::NullOpt;
    }

    static std::optional<T> to(const Optional<T>& other) {
        return other ? std::optional<T>{*other} : std::nullopt;
    }

    static std::optional<T> to(Optional<T>&& other) {
        return other ? std::optional<T>{*std::move(other)} : std::nullopt;
    }
};

template<class T> struct DeducedOptionalConverter<std::optional<T>>: OptionalConverter<T, std::optional<T>> {};

}}}

#endif
#endif
