/*
    Corrade::Containers::ArrayView
    Corrade::Containers::StaticArrayView
        — lightweight alternatives to std::span / gsl::span

    https://doc.magnum.graphics/corrade/classCorrade_1_1Containers_1_1ArrayView.html
    https://doc.magnum.graphics/corrade/classCorrade_1_1Containers_1_1StaticArrayView.html

    This is a single-header library generated from the Corrade project. With
    the goal being easy integration, it's deliberately free of all comments
    to keep the file size small. More info, detailed changelogs and docs here:

    -   Project homepage — https://magnum.graphics/corrade/
    -   Documentation — https://doc.magnum.graphics/corrade/
    -   GitHub project page — https://github.com/mosra/corrade
    -   GitHub Singles repository — https://github.com/mosra/magnum-singles

    Structured bindings for StaticArrayView on C++17 are opt-in due to reliance
    on a potentially heavy STL header --- `#define CORRADE_STRUCTURED_BINDINGS`
    before including the file. The STL compatibility bits are included as well
    --- opt-in with either `#define CORRADE_ARRAYVIEW_STL_COMPATIBILITY` or
    `#define CORRADE_ARRAYVIEW_STL_SPAN_COMPATIBILITY` before including the
    file. Including it multiple times with different macros defined works too.

    v2020.06-1890-g77f9f (2025-04-11)
    -   Further cleanup and unification of SFINAE code, no functional change
    v2020.06-1846-gc4cdf (2025-01-07)
    -   Added arraySize() overload for arrays as struct members
    -   Structured bindings of const types now work even w/o <utility>
    v2020.06-1687-g6b5f (2024-06-29)
    -   Structured bindings for StaticArrayView on C++17
    v2020.06-1502-g147e (2023-09-11)
    -   Fixes to the Utility::swap() helper to avoid ambiguity with std::swap()
    v2020.06-1454-gfc3b7 (2023-08-27)
    -   New exceptPrefix() API, the except() API is renamed to exceptSuffix().
        The suffix() API, which took an offset, is removed and will be
        eventually reintroduced again but taking suffix size, consistently with
        prefix() that takes prefix size.
    -   New sliceSize() API, taking a begin + size instead of begin + end
    -   Element access with operator[](), front() and back() is now
        bounds-checked with assertions
    -   The front() and back() APIs are now constexpr
    -   Convenience ArrayView2, ArrayView3, ArrayView4 aliases for
        StaticArrayView
    -   Renamed empty() to isEmpty() for consistency with other bool-returning
        APIs
    -   MSVC 2022 compatibility
    -   Compatibility of the std::span conversion code with C++20 which removes
        the <ciso646> header
    -   Removed dependency on <utility>, resulting in about ~600 preprocessed
        lines less
    v2020.06-0-g61d1b58c (2020-06-27)
    -   Conversion of const types to mutable arrays is now disabled with SFINAE
        to prevent ambiguous constructor overloads
    -   Added arrayCast() overloads from ArrayView<void> and
        ArrayView<const void>
    -   Updated std::span compatibility for libc++ 9.0, which switched away
        from a signed size type
    v2019.10-0-g162d6a7d (2019-10-24)
    -   Fixed OOB access when converting empty STL containers to ArrayView
    v2019.01-301-gefe8d740 (2019-08-05)
    -   MSVC 2019 compatibility
    -   Added except() for taking everything except last N elements
    -   Added StaticArrayView::slice() with compile-time begin and end
    v2019.01-173-ge663b49c (2019-04-30)
    -   Added ArrayView<void> as a counterpart to ArrayView<const void>
    -   Added compile-time-sized StaticArrayView::suffix()
    v2019.01-107-g80d9f347 (2019-03-23)
    -   Including <cassert> only when needed
    v2019.01-41-g39c08d7c (2019-02-18)
    -   Initial release

    Generated from Corrade v2020.06-1890-g77f9f (2025-04-11), 927 / 2031 LoC
*/

