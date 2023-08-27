/*
    Corrade::Containers::Pair
        — a lightweight alternative to std::pair

    https://doc.magnum.graphics/corrade/classCorrade_1_1Containers_1_1Pair.html

    This is a single-header library generated from the Corrade project. With
    the goal being easy integration, it's deliberately free of all comments
    to keep the file size small. More info, detailed changelogs and docs here:

    -   Project homepage — https://magnum.graphics/corrade/
    -   Documentation — https://doc.magnum.graphics/corrade/
    -   GitHub project page — https://github.com/mosra/corrade
    -   GitHub Singles repository — https://github.com/mosra/magnum-singles

    v2020.06-1454-gfc3b7 (2023-08-27)
    -   Initial release

    Generated from Corrade v2020.06-1454-gfc3b7 (2023-08-27), 284 / 1721 LoC
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

#include <type_traits>

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

template<class T> void swap(T& a, T& b) noexcept(std::is_nothrow_move_constructible<T>::value && std::is_nothrow_move_assignable<T>::value) {
    T tmp = static_cast<T&&>(a);
    a = static_cast<T&&>(b);
    b = static_cast<T&&>(tmp);
}
template<class T> void swap(T*& a, T*& b) noexcept {
    T* tmp = a;
    a = b;
    b = tmp;
}

}}

#endif
#ifndef Corrade_Containers_Pair_h
#define Corrade_Containers_Pair_h

namespace Corrade { namespace Containers {

namespace Implementation {
    template<class, class, class> struct PairConverter;
}

template<class F, class S> class Pair {
    static_assert(!std::is_lvalue_reference<F>::value && !std::is_lvalue_reference<S>::value, "use a Reference<T> to store a T& in a Pair");

    public:
        typedef F FirstType;
        typedef S SecondType;

        #ifndef CORRADE_MSVC2015_COMPATIBILITY
        constexpr
        #endif
        explicit Pair(Corrade::DefaultInitT) noexcept(std::is_nothrow_constructible<F>::value && std::is_nothrow_constructible<S>::value) {}

        constexpr explicit Pair(Corrade::ValueInitT) noexcept(std::is_nothrow_constructible<F>::value && std::is_nothrow_constructible<S>::value):
            _first(), _second() {}

        template<class F_ = F, class = typename std::enable_if<std::is_standard_layout<F_>::value && std::is_standard_layout<S>::value && std::is_trivial<F_>::value && std::is_trivial<S>::value>::type> explicit Pair(Corrade::NoInitT) noexcept {}
        template<class F_ = F, class S_ = S, class = typename std::enable_if<std::is_constructible<F_, Corrade::NoInitT>::value && std::is_constructible<S_, Corrade::NoInitT>::value>::type> explicit Pair(Corrade::NoInitT) noexcept(std::is_nothrow_constructible<F, Corrade::NoInitT>::value && std::is_nothrow_constructible<S, Corrade::NoInitT>::value): _first{Corrade::NoInit}, _second{Corrade::NoInit} {}

        constexpr /*implicit*/ Pair() noexcept(std::is_nothrow_constructible<F>::value && std::is_nothrow_constructible<S>::value):
            #ifdef CORRADE_MSVC2015_COMPATIBILITY
            _first{}, _second{}
            #else
            Pair{Corrade::ValueInit}
            #endif
        {}

        constexpr /*implicit*/ Pair(const F& first, const S& second) noexcept(std::is_nothrow_copy_constructible<F>::value && std::is_nothrow_copy_constructible<S>::value):
            #if defined(CORRADE_TARGET_GCC) && !defined(CORRADE_TARGET_CLANG) && __GNUC__ < 5
            _first(first), _second(second)
            #else
            _first{first}, _second{second}
            #endif
            {}
        constexpr /*implicit*/ Pair(const F& first, S&& second) noexcept(std::is_nothrow_copy_constructible<F>::value && std::is_nothrow_move_constructible<S>::value):
            #if defined(CORRADE_TARGET_GCC) && !defined(CORRADE_TARGET_CLANG) && __GNUC__ < 5
            _first(first), _second(Utility::move(second))
            #else
            _first{first}, _second{Utility::move(second)}
            #endif
            {}
        constexpr /*implicit*/ Pair(F&& first, const S& second) noexcept(std::is_nothrow_move_constructible<F>::value && std::is_nothrow_copy_constructible<S>::value):
            #if defined(CORRADE_TARGET_GCC) && !defined(CORRADE_TARGET_CLANG) && __GNUC__ < 5
            _first(Utility::move(first)), _second(second)
            #else
            _first{Utility::move(first)}, _second{second}
            #endif
            {}
        constexpr /*implicit*/ Pair(F&& first, S&& second) noexcept(std::is_nothrow_move_constructible<F>::value && std::is_nothrow_move_constructible<S>::value):
            #if defined(CORRADE_TARGET_GCC) && !defined(CORRADE_TARGET_CLANG) && __GNUC__ < 5
            _first(Utility::move(first)), _second(Utility::move(second))
            #else
            _first{Utility::move(first)}, _second{Utility::move(second)}
            #endif
            {}

        template<class T, class = decltype(Implementation::PairConverter<F, S, T>::from(std::declval<const T&>()))> /*implicit*/ Pair(const T& other) noexcept(std::is_nothrow_copy_constructible<F>::value && std::is_nothrow_copy_constructible<S>::value): Pair{Implementation::PairConverter<F, S, T>::from(other)} {}

        template<class T, class = decltype(Implementation::PairConverter<F, S, T>::from(std::declval<T&&>()))> /*implicit*/ Pair(T&& other) noexcept(std::is_nothrow_move_constructible<F>::value && std::is_nothrow_move_constructible<S>::value): Pair{Implementation::PairConverter<F, S, T>::from(Utility::move(other))} {}

        template<class T, class = decltype(Implementation::PairConverter<F, S, T>::to(std::declval<const Pair<F, S>&>()))> /*implicit*/ operator T() const & {
            return Implementation::PairConverter<F, S, T>::to(*this);
        }

        template<class T, class = decltype(Implementation::PairConverter<F, S, T>::to(std::declval<Pair<F, S>&&>()))> /*implicit*/ operator T() && {
            return Implementation::PairConverter<F, S, T>::to(Utility::move(*this));
        }

        constexpr bool operator==(const Pair<F, S>& other) const {
            return _first == other._first && _second == other._second;
        }

        constexpr bool operator!=(const Pair<F, S>& other) const {
            return !operator==(other);
        }

        F& first() & { return _first; }
        F first() && { return Utility::move(_first); }
        constexpr const F& first() const & { return _first; }

        S& second() & { return _second; }
        S second() && { return Utility::move(_second); }
        constexpr const S& second() const & { return _second; }

    private:
        F _first;
        S _second;
};

