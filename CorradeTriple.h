/*
    Corrade::Containers::Triple
        — a lightweight alternative to a three-component std::tuple

    https://doc.magnum.graphics/corrade/classCorrade_1_1Containers_1_1Triple.html

    This is a single-header library generated from the Corrade project. With
    the goal being easy integration, it's deliberately free of all comments
    to keep the file size small. More info, detailed changelogs and docs here:

    -   Project homepage — https://magnum.graphics/corrade/
    -   Documentation — https://doc.magnum.graphics/corrade/
    -   GitHub project page — https://github.com/mosra/corrade
    -   GitHub Singles repository — https://github.com/mosra/magnum-singles

    The STL compatibility bits are included as well --- opt-in by specifying
    `#define CORRADE_TRIPLE_STL_COMPATIBILITY` before including the file.
    Including it multiple times with different macros defined works too.

    v2020.06-1454-gfc3b7 (2023-08-27)
    -   Initial release

    Generated from Corrade v2020.06-1454-gfc3b7 (2023-08-27), 318 / 1738 LoC
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
#ifndef Corrade_Containers_Triple_h
#define Corrade_Containers_Triple_h

namespace Corrade { namespace Containers {

namespace Implementation {
    template<class, class, class, class> struct TripleConverter;
}

template<class F, class S, class T> class Triple {
    static_assert(!std::is_lvalue_reference<F>::value && !std::is_lvalue_reference<S>::value && !std::is_lvalue_reference<T>::value, "use a Reference<T> to store a T& in a Triple");

    public:
        typedef F FirstType;
        typedef S SecondType;
        typedef T ThirdType;

        #ifndef CORRADE_MSVC2015_COMPATIBILITY
        constexpr
        #endif
        explicit Triple(Corrade::DefaultInitT) noexcept(std::is_nothrow_constructible<F>::value && std::is_nothrow_constructible<S>::value && std::is_nothrow_constructible<T>::value) {}

        constexpr explicit Triple(Corrade::ValueInitT) noexcept(std::is_nothrow_constructible<F>::value && std::is_nothrow_constructible<S>::value && std::is_nothrow_constructible<T>::value):
            _first(), _second(), _third() {}

        template<class F_ = F, class = typename std::enable_if<std::is_standard_layout<F_>::value && std::is_standard_layout<S>::value && std::is_standard_layout<T>::value && std::is_trivial<F_>::value && std::is_trivial<S>::value && std::is_trivial<T>::value>::type> explicit Triple(Corrade::NoInitT) noexcept {}
        template<class F_ = F, class S_ = S, class = typename std::enable_if<std::is_constructible<F_, Corrade::NoInitT>::value && std::is_constructible<S_, Corrade::NoInitT>::value && std::is_constructible<T, Corrade::NoInitT>::value>::type> explicit Triple(Corrade::NoInitT) noexcept(std::is_nothrow_constructible<F, Corrade::NoInitT>::value && std::is_nothrow_constructible<S, Corrade::NoInitT>::value && std::is_nothrow_constructible<T, Corrade::NoInitT>::value): _first{Corrade::NoInit}, _second{Corrade::NoInit}, _third{Corrade::NoInit} {}

        constexpr /*implicit*/ Triple() noexcept(std::is_nothrow_constructible<F>::value && std::is_nothrow_constructible<S>::value && std::is_nothrow_constructible<T>::value):
            #ifdef CORRADE_MSVC2015_COMPATIBILITY
            _first{}, _second{}, _third{}
            #else
            Triple{Corrade::ValueInit}
            #endif
        {}

        constexpr /*implicit*/ Triple(const F& first, const S& second, const T& third) noexcept(std::is_nothrow_copy_constructible<F>::value && std::is_nothrow_copy_constructible<S>::value && std::is_nothrow_copy_constructible<T>::value):
            #if defined(CORRADE_TARGET_GCC) && !defined(CORRADE_TARGET_CLANG) && __GNUC__ < 5
            _first(first), _second(second), _third(third)
            #else
            _first{first}, _second{second}, _third{third}
            #endif
            {}
        constexpr /*implicit*/ Triple(const F& first, const S& second, T&& third) noexcept(std::is_nothrow_copy_constructible<F>::value && std::is_nothrow_copy_constructible<S>::value && std::is_nothrow_move_constructible<T>::value):
            #if defined(CORRADE_TARGET_GCC) && !defined(CORRADE_TARGET_CLANG) && __GNUC__ < 5
            _first(first), _second(second), _third(Utility::move(third))
            #else
            _first{first}, _second{second}, _third{Utility::move(third)}
            #endif
            {}
        constexpr /*implicit*/ Triple(const F& first, S&& second, const T& third) noexcept(std::is_nothrow_copy_constructible<F>::value && std::is_nothrow_move_constructible<S>::value && std::is_nothrow_copy_constructible<T>::value):
            #if defined(CORRADE_TARGET_GCC) && !defined(CORRADE_TARGET_CLANG) && __GNUC__ < 5
            _first(first), _second(Utility::move(second)), _third(third)
            #else
            _first{first}, _second{Utility::move(second)}, _third{third}
            #endif
            {}
        constexpr /*implicit*/ Triple(F&& first, const S& second, const T& third) noexcept(std::is_nothrow_move_constructible<F>::value && std::is_nothrow_copy_constructible<S>::value && std::is_nothrow_copy_constructible<T>::value):
            #if defined(CORRADE_TARGET_GCC) && !defined(CORRADE_TARGET_CLANG) && __GNUC__ < 5
            _first(Utility::move(first)), _second(second), _third(third)
            #else
            _first{Utility::move(first)}, _second{second}, _third{third}
            #endif
            {}
        constexpr /*implicit*/ Triple(const F& first, S&& second, T&& third) noexcept(std::is_nothrow_copy_constructible<F>::value && std::is_nothrow_move_constructible<S>::value && std::is_nothrow_move_constructible<T>::value):
            #if defined(CORRADE_TARGET_GCC) && !defined(CORRADE_TARGET_CLANG) && __GNUC__ < 5
            _first(first), _second(Utility::move(second)), _third(Utility::move(third))
            #else
            _first{first}, _second{Utility::move(second)}, _third{Utility::move(third)}
            #endif
            {}
        constexpr /*implicit*/ Triple(F&& first, const S& second, T&& third) noexcept(std::is_nothrow_move_constructible<F>::value && std::is_nothrow_copy_constructible<S>::value && std::is_nothrow_move_constructible<T>::value):
            #if defined(CORRADE_TARGET_GCC) && !defined(CORRADE_TARGET_CLANG) && __GNUC__ < 5
            _first(Utility::move(first)), _second(second), _third(Utility::move(third))
            #else
            _first{Utility::move(first)}, _second{second}, _third{Utility::move(third)}
            #endif
            {}
        constexpr /*implicit*/ Triple(F&& first, S&& second, const T& third) noexcept(std::is_nothrow_move_constructible<F>::value && std::is_nothrow_move_constructible<S>::value && std::is_nothrow_copy_constructible<T>::value):
            #if defined(CORRADE_TARGET_GCC) && !defined(CORRADE_TARGET_CLANG) && __GNUC__ < 5
            _first(Utility::move(first)), _second(Utility::move(second)), _third(third)
            #else
            _first{Utility::move(first)}, _second{Utility::move(second)}, _third{third}
            #endif
            {}
        constexpr /*implicit*/ Triple(F&& first, S&& second, T&& third) noexcept(std::is_nothrow_move_constructible<F>::value && std::is_nothrow_move_constructible<S>::value && std::is_nothrow_move_constructible<T>::value):
            #if defined(CORRADE_TARGET_GCC) && !defined(CORRADE_TARGET_CLANG) && __GNUC__ < 5
            _first(Utility::move(first)), _second(Utility::move(second)), _third(Utility::move(third))
            #else
            _first{Utility::move(first)}, _second{Utility::move(second)}, _third{Utility::move(third)}
            #endif
            {}

        template<class U, class = decltype(Implementation::TripleConverter<F, S, T, U>::from(std::declval<const U&>()))> /*implicit*/ Triple(const U& other) noexcept(std::is_nothrow_copy_constructible<F>::value && std::is_nothrow_copy_constructible<S>::value && std::is_nothrow_copy_constructible<T>::value): Triple{Implementation::TripleConverter<F, S, T, U>::from(other)} {}

        template<class U, class = decltype(Implementation::TripleConverter<F, S, T, U>::from(std::declval<U&&>()))> /*implicit*/ Triple(U&& other) noexcept(std::is_nothrow_move_constructible<F>::value && std::is_nothrow_move_constructible<S>::value && std::is_nothrow_move_constructible<T>::value): Triple{Implementation::TripleConverter<F, S, T, U>::from(Utility::move(other))} {}

        template<class U, class = decltype(Implementation::TripleConverter<F, S, T, U>::to(std::declval<const Triple<F, S, T>&>()))> /*implicit*/ operator U() const & {
            return Implementation::TripleConverter<F, S, T, U>::to(*this);
        }

        template<class U, class = decltype(Implementation::TripleConverter<F, S, T, U>::to(std::declval<Triple<F, S, T>&&>()))> /*implicit*/ operator U() && {
            return Implementation::TripleConverter<F, S, T, U>::to(Utility::move(*this));
        }

        constexpr bool operator==(const Triple<F, S, T>& other) const {
            return _first == other._first && _second == other._second && _third == other._third;
        }

        constexpr bool operator!=(const Triple<F, S, T>& other) const {
            return !operator==(other);
        }

        F& first() & { return _first; }
        F first() && { return Utility::move(_first); }
        constexpr const F& first() const & { return _first; }

        S& second() & { return _second; }
        S second() && { return Utility::move(_second); }
        constexpr const S& second() const & { return _second; }

        T& third() & { return _third; }
        T third() && { return Utility::move(_third); }
        constexpr const T& third() const & { return _third; }

    private:
        F _first;
        S _second;
        T _third;
};