/*
    This file is part of Corrade.

    Copyright © 2007, 2008, 2009, 2010, 2011, 2012, 2013, 2014, 2015, 2016,
                2017, 2018, 2019, 2020, 2021, 2022, 2023, 2024, 2025
              Vladimír Vondruš <mosra@centrum.cz>
    Copyright © 2022 Stanislaw Halik <sthalik@misaki.pl>

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
#include <initializer_list>
#include <type_traits>
#if (!defined(CORRADE_ASSERT) || !defined(CORRADE_CONSTEXPR_ASSERT)) && !defined(NDEBUG)
#include <cassert>
#endif

#ifdef _MSC_VER
#define CORRADE_MSVC_COMPATIBILITY
#endif
#if defined(_MSC_VER) && _MSC_VER < 1910
#define CORRADE_MSVC2015_COMPATIBILITY
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

#ifndef CorradeArrayView_h
#define CorradeArrayView_h
namespace Corrade { namespace Containers {

template<std::size_t, class> class StaticArrayView;
template<class T> using ArrayView2 = StaticArrayView<2, T>;
template<class T> using ArrayView3 = StaticArrayView<3, T>;
template<class T> using ArrayView4 = StaticArrayView<4, T>;

}}
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
#ifndef CORRADE_ASSERT
#ifdef NDEBUG
#define CORRADE_ASSERT(condition, message, returnValue) do {} while(false)
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
#ifndef CORRADE_CONSTEXPR_DEBUG_ASSERT
#define CORRADE_CONSTEXPR_DEBUG_ASSERT(condition, message)                  \
    CORRADE_CONSTEXPR_ASSERT(condition, message)
#endif
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

        template<class U, typename std::enable_if<std::is_same<std::nullptr_t, U>::value, int>::type = 0> constexpr /*implicit*/ ArrayView(U) noexcept: _data{}, _size{} {}

        constexpr /*implicit*/ ArrayView() noexcept: _data{}, _size{} {}

        constexpr /*implicit*/ ArrayView(T* data, std::size_t size) noexcept: _data(data), _size(size) {}

        template<class U, std::size_t size
            , typename std::enable_if<std::is_convertible<U*, T*>::value, int>::type = 0
        > constexpr /*implicit*/ ArrayView(U(&data)[size]) noexcept: _data{data}, _size{size} {
            static_assert(sizeof(T) == sizeof(U), "type sizes are not compatible");
        }

        template<class U
            , typename std::enable_if<std::is_convertible<U*, T*>::value, int>::type = 0
        > constexpr /*implicit*/ ArrayView(ArrayView<U> view) noexcept: _data{view}, _size{view.size()} {
            static_assert(sizeof(T) == sizeof(U), "type sizes are not compatible");
        }

        template<std::size_t size, class U
            , typename std::enable_if<std::is_convertible<U*, T*>::value, int>::type = 0
        > constexpr /*implicit*/ ArrayView(StaticArrayView<size, U> view) noexcept: _data{view}, _size{size} {
            static_assert(sizeof(U) == sizeof(T), "type sizes are not compatible");
        }

        template<class U, class = decltype(Implementation::ArrayViewConverter<T, typename std::decay<U&&>::type>::from(std::declval<U&&>()))> constexpr /*implicit*/ ArrayView(U&& other) noexcept: ArrayView{Implementation::ArrayViewConverter<T, typename std::decay<U&&>::type>::from(Utility::forward<U>(other))} {}

        template<class U, class = decltype(Implementation::ArrayViewConverter<T, U>::to(std::declval<ArrayView<T>>()))> constexpr /*implicit*/ operator U() const {
            return Implementation::ArrayViewConverter<T, U>::to(*this);
        }

        #ifndef CORRADE_MSVC_COMPATIBILITY
        constexpr explicit operator bool() const { return _data; }
        #endif

        constexpr /*implicit*/ operator T*() const { return _data; }

        constexpr T* data() const { return _data; }

        constexpr std::size_t size() const { return _size; }

        constexpr bool isEmpty() const { return !_size; }

        constexpr T* begin() const { return _data; }
        constexpr T* cbegin() const { return _data; }

        constexpr T* end() const { return _data+_size; }
        constexpr T* cend() const { return _data+_size; }

        constexpr T& front() const;

        constexpr T& back() const;

        template<class U, typename std::enable_if<std::is_convertible<U, std::size_t>::value, int>::type = 0> constexpr T& operator[](U i) const;

        constexpr ArrayView<T> slice(T* begin, T* end) const;

        constexpr ArrayView<T> slice(std::size_t begin, std::size_t end) const;

        template<class U, typename std::enable_if<std::is_convertible<U, T*>::value && !std::is_convertible<U, std::size_t>::value, int>::type = 0> constexpr ArrayView<T> sliceSize(U begin, std::size_t size) const {
            return slice(begin, begin + size);
        }

        constexpr ArrayView<T> sliceSize(std::size_t begin, std::size_t size) const {
            return slice(begin, begin + size);
        }

        template<std::size_t size_, class U, typename std::enable_if<std::is_convertible<U, T*>::value && !std::is_convertible<U, std::size_t>::value, int>::type = 0> constexpr StaticArrayView<size_, T> slice(U begin) const;

        template<std::size_t size_> constexpr StaticArrayView<size_, T> slice(std::size_t begin) const;

        template<std::size_t begin_, std::size_t end_> constexpr StaticArrayView<end_ - begin_, T> slice() const;

        template<std::size_t begin_, std::size_t size_> constexpr StaticArrayView<size_, T> sliceSize() const {
            return slice<begin_, begin_ + size_>();
        }

        template<class U, typename std::enable_if<std::is_convertible<U, T*>::value && !std::is_convertible<U, std::size_t>::value, int>::type = 0> constexpr ArrayView<T> prefix(U end) const {
            return static_cast<T*>(end) ? slice(_data, end) : nullptr;
        }

        constexpr ArrayView<T> suffix(T* begin) const {
            return _data && !begin ? nullptr : slice(begin, _data + _size);
        }

        constexpr ArrayView<T> prefix(std::size_t size) const {
            return slice(0, size);
        }

        template<std::size_t size_> constexpr StaticArrayView<size_, T> prefix() const {
            return slice<0, size_>();
        }

        template<std::size_t size_> constexpr StaticArrayView<size_, T> suffix() const {
            return slice<size_>(_size - size_);
        }

        constexpr ArrayView<T> exceptPrefix(std::size_t size) const {
            return slice(size, _size);
        }

        constexpr ArrayView<T> exceptSuffix(std::size_t size) const {
            return slice(0, _size - size);
        }

    private:
        T* _data;
        std::size_t _size;
};