template<class F, class S> constexpr Pair<typename std::decay<F>::type, typename std::decay<S>::type> pair(F&& first, S&& second) {
    return Pair<typename std::decay<F>::type, typename std::decay<S>::type>{Utility::forward<F>(first), Utility::forward<S>(second)};
}

namespace Implementation {
    template<class> struct DeducedPairConverter;
}

template<class T> inline auto pair(T&& other) -> decltype(Implementation::DeducedPairConverter<typename std::decay<T>::type>::from(Utility::forward<T>(other))) {
    return Implementation::DeducedPairConverter<typename std::decay<T>::type>::from(Utility::forward<T>(other));
}

}}

#endif
#ifdef CORRADE_PAIR_STL_COMPATIBILITY
#include <utility>
#ifndef Corrade_Containers_PairStl_h
#define Corrade_Containers_PairStl_h

namespace Corrade { namespace Containers { namespace Implementation {

template<class F, class S> struct PairConverter<F, S, std::pair<F, S>> {
    static Pair<F, S> from(const std::pair<F, S>& other) {
        return {other.first, other.second};
    }

    static Pair<F, S> from(std::pair<F, S>&& other) {
        return {Utility::move(other.first), Utility::move(other.second)};
    }

    static std::pair<F, S> to(const Pair<F, S>& other) {
        return {other.first(), other.second()};
    }

    static std::pair<F, S> to(Pair<F, S>&& other) {
        return {Utility::move(other.first()), Utility::move(other.second())};
    }
};

template<class F, class S> struct DeducedPairConverter<std::pair<F, S>>: PairConverter<F, S, std::pair<F, S>> {};

}}}

#endif
#endif