template<class F, class S, class T> constexpr Triple<typename std::decay<F>::type, typename std::decay<S>::type, typename std::decay<T>::type> triple(F&& first, S&& second, T&& third) {
    return Triple<typename std::decay<F>::type, typename std::decay<S>::type, typename std::decay<T>::type>{Utility::forward<F>(first), Utility::forward<S>(second), Utility::forward<T>(third)};
}

namespace Implementation {
    template<class> struct DeducedTripleConverter;
}

template<class T> inline auto triple(T&& other) -> decltype(Implementation::DeducedTripleConverter<typename std::decay<T>::type>::from(Utility::forward<T>(other))) {
    return Implementation::DeducedTripleConverter<typename std::decay<T>::type>::from(Utility::forward<T>(other));
}

}}

#endif
#ifdef CORRADE_TRIPLE_STL_COMPATIBILITY
#include <tuple>
#ifndef Corrade_Containers_TripleStl_h
#define Corrade_Containers_TripleStl_h

namespace Corrade { namespace Containers { namespace Implementation {

template<class F, class S, class T> struct TripleConverter<F, S, T, std::tuple<F, S, T>> {
    static Triple<F, S, T> from(const std::tuple<F, S, T>& other) {
        return {std::get<0>(other), std::get<1>(other), std::get<2>(other)};
    }

    static Triple<F, S, T> from(std::tuple<F, S, T>&& other) {
        return {Utility::move(std::get<0>(other)), Utility::move(std::get<1>(other)), Utility::move(std::get<2>(other))};
    }

    static std::tuple<F, S, T> to(const Triple<F, S, T>& other) {
        return std::tuple<F, S, T>{other.first(), other.second(), other.third()};
    }

    static std::tuple<F, S, T> to(Triple<F, S, T>&& other) {
        return std::tuple<F, S, T>{Utility::move(other.first()), Utility::move(other.second()), Utility::move(other.third())};
    }
};

template<class F, class S, class T> struct DeducedTripleConverter<std::tuple<F, S, T>>: TripleConverter<F, S, T, std::tuple<F, S, T>> {};

}}}

#endif
#endif