template<> class ArrayView<void> {
    public:
        typedef void Type;

        template<class U, typename std::enable_if<std::is_same<std::nullptr_t, U>::value, int>::type = 0> constexpr /*implicit*/ ArrayView(U) noexcept: _data{}, _size{} {}
        constexpr /*implicit*/ ArrayView() noexcept: _data{}, _size{} {}

        constexpr /*implicit*/ ArrayView(void* data, std::size_t size) noexcept: _data(data), _size(size) {}

        template<class T> constexpr /*implicit*/ ArrayView(T* data, std::size_t size) noexcept: _data(data), _size(size*sizeof(T)) {}

        template<class T, std::size_t size
            , typename std::enable_if<!std::is_const<T>::value, int>::type = 0
        > constexpr /*implicit*/ ArrayView(T(&data)[size]) noexcept: _data(data), _size(size*sizeof(T)) {}

        template<class T
            , typename std::enable_if<!std::is_const<T>::value, int>::type = 0
        > constexpr /*implicit*/ ArrayView(ArrayView<T> array) noexcept: _data(array), _size(array.size()*sizeof(T)) {}

        template<std::size_t size, class T
            , typename std::enable_if<!std::is_const<T>::value, int>::type = 0
        > constexpr /*implicit*/ ArrayView(const StaticArrayView<size, T>& array) noexcept: _data{array}, _size{size*sizeof(T)} {}

        template<class T, class = decltype(Implementation::ErasedArrayViewConverter<typename std::decay<T&&>::type>::from(std::declval<T&&>()))> constexpr /*implicit*/ ArrayView(T&& other) noexcept: ArrayView{Implementation::ErasedArrayViewConverter<typename std::decay<T&&>::type>::from(other)} {}

        #ifndef CORRADE_MSVC_COMPATIBILITY
        constexpr explicit operator bool() const { return _data; }
        #endif

        constexpr /*implicit*/ operator void*() const { return _data; }

        constexpr void* data() const { return _data; }

        constexpr std::size_t size() const { return _size; }

        constexpr bool isEmpty() const { return !_size; }

    private:
        void* _data;
        std::size_t _size;
};

