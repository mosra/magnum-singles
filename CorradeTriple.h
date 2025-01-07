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

    Structured bindings on C++17 are opt-in due to reliance on a potentially
    heavy STL header --- `#define CORRADE_STRUCTURED_BINDINGS` before including
    the file. The STL compatibility bits are included as well --- opt-in with
    `#define CORRADE_TRIPLE_STL_COMPATIBILITY` before including the file.
    Including it multiple times with different macros defined works too.

    v2020.06-1846-gc4cdf (2025-01-07)
    -   Non-const C++17 structured bindings are now constexpr as well
    -   Structured bindings of const types now work even w/o <utility>
    v2020.06-1687-g6b5f (2024-06-29)
    -   Added explicit conversion constructors
    -   Structured bindings on C++17
    v2020.06-1502-g147e (2023-09-11)
    -   Fixes to the Utility::swap() helper to avoid ambiguity with std::swap()
    v2020.06-1454-gfc3b7 (2023-08-27)
    -   Initial release

    Generated from Corrade v2020.06-1846-gc4cdf (2025-01-07), 476 / 1746 LoC
*/

/*
    This file is part of Corrade.

    Copyright © 2007, 2008, 2009, 2010, 2011, 2012, 2013, 2014, 2015, 2016,
                2017, 2018, 2019, 2020, 2021, 2022, 2023, 2024, 2025
              Vladimír Vondruš <mosra@centrum.cz>
    Copyright © 2022, 2023 Stanislaw Halik <sthalik@misaki.pl>

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

#include <cstddef>
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
#ifdef _MSC_VER
#ifdef _MSVC_LANG
#define CORRADE_CXX_STANDARD _MSVC_LANG
#else
#define CORRADE_CXX_STANDARD 201103L
#endif
#else
#define CORRADE_CXX_STANDARD __cplusplus
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
#if CORRADE_CXX_STANDARD >= 201402 && !defined(CORRADE_MSVC2015_COMPATIBILITY)
#define CORRADE_CONSTEXPR14 constexpr
#else
#define CORRADE_CONSTEXPR14
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

        template<class OtherF, class OtherS, class OtherT, class = typename std::enable_if<std::is_constructible<F, const OtherF&>::value && std::is_constructible<S, const OtherS&>::value && std::is_constructible<T, const OtherT&>::value>::type> constexpr explicit Triple(const Triple<OtherF, OtherS, OtherT>& other) noexcept(std::is_nothrow_constructible<F, const OtherF&>::value && std::is_nothrow_constructible<S, const OtherS&>::value && std::is_nothrow_constructible<T, const OtherT&>::value):
            #if defined(CORRADE_TARGET_GCC) && !defined(CORRADE_TARGET_CLANG) && __GNUC__ < 5
            _first(F(other._first)), _second(S(other._second)), _third(T(other._third))
            #else
            _first{F(other._first)}, _second{S(other._second)}, _third{T(other._third)}
            #endif
            {}

        template<class OtherF, class OtherS, class OtherT, class = typename std::enable_if<std::is_constructible<F, OtherF&&>::value && std::is_constructible<S, OtherS&&>::value && std::is_constructible<T, OtherT&&>::value>::type> constexpr explicit Triple(Triple<OtherF, OtherS, OtherT>&& other) noexcept(std::is_nothrow_constructible<F, OtherF&&>::value && std::is_nothrow_constructible<S, OtherS&&>::value && std::is_nothrow_constructible<T, OtherT&&>::value):
            #if defined(CORRADE_TARGET_GCC) && !defined(CORRADE_TARGET_CLANG) && __GNUC__ < 5
            _first(F(Utility::move(other._first))), _second(S(Utility::move(other._second))), _third(T(Utility::move(other._third)))
            #else
            _first{F(Utility::move(other._first))}, _second{S(Utility::move(other._second))}, _third{T(Utility::move(other._third))}
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

        CORRADE_CONSTEXPR14 F& first() & { return _first; }
        CORRADE_CONSTEXPR14 F first() && { return Utility::move(_first); }
        constexpr const F& first() const & { return _first; }

        CORRADE_CONSTEXPR14 S& second() & { return _second; }
        CORRADE_CONSTEXPR14 S second() && { return Utility::move(_second); }
        constexpr const S& second() const & { return _second; }

        CORRADE_CONSTEXPR14 T& third() & { return _third; }
        CORRADE_CONSTEXPR14 T third() && { return Utility::move(_third); }
        constexpr const T& third() const & { return _third; }

    private:
        template<class, class, class> friend class Triple;

        #if CORRADE_CXX_STANDARD > 201402
        template<std::size_t index, typename std::enable_if<index == 0, F>::type* = nullptr> constexpr friend const F& get(const Triple<F, S, T>& value) {
            return value._first;
        }
        template<std::size_t index, typename std::enable_if<index == 0, F>::type* = nullptr> CORRADE_CONSTEXPR14 friend F& get(Triple<F, S, T>& value) {
            return value._first;
        }
        template<std::size_t index, typename std::enable_if<index == 0, F>::type* = nullptr> CORRADE_CONSTEXPR14 friend F&& get(Triple<F, S, T>&& value) {
            return Utility::move(value._first);
        }
        template<std::size_t index, typename std::enable_if<index == 1, S>::type* = nullptr> constexpr friend const S& get(const Triple<F, S, T>& value) {
            return value._second;
        }
        template<std::size_t index, typename std::enable_if<index == 1, S>::type* = nullptr> CORRADE_CONSTEXPR14 friend S& get(Triple<F, S, T>& value) {
            return value._second;
        }
        template<std::size_t index, typename std::enable_if<index == 1, S>::type* = nullptr> CORRADE_CONSTEXPR14 friend S&& get(Triple<F, S, T>&& value) {
            return Utility::move(value._second);
        }
        template<std::size_t index, typename std::enable_if<index == 2, T>::type* = nullptr> constexpr friend const T& get(const Triple<F, S, T>& value) {
            return value._third;
        }
        template<std::size_t index, typename std::enable_if<index == 2, T>::type* = nullptr> CORRADE_CONSTEXPR14 friend T& get(Triple<F, S, T>& value) {
            return value._third;
        }
        template<std::size_t index, typename std::enable_if<index == 2, T>::type* = nullptr> CORRADE_CONSTEXPR14 friend T&& get(Triple<F, S, T>&& value) {
            return Utility::move(value._third);
        }
        #endif

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
#ifdef CORRADE_STRUCTURED_BINDINGS
#if CORRADE_CXX_STANDARD >= 202002
#include <version>
#else
#include <ciso646>
#endif
#ifdef _LIBCPP_VERSION
#define CORRADE_TARGET_LIBCXX
#elif defined(_CPPLIB_VER)
#define CORRADE_TARGET_DINKUMWARE
#elif defined(__GLIBCXX__)
#define CORRADE_TARGET_LIBSTDCXX
#elif defined(__has_include)
    #if __has_include(<bits/c++config.h>)
        #include <bits/c++config.h>
        #ifdef __GLIBCXX__
        #define CORRADE_TARGET_LIBSTDCXX
        #endif
    #endif
#elif defined(__GNUC__) && !defined(__clang__) && __GNUC__ < 5
#define CORRADE_TARGET_LIBSTDCXX
#else
#endif
#ifndef Corrade_Utility_StlForwardTupleSizeElement_h
#define Corrade_Utility_StlForwardTupleSizeElement_h

#ifdef CORRADE_TARGET_LIBCXX
    _LIBCPP_BEGIN_NAMESPACE_STD
#elif defined(CORRADE_TARGET_LIBSTDCXX)
    #include <bits/c++config.h>
    namespace std _GLIBCXX_VISIBILITY(default) { _GLIBCXX_BEGIN_NAMESPACE_VERSION
#elif defined(CORRADE_TARGET_DINKUMWARE)
    _STD_BEGIN
#endif

#if defined(CORRADE_TARGET_LIBCXX) || defined(CORRADE_TARGET_LIBSTDCXX) || defined(CORRADE_TARGET_DINKUMWARE)
    template<size_t, class> struct tuple_element;
    template<class> struct tuple_size;
#else
    #include <utility>
#endif

#ifdef CORRADE_TARGET_LIBCXX
    _LIBCPP_END_NAMESPACE_STD
#elif defined(CORRADE_TARGET_LIBSTDCXX)
    _GLIBCXX_END_NAMESPACE_VERSION }
#elif defined CORRADE_TARGET_MSVC
    _STD_END
#endif

#endif
namespace std {

#ifndef Corrade_Containers_StructuredBindings_Triple_h
#define Corrade_Containers_StructuredBindings_Triple_h
template<class F, class S, class T> struct tuple_size<Corrade::Containers::Triple<F, S, T>>: integral_constant<size_t, 3> {};
template<class F, class S, class T> struct tuple_size<const Corrade::Containers::Triple<F, S, T>>: integral_constant<size_t, 3> {};
template<class F, class S, class T> struct tuple_element<0, Corrade::Containers::Triple<F, S, T>> { typedef F type; };
template<class F, class S, class T> struct tuple_element<1, Corrade::Containers::Triple<F, S, T>> { typedef S type; };
template<class F, class S, class T> struct tuple_element<2, Corrade::Containers::Triple<F, S, T>> { typedef T type; };
template<size_t i, class F, class S, class T> struct tuple_element<i, const Corrade::Containers::Triple<F, S, T>> {
    typedef const typename tuple_element<i, Corrade::Containers::Triple<F, S, T>>::type type;
};
#endif

}
#endif