template<> class ArrayView<const void> {
    public:
        typedef const void Type;

        template<class U, typename std::enable_if<std::is_same<std::nullptr_t, U>::value, int>::type = 0> constexpr /*implicit*/ ArrayView(U) noexcept: _data{}, _size{} {}
        constexpr /*implicit*/ ArrayView() noexcept: _data{}, _size{} {}

        constexpr /*implicit*/ ArrayView(const void* data, std::size_t size) noexcept: _data(data), _size(size) {}

        template<class T> constexpr /*implicit*/ ArrayView(const T* data, std::size_t size) noexcept: _data(data), _size(size*sizeof(T)) {}

        template<class T, std::size_t size> constexpr /*implicit*/ ArrayView(T(&data)[size]) noexcept: _data(data), _size(size*sizeof(T)) {}

        constexpr /*implicit*/ ArrayView(ArrayView<void> array) noexcept: _data{array}, _size{array.size()} {}

        template<class T> constexpr /*implicit*/ ArrayView(ArrayView<T> array) noexcept: _data(array), _size(array.size()*sizeof(T)) {}

        template<std::size_t size, class T> constexpr /*implicit*/ ArrayView(const StaticArrayView<size, T>& array) noexcept: _data{array}, _size{size*sizeof(T)} {}

        template<class T, class = decltype(Implementation::ErasedArrayViewConverter<const T>::from(std::declval<const T&>()))> constexpr /*implicit*/ ArrayView(const T& other) noexcept: ArrayView{Implementation::ErasedArrayViewConverter<const T>::from(other)} {}

        #ifndef CORRADE_MSVC_COMPATIBILITY
        constexpr explicit operator bool() const { return _data; }
        #endif

        constexpr /*implicit*/ operator const void*() const { return _data; }

        constexpr const void* data() const { return _data; }

        constexpr std::size_t size() const { return _size; }

        constexpr bool isEmpty() const { return !_size; }

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

template<class T> ArrayView<const T> arrayView(std::initializer_list<T> list) {
    return ArrayView<const T>{list.begin(), list.size()};
}

template<std::size_t size, class T> constexpr ArrayView<T> arrayView(StaticArrayView<size, T> view) {
    return ArrayView<T>{view};
}

template<class T> constexpr ArrayView<T> arrayView(ArrayView<T> view) {
    return view;
}

template<class T, class U = decltype(Implementation::ErasedArrayViewConverter<typename std::remove_reference<T&&>::type>::from(std::declval<T&&>()))> constexpr U arrayView(T&& other) {
    return Implementation::ErasedArrayViewConverter<typename std::remove_reference<T&&>::type>::from(Utility::forward<T>(other));
}

template<class U, class T> ArrayView<U> arrayCast(ArrayView<T> view) {
    static_assert(std::is_standard_layout<T>::value, "the source type is not standard layout");
    static_assert(std::is_standard_layout<U>::value, "the target type is not standard layout");
    const std::size_t size = view.size()*sizeof(T)/sizeof(U);
    CORRADE_ASSERT(size*sizeof(U) == view.size()*sizeof(T),
        "Containers::arrayCast(): can't reinterpret" << view.size() << sizeof(T) << Utility::Debug::nospace << "-byte items into a" << sizeof(U) << Utility::Debug::nospace << "-byte type", {});
    return {reinterpret_cast<U*>(view.begin()), size};
}

template<class U> ArrayView<U> arrayCast(ArrayView<const void> view) {
    static_assert(std::is_standard_layout<U>::value, "the target type is not standard layout");
    const std::size_t size = view.size()/sizeof(U);
    CORRADE_ASSERT(size*sizeof(U) == view.size(),
        "Containers::arrayCast(): can't reinterpret" << view.size() << "bytes into a" << sizeof(U) << Utility::Debug::nospace << "-byte type", {});
    return {reinterpret_cast<U*>(view.data()), size};
}

template<class U> ArrayView<U> arrayCast(ArrayView<void> view) {
    auto out = arrayCast<const U>(ArrayView<const void>{view});
    return ArrayView<U>{const_cast<U*>(out.data()), out.size()};
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

template<std::size_t size_, class T, class U> constexpr std::size_t arraySize(U(T::*)[size_]) {
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

        template<class U, class = U, typename std::enable_if<std::is_same<std::nullptr_t, U>::value, int>::type = 0> constexpr /*implicit*/ StaticArrayView(U) noexcept: _data{} {}
        constexpr /*implicit*/ StaticArrayView() noexcept: _data{} {}

        template<class U, typename std::enable_if<std::is_pointer<U>::value && !std::is_same<U, T(&)[size_]>::value, int>::type = 0> constexpr explicit StaticArrayView(U data)
        noexcept: _data(data) {}

        template<class U
            , typename std::enable_if<std::is_convertible<U*, T*>::value, int>::type = 0
        > constexpr /*implicit*/ StaticArrayView(U(&data)[size_]) noexcept: _data{data} {
            static_assert(sizeof(T) == sizeof(U), "type sizes are not compatible");
        }

        template<class U
            , typename std::enable_if<std::is_convertible<U*, T*>::value, int>::type = 0
        > constexpr /*implicit*/ StaticArrayView(StaticArrayView<size_, U> view) noexcept: _data{view} {
            static_assert(sizeof(T) == sizeof(U), "type sizes are not compatible");
        }

        template<class U, class = decltype(Implementation::StaticArrayViewConverter<size_, T, typename std::decay<U&&>::type>::from(std::declval<U&&>()))> constexpr /*implicit*/ StaticArrayView(U&& other) noexcept: StaticArrayView{Implementation::StaticArrayViewConverter<size_, T, typename std::decay<U&&>::type>::from(Utility::forward<U>(other))} {}

        template<class U, class = decltype(Implementation::StaticArrayViewConverter<size_, T, U>::to(std::declval<StaticArrayView<size_, T>>()))> constexpr /*implicit*/ operator U() const {
            return Implementation::StaticArrayViewConverter<size_, T, U>::to(*this);
        }

        #ifndef CORRADE_MSVC_COMPATIBILITY
        constexpr explicit operator bool() const { return _data; }
        #endif

        constexpr /*implicit*/ operator T*() const { return _data; }

        constexpr T* data() const { return _data; }

        constexpr std::size_t size() const { return size_; }

        constexpr bool isEmpty() const { return !size_; }

        constexpr T* begin() const { return _data; }
        constexpr T* cbegin() const { return _data; }

        constexpr T* end() const { return _data + size_; }
        constexpr T* cend() const { return _data + size_; }

        constexpr T& front() const;

        constexpr T& back() const;

        template<class U, typename std::enable_if<std::is_convertible<U, std::size_t>::value, int>::type = 0> constexpr T& operator[](U i) const;

        constexpr ArrayView<T> slice(T* begin, T* end) const {
            return ArrayView<T>(*this).slice(begin, end);
        }
        constexpr ArrayView<T> slice(std::size_t begin, std::size_t end) const {
            return ArrayView<T>(*this).slice(begin, end);
        }

        template<class U, typename std::enable_if<std::is_convertible<U, T*>::value && !std::is_convertible<U, std::size_t>::value, int>::type = 0> constexpr ArrayView<T> sliceSize(U begin, std::size_t size) const {
            return ArrayView<T>(*this).sliceSize(begin, size);
        }

        constexpr ArrayView<T> sliceSize(std::size_t begin, std::size_t size) const {
            return ArrayView<T>(*this).sliceSize(begin, size);
        }

        template<std::size_t size__, class U, typename std::enable_if<std::is_convertible<U, T*>::value && !std::is_convertible<U, std::size_t>::value, int>::type = 0> constexpr StaticArrayView<size__, T> slice(U begin) const {
            return ArrayView<T>(*this).template slice<size__>(begin);
        }

        template<std::size_t size__> constexpr StaticArrayView<size__, T> slice(std::size_t begin) const {
            return ArrayView<T>(*this).template slice<size__>(begin);
        }

        template<std::size_t begin_, std::size_t end_> constexpr StaticArrayView<end_ - begin_, T> slice() const;

        template<std::size_t begin_, std::size_t size__> constexpr StaticArrayView<size__, T> sliceSize() const {
            return slice<begin_, begin_ + size__>();
        }

        template<class U, typename std::enable_if<std::is_convertible<U, T*>::value && !std::is_convertible<U, std::size_t>::value, int>::type = 0> constexpr ArrayView<T> prefix(U end) const {
            return ArrayView<T>(*this).prefix(end);
        }

        constexpr ArrayView<T> suffix(T* begin) const {
            return ArrayView<T>(*this).suffix(begin);
        }

        constexpr ArrayView<T> prefix(std::size_t size) const {
            return ArrayView<T>(*this).prefix(size);
        }

        template<std::size_t size__> constexpr StaticArrayView<size__, T> prefix() const {
            return slice<0, size__>();
        }

        constexpr ArrayView<T> exceptPrefix(std::size_t size__) const {
            return ArrayView<T>(*this).exceptPrefix(size__);
        }

        template<std::size_t size__> constexpr StaticArrayView<size_ - size__, T> exceptPrefix() const {
            return slice<size__, size_>();
        }

        constexpr ArrayView<T> exceptSuffix(std::size_t size) const {
            return ArrayView<T>(*this).exceptSuffix(size);
        }

        template<std::size_t size__> constexpr StaticArrayView<size_ - size__, T> exceptSuffix() const {
            return slice<0, size_ - size__>();
        }

    private:
        #if CORRADE_CXX_STANDARD > 201402
        template<std::size_t index> constexpr friend T& get(StaticArrayView<size_, T> value) {
            return value._data[index];
        }
        #endif

        T* _data;
};

#ifndef CORRADE_MSVC2015_COMPATIBILITY /* Multiple definitions still broken */
template<class T> using ArrayView2 = StaticArrayView<2, T>;

template<class T> using ArrayView3 = StaticArrayView<3, T>;

template<class T> using ArrayView4 = StaticArrayView<4, T>;
#endif

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
    return Implementation::ErasedStaticArrayViewConverter<typename std::remove_reference<T&&>::type>::from(Utility::forward<T>(other));
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

template<class T> constexpr T& ArrayView<T>::front() const {
    return CORRADE_CONSTEXPR_DEBUG_ASSERT(_size, "Containers::ArrayView::front(): view is empty"), _data[0];
}

template<class T> constexpr T& ArrayView<T>::back() const {
    return CORRADE_CONSTEXPR_DEBUG_ASSERT(_size, "Containers::ArrayView::back(): view is empty"), _data[_size - 1];
}

template<class T> template<class U, typename std::enable_if<std::is_convertible<U, std::size_t>::value, int>::type> constexpr T& ArrayView<T>::operator[](const U i) const {
    return CORRADE_CONSTEXPR_DEBUG_ASSERT(std::size_t(i) < _size,
        "Containers::ArrayView::operator[](): index" << i << "out of range for" << _size << "elements"), _data[i];
}

template<class T> constexpr ArrayView<T> ArrayView<T>::slice(T* begin, T* end) const {
    return CORRADE_CONSTEXPR_DEBUG_ASSERT(_data <= begin && begin <= end && end <= _data + _size,
            "Containers::ArrayView::slice(): slice ["
            << Utility::Debug::nospace << begin - _data
            << Utility::Debug::nospace << ":"
            << Utility::Debug::nospace << end - _data
            << Utility::Debug::nospace << "] out of range for" << _size
            << "elements"),
        ArrayView<T>{begin, std::size_t(end - begin)};
}

template<class T> constexpr ArrayView<T> ArrayView<T>::slice(std::size_t begin, std::size_t end) const {
    return CORRADE_CONSTEXPR_DEBUG_ASSERT(begin <= end && end <= _size,
            "Containers::ArrayView::slice(): slice ["
            << Utility::Debug::nospace << begin
            << Utility::Debug::nospace << ":"
            << Utility::Debug::nospace << end
            << Utility::Debug::nospace << "] out of range for" << _size
            << "elements"),
        ArrayView<T>{_data + begin, end - begin};
}

template<std::size_t size_, class T> constexpr T& StaticArrayView<size_, T>::front() const {
    static_assert(size_, "view is empty");
    return _data[0];
}

template<std::size_t size_, class T> constexpr T& StaticArrayView<size_, T>::back() const {
    static_assert(size_, "view is empty");
    return _data[size_ - 1];
}

template<std::size_t size_, class T> template<class U, typename std::enable_if<std::is_convertible<U, std::size_t>::value, int>::type> constexpr T& StaticArrayView<size_, T>::operator[](const U i) const {
    return CORRADE_CONSTEXPR_DEBUG_ASSERT(std::size_t(i) < size_,
        "Containers::StaticArrayView::operator[](): index" << i << "out of range for" << size_ << "elements"), _data[i];
}

template<class T> template<std::size_t size_, class U, typename std::enable_if<std::is_convertible<U, T*>::value && !std::is_convertible<U, std::size_t>::value, int>::type> constexpr StaticArrayView<size_, T> ArrayView<T>::slice(const U begin) const {
    return CORRADE_CONSTEXPR_DEBUG_ASSERT(_data <= begin && begin + size_ <= _data + _size,
            "Containers::ArrayView::slice(): slice ["
            << Utility::Debug::nospace << begin - _data
            << Utility::Debug::nospace << ":"
            << Utility::Debug::nospace << begin + size_ - _data
            << Utility::Debug::nospace << "] out of range for" << _size
            << "elements"),
        StaticArrayView<size_, T>{begin};
}

template<class T> template<std::size_t size_> constexpr StaticArrayView<size_, T> ArrayView<T>::slice(std::size_t begin) const {
    return CORRADE_CONSTEXPR_DEBUG_ASSERT(begin + size_ <= _size,
            "Containers::ArrayView::slice(): slice ["
            << Utility::Debug::nospace << begin
            << Utility::Debug::nospace << ":"
            << Utility::Debug::nospace << begin + size_
            << Utility::Debug::nospace << "] out of range for" << _size
            << "elements"),
        StaticArrayView<size_, T>{_data + begin};
}

template<class T> template<std::size_t begin_, std::size_t end_> constexpr StaticArrayView<end_ - begin_, T> ArrayView<T>::slice() const {
    static_assert(begin_ < end_, "fixed-size slice needs to have a positive size");
    return CORRADE_CONSTEXPR_DEBUG_ASSERT(end_ <= _size,
            "Containers::ArrayView::slice(): slice ["
            << Utility::Debug::nospace << begin_
            << Utility::Debug::nospace << ":"
            << Utility::Debug::nospace << end_
            << Utility::Debug::nospace << "] out of range for" << _size
            << "elements"),
        StaticArrayView<end_ - begin_, T>{_data + begin_};
}

template<std::size_t size_, class T> template<std::size_t begin_, std::size_t end_> constexpr StaticArrayView<end_ - begin_, T> StaticArrayView<size_, T>::slice() const {
    static_assert(begin_ < end_, "fixed-size slice needs to have a positive size");
    static_assert(end_ <= size_, "slice out of range");
    return StaticArrayView<end_ - begin_, T>{_data + begin_};
}

}}

#endif
#ifdef CORRADE_STRUCTURED_BINDINGS
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

#ifndef Corrade_Containers_StructuredBindings_StaticArrayView_h
#define Corrade_Containers_StructuredBindings_StaticArrayView_h
template<size_t size_, class T> struct tuple_size<Corrade::Containers::StaticArrayView<size_, T>>: integral_constant<size_t, size_> {};
template<size_t size_, class T> struct tuple_size<const Corrade::Containers::StaticArrayView<size_, T>>: integral_constant<size_t, size_> {};
template<size_t index, size_t size_, class T> struct tuple_element<index, Corrade::Containers::StaticArrayView<size_, T>> { typedef T type; };
template<size_t index, size_t size_, class T> struct tuple_element<index, const Corrade::Containers::StaticArrayView<size_, T>> { typedef const T type; };
#endif

}
#endif
#ifdef CORRADE_ARRAYVIEW_STL_COMPATIBILITY
#include <array>
#include <vector>
#ifndef Corrade_Containers_ArrayViewStl_h
#define Corrade_Containers_ArrayViewStl_h

namespace Corrade { namespace Containers { namespace Implementation {

template<std::size_t size, class T> struct ArrayViewConverter<T, std::array<T, size>> {
    constexpr static ArrayView<T> from(std::array<T, size>& other) {
        return {other.data(), other.size()};
    }
    constexpr static ArrayView<T> from(std::array<T, size>&& other) {
        return {other.data(), other.size()};
    }
};
template<std::size_t size, class T> struct ArrayViewConverter<const T, std::array<T, size>> {
    constexpr static ArrayView<const T> from(const std::array<T, size>& other) {
        return {other.data(), other.size()};
    }
};
template<std::size_t size, class T> struct ErasedArrayViewConverter<std::array<T, size>>: ArrayViewConverter<T, std::array<T, size>> {};
template<std::size_t size, class T> struct ErasedArrayViewConverter<const std::array<T, size>>: ArrayViewConverter<const T, std::array<T, size>> {};

template<class T, class Allocator> struct ArrayViewConverter<T, std::vector<T, Allocator>> {
    static ArrayView<T> from(std::vector<T, Allocator>& other) {
        return {other.data(), other.size()};
    }
    static ArrayView<T> from(std::vector<T, Allocator>&& other) {
        return {other.data(), other.size()};
    }
};
template<class T, class Allocator> struct ArrayViewConverter<const T, std::vector<T, Allocator>> {
    static ArrayView<const T> from(const std::vector<T, Allocator>& other) {
        return {other.data(), other.size()};
    }
};
template<class T, class Allocator> struct ErasedArrayViewConverter<std::vector<T, Allocator>>: ArrayViewConverter<T, std::vector<T, Allocator>> {};
template<class T, class Allocator> struct ErasedArrayViewConverter<const std::vector<T, Allocator>>: ArrayViewConverter<const T, std::vector<T, Allocator>> {};

template<std::size_t size, class T> struct StaticArrayViewConverter<size, T, std::array<T, size>> {
    constexpr static StaticArrayView<size, T> from(std::array<T, size>& other) {
        return StaticArrayView<size, T>{other.data()};
    }
};
template<std::size_t size, class T> struct StaticArrayViewConverter<size, const T, std::array<T, size>> {
    constexpr static StaticArrayView<size, const T> from(const std::array<T, size>& other) {
        return StaticArrayView<size, const T>(other.data());
    }
};
template<std::size_t size, class T> struct ErasedStaticArrayViewConverter<std::array<T, size>>: StaticArrayViewConverter<size, T, std::array<T, size>> {};
template<std::size_t size, class T> struct ErasedStaticArrayViewConverter<const std::array<T, size>>: StaticArrayViewConverter<size, const T, std::array<T, size>> {};

}}}

#endif
#endif
#ifdef CORRADE_ARRAYVIEW_STL_SPAN_COMPATIBILITY
#include <span>
#ifndef Corrade_Containers_ArrayViewStlSpan_h
#define Corrade_Containers_ArrayViewStlSpan_h

namespace Corrade { namespace Containers { namespace Implementation {

#if defined(CORRADE_TARGET_LIBCXX) && _LIBCPP_VERSION < 9000
typedef std::ptrdiff_t StlSpanSizeType;
#else
typedef std::size_t StlSpanSizeType;
#endif

template<class T> struct ArrayViewConverter<T, std::span<T>> {
    constexpr static ArrayView<T> from(std::span<T> other) {
        return {other.data(), std::size_t(other.size())};
    }
};
template<class T> struct ArrayViewConverter<T, const std::span<T>>: ArrayViewConverter<T, std::span<T>> {};
template<class T> struct ArrayViewConverter<const T, std::span<T>> {
    constexpr static ArrayView<const T> from(std::span<T> other) {
        return {other.data(), std::size_t(other.size())};
    }
};
template<class T> struct ErasedArrayViewConverter<std::span<T>>: ArrayViewConverter<T, std::span<T>> {};
template<class T> struct ErasedArrayViewConverter<const std::span<T>>: ArrayViewConverter<T, std::span<T>> {};

template<class T, StlSpanSizeType Extent> struct ArrayViewConverter<T, std::span<T, Extent>> {
    constexpr static ArrayView<T> from(std::span<T, Extent> other) {
        return {other.data(), std::size_t(other.size())};
    }
};
template<class T, StlSpanSizeType Extent> struct ArrayViewConverter<const T, std::span<T, Extent>> {
    constexpr static ArrayView<T> from(std::span<T, Extent> other) {
        return {other.data(), std::size_t(other.size())};
    }
};
template<class T, StlSpanSizeType Extent> struct ArrayViewConverter<T, const std::span<T, Extent>>: ArrayViewConverter<T, std::span<T, Extent>> {};
template<class T, StlSpanSizeType Extent> struct ErasedArrayViewConverter<std::span<T, Extent>>: ArrayViewConverter<T, std::span<T, Extent>> {};
template<class T, StlSpanSizeType Extent> struct ErasedArrayViewConverter<const std::span<T, Extent>>: ArrayViewConverter<T, std::span<T, Extent>> {};

template<std::size_t size, class T> struct StaticArrayViewConverter<size, T, std::span<T, StlSpanSizeType(size)>> {
    constexpr static StaticArrayView<size, T> from(std::span<T, StlSpanSizeType(size)> other) {
        return StaticArrayView<size, T>{other.data()};
    }
    #if !defined(CORRADE_TARGET_LIBCXX) || _LIBCPP_VERSION >= 9000
    constexpr static std::span<T, size> to(StaticArrayView<size, T> other) {
        return std::span<T, size>{other.data(), other.size()};
    }
    #endif
};
template<std::size_t size, class T> struct StaticArrayViewConverter<size, T, const std::span<T, StlSpanSizeType(size)>>: StaticArrayViewConverter<size, T, std::span<T, StlSpanSizeType(size)>> {};
template<std::size_t size, class T> struct StaticArrayViewConverter<size, const T, std::span<T, StlSpanSizeType(size)>> {
    constexpr static StaticArrayView<size, const T> from(std::span<T, StlSpanSizeType(size)> other) {
        return StaticArrayView<size, const T>{other.data()};
    }
};
#if !defined(CORRADE_TARGET_LIBCXX) || _LIBCPP_VERSION >= 9000
template<std::size_t size, class T> struct StaticArrayViewConverter<size, T, std::span<const T, size>> {
    constexpr static std::span<const T, size> to(StaticArrayView<size, T> other) {
        return std::span<const T, size>{other.data(), other.size()};
    }
};
#endif
template<class T, StlSpanSizeType Extent> struct ErasedStaticArrayViewConverter<std::span<T, Extent>>: StaticArrayViewConverter<std::size_t(Extent), T, std::span<T, Extent>> {
    static_assert(Extent >= 0, "can't convert dynamic std::span to StaticArrayView");
};
template<class T, StlSpanSizeType Extent> struct ErasedStaticArrayViewConverter<const std::span<T, Extent>>: StaticArrayViewConverter<std::size_t(Extent), T, std::span<T, Extent>> {
    static_assert(Extent >= 0, "can't convert dynamic std::span to StaticArrayView");
};

}}}

#endif
#endif
