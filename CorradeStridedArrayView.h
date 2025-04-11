/*
    Corrade::Containers::StridedArrayView
        — multidimensional strided array view

    https://doc.magnum.graphics/corrade/classCorrade_1_1Containers_1_1StridedArrayView.html

    Depends on CorradeArrayView.h.

    This is a single-header library generated from the Corrade project. With
    the goal being easy integration, it's deliberately free of all comments
    to keep the file size small. More info, detailed changelogs and docs here:

    -   Project homepage — https://magnum.graphics/corrade/
    -   Documentation — https://doc.magnum.graphics/corrade/
    -   GitHub project page — https://github.com/mosra/corrade
    -   GitHub Singles repository — https://github.com/mosra/magnum-singles

    Structured bindings for StridedDimensions on C++17 are opt-in due to
    reliance on a potentially heavy STL header ---
    `#define CORRADE_STRUCTURED_BINDINGS` before including the file. Including
    it multiple times with different macros defined works too.

    v2020.06-1890-g77f9f (2025-04-11)
    -   Ability to construct StridedArrayView<T> from a StridedArrayView<T[]>
        of one dimension less
    -   Cleanup and unification of SFINAE code, it's now done in template args
        in all cases as that's simpler for the compiler
    -   Workaround for a MSVC compiler crash when slicing to a member function
        that returns a C array reference
    -   Added a missing StridedArrayView arraySize() overload
    v2020.06-1872-gbf086 (2025-03-03)
    -   Fixed slice() to work for non-overloaded member functions
    v2020.06-1846-gc4cdf (2025-01-07)
    -   Worked around an issue where certain new Clang versions would do an OOB
        access with negative strides on 32-bit builds (such as on Emscripten)
    -   Fixed StridedDimensions structured bindings constexpr signature
    -   Structured bindings of const types now work even w/o <utility>
    v2020.06-1687-g6b5f (2024-06-29)
    -   Structured bindings for StridedDimensions on C++17
    v2020.06-1454-gfc3b7 (2023-08-27)
    -   New expanded() and collapsed() APIs
    -   Ability to slice to struct members and member functions
    -   New exceptPrefix() API, the except() API is renamed to exceptSuffix().
        The suffix() API, which took an offset, is removed and will be
        eventually reintroduced again but taking suffix size, consistently with
        prefix() that takes prefix size.
    -   New sliceSize() API, taking a begin + size instead of begin + end
    -   New stridedArrayView() convenience helpers for creating 1D strided
        array views from ArrayView instances and a pointer + size
    -   The Size and Stride member typedefs are moved to the Containers
        namespace; Size1D, Size2D, Size3D, Size4D, Stride1D, Stride2D,
        Stride3D and Stride4D convenience typedefs were added
    -   Renamed empty() to isEmpty() for consistency with other bool-returning
        APIs
    -   MSVC 2022 compatibility
    -   Removed dependency on <utility>, resulting in about ~600 preprocessed
        lines less
    v2020.06-0-g61d1b58c (2020-06-27)
    -   Added mutable StridedDimensions::begin()/end()
    -   New cross-dimension arrayCast() overloads
    -   Added isContiguous() and asContiguous() overloads
    -   Similarly to ArrayView, there's now a StridedArrayView<void> and
        StridedArrayView<const void> specialization usable for type-erased
        storage in constexpr contexts
    v2019.10-0-g162d6a7d (2019-10-24)
    -   Don't assert when creating arrays with non-zero stride but zero size
    -   Added a StridedArrayView4D convenience typedef
    v2019.01-301-gefe8d740 (2019-08-05)
    -   MSVC 2019 compatibility
    -   New constructor taking just a size, with stride calculated implicitly
    -   Added except() for taking everything except last N elements
    -   Added every() for taking every Nth element
    v2019.01-173-ge663b49c (2019-04-30)
    -   Initial release

    Generated from Corrade v2020.06-1890-g77f9f (2025-04-11), 1415 / 2906 LoC
*/

/*
    This file is part of Corrade.

    Copyright © 2007, 2008, 2009, 2010, 2011, 2012, 2013, 2014, 2015, 2016,
                2017, 2018, 2019, 2020, 2021, 2022, 2023, 2024, 2025
              Vladimír Vondruš <mosra@centrum.cz>
    Copyright © 2022 Stanislaw Halik <sthalik@misaki.pl>
    Copyright © 2024 Will Usher <will@willusher.io>

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

#include "CorradeArrayView.h"

#ifdef __GNUC__
#define CORRADE_TARGET_GCC
#endif
#ifdef __clang__
#define CORRADE_TARGET_CLANG
#endif

#ifndef CORRADE_DEBUG_ASSERT
#define CORRADE_DEBUG_ASSERT(condition, message, returnValue)               \
    CORRADE_ASSERT(condition, message, returnValue)
#endif
#ifndef CorradeStridedArrayView_h
#define CorradeStridedArrayView_h
namespace Corrade { namespace Containers {

template<unsigned, class> class StridedDimensions;
template<unsigned dimensions> using Size = StridedDimensions<dimensions, std::size_t>;
template<unsigned dimensions> using Stride = StridedDimensions<dimensions, std::ptrdiff_t>;
template<unsigned, class> class StridedIterator;
template<unsigned, class> class StridedArrayView;
template<class T> using StridedArrayView1D = StridedArrayView<1, T>;
template<class T> using StridedArrayView2D = StridedArrayView<2, T>;
template<class T> using StridedArrayView3D = StridedArrayView<3, T>;
template<class T> using StridedArrayView4D = StridedArrayView<4, T>;
template<unsigned, class> class BasicStridedBitArrayView;

}}
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
#ifndef Corrade_Containers_sequenceHelpers_h
#define Corrade_Containers_sequenceHelpers_h

namespace Corrade { namespace Containers { namespace Implementation {

template<std::size_t ...> struct Sequence {};

template<class A, class B> struct SequenceConcat;
template<std::size_t ...first, std::size_t ...second> struct SequenceConcat<Sequence<first...>, Sequence<second...>> {
    typedef Sequence<first..., (sizeof...(first) + second)...> Type;
};

template<std::size_t N> struct GenerateSequence:
    SequenceConcat<typename GenerateSequence<N/2>::Type,
                   typename GenerateSequence<N - N/2>::Type> {};
template<> struct GenerateSequence<1> { typedef Sequence<0> Type; };
template<> struct GenerateSequence<0> { typedef Sequence<> Type; };

}}}

#endif
#ifndef Corrade_Utility_Math_h
#define Corrade_Utility_Math_h

namespace Corrade { namespace Utility {

template<class T> constexpr T min(T value, T min) {
    return min < value ? min : value;
}

template<class T> constexpr T max(T value, T max) {
    return value < max ? max : value;
}

template<class T> constexpr T abs(T a) {
    return a < T{0} ? -a : a;
}

}}

#endif
#if CORRADE_CXX_STANDARD >= 201402 && !defined(CORRADE_MSVC2015_COMPATIBILITY)
#define CORRADE_CONSTEXPR14 constexpr
#else
#define CORRADE_CONSTEXPR14
#endif
#ifndef Corrade_Containers_StridedDimensions_h
#define Corrade_Containers_StridedDimensions_h

namespace Corrade { namespace Containers {

namespace Implementation {
    template<unsigned, class, class> struct StridedDimensionsConverter;
    template<unsigned, class> struct StridedElement;
    template<unsigned, class> struct StridedBitElement;
    template<int> struct ArrayCastFlattenOrInflate;

    template<unsigned dimensions, class T> constexpr bool isAnyDimensionZero(const T(&)[dimensions], Sequence<>) {
        return false;
    }
    template<unsigned dimensions, class T, std::size_t first, std::size_t ...next> constexpr bool isAnyDimensionZero(const T(&size)[dimensions], Sequence<first, next...>) {
        return !size[first] || isAnyDimensionZero(size, Sequence<next...>{});
    }
    template<unsigned dimensions> constexpr std::size_t largestStride(const std::size_t(&)[dimensions], const std::ptrdiff_t(&)[dimensions], Sequence<>) {
        return 0;
    }
    template<unsigned dimensions, std::size_t first, std::size_t ...next> constexpr std::size_t largestStride(const std::size_t(&size)[dimensions], const std::ptrdiff_t(&stride)[dimensions], Sequence<first, next...>) {
        return Utility::max(size[first]*std::size_t(stride[first] < 0 ? -stride[first] : stride[first]),
            largestStride(size, stride, Sequence<next...>{}));
    }

    template<unsigned dimensions> constexpr std::ptrdiff_t strideForSizeInternal(const std::size_t(&)[dimensions], std::size_t, Sequence<>) {
        return 1;
    }
    template<unsigned dimensions, std::size_t first, std::size_t ...next> constexpr std::ptrdiff_t strideForSizeInternal(const std::size_t(&size)[dimensions], std::size_t index, Sequence<first, next...>) {
        #if defined(CORRADE_TARGET_GCC) && !defined(CORRADE_TARGET_CLANG) && __GNUC__*100 + __GNUC_MINOR__ >= 1002
        #pragma GCC diagnostic push
        #pragma GCC diagnostic ignored "-Wtype-limits"
        #endif
        return (first > index ? size[first] : 1)*strideForSizeInternal(size, index, Sequence<next...>{});
        #if defined(CORRADE_TARGET_GCC) && !defined(CORRADE_TARGET_CLANG) && __GNUC__*100 + __GNUC_MINOR__ >= 1002
        #pragma GCC diagnostic pop
        #endif
    }
    template<unsigned dimensions, std::size_t ...index> constexpr Stride<dimensions> strideForSize(const std::size_t(&size)[dimensions], std::size_t typeSize, Sequence<index...>) {
        return {std::ptrdiff_t(typeSize)*strideForSizeInternal(size, index, typename GenerateSequence<dimensions>::Type{})...};
    }
}

template<unsigned dimensions, class T> class StridedDimensions {
    public:
        constexpr /*implicit*/ StridedDimensions() noexcept: _data{} {}

        constexpr explicit StridedDimensions(Corrade::ValueInitT) noexcept: _data{} {}

        explicit StridedDimensions(Corrade::NoInitT) noexcept {}

        template<class ...Args> constexpr /*implicit*/ StridedDimensions(T first, Args... next) noexcept: _data{T(first), T(next)...} {
            static_assert(sizeof...(Args) + 1 == dimensions, "wrong value count");
        }

        constexpr /*implicit*/ StridedDimensions(const T(&values)[dimensions]) noexcept: StridedDimensions{values, typename Implementation::GenerateSequence<dimensions>::Type{}} {}

        template<class U, class = decltype(Implementation::StridedDimensionsConverter<dimensions, T, typename std::decay<U&&>::type>::from(std::declval<U&&>()))> constexpr /*implicit*/ StridedDimensions(U&& other) noexcept: StridedDimensions{Implementation::StridedDimensionsConverter<dimensions, T, typename std::decay<U&&>::type>::from(Utility::forward<U>(other))} {}

        template<class U, class = decltype(Implementation::StridedDimensionsConverter<dimensions, T, U>::to(std::declval<StridedDimensions<dimensions, T>>()))> constexpr /*implicit*/ operator U() const noexcept {
            return Implementation::StridedDimensionsConverter<dimensions, T, U>::to(*this);
        }

        template<unsigned d = dimensions, typename std::enable_if<d == 1, int>::type = 0>
        constexpr /*implicit*/ operator T() const { return _data[0]; }

        bool operator==(const StridedDimensions<dimensions, T>& other) const {
            for(std::size_t i = 0; i != dimensions; ++i)
                if(_data[i] != other._data[i]) return false;
            return true;
        }

        bool operator!=(const StridedDimensions<dimensions, T>& other) const {
            for(std::size_t i = 0; i != dimensions; ++i)
                if(_data[i] != other._data[i]) return true;
            return false;
        }

        constexpr T operator[](std::size_t i) const {
            return CORRADE_CONSTEXPR_DEBUG_ASSERT(i < dimensions,
                "Containers::StridedDimensions::operator[](): dimension" << i << "out of range for" << dimensions << "dimensions"), _data[i];
        }

        T& operator[](std::size_t i) {
            CORRADE_DEBUG_ASSERT(i < dimensions,
                "Containers::StridedDimensions::operator[](): dimension" << i << "out of range for" << dimensions << "dimensions", _data[0]);
            return _data[i];
        }

        T* begin() { return _data; }

        constexpr const T* begin() const { return _data; }
        constexpr const T* cbegin() const { return _data; }

        T* end() { return _data + dimensions; }

        constexpr const T* end() const { return _data + dimensions; }
        constexpr const T* cend() const { return _data + dimensions; }

    private:
        template<unsigned, class> friend class StridedArrayView;
        template<unsigned, class> friend class BasicStridedBitArrayView;
        template<unsigned, class> friend struct Implementation::StridedElement;
        template<unsigned, class> friend struct Implementation::StridedBitElement;
        template<int> friend struct Implementation::ArrayCastFlattenOrInflate;
        template<class U, unsigned dimensions_, class T_> friend StridedArrayView<dimensions_, U> arrayCast(const StridedArrayView<dimensions_, T_>&);
        template<class U, unsigned dimensions_> friend StridedArrayView<dimensions_, U> arrayCast(const StridedArrayView<dimensions_, void>&);
        template<class U, unsigned dimensions_> friend StridedArrayView<dimensions_, U> arrayCast(const StridedArrayView<dimensions_, const void>&);
        template<unsigned newDimensions, class U, unsigned dimensions_> StridedArrayView<newDimensions, U> friend arrayCast(const StridedArrayView<dimensions_, void>&, std::size_t);
        template<unsigned newDimensions, class U, unsigned dimensions_> StridedArrayView<newDimensions, U> friend arrayCast(const StridedArrayView<dimensions_, const void>&, std::size_t);

        #if CORRADE_CXX_STANDARD > 201402
        template<std::size_t index> constexpr friend const T& get(const StridedDimensions<dimensions, T>& value) {
            return value._data[index];
        }
        template<std::size_t index> CORRADE_CONSTEXPR14 friend T& get(StridedDimensions<dimensions, T>& value) {
            return value._data[index];
        }
        template<std::size_t index> CORRADE_CONSTEXPR14 friend T&& get(StridedDimensions<dimensions, T>&& value) {
            return Utility::move(value._data[index]);
        }
        #endif

        template<class U, std::size_t ...sequence> constexpr explicit StridedDimensions(const U* values, Implementation::Sequence<sequence...>) noexcept: _data{T(values[sequence])...} {}

        T _data[dimensions];
};

namespace Implementation {

template<unsigned dimensions, class T> struct StridedDimensionsConverter<dimensions, T, StaticArrayView<std::size_t(dimensions), const T>> {
    constexpr static StridedDimensions<dimensions, T> from(StaticArrayView<dimensions, const T> view) {
        return fromInternal(view.data(), typename GenerateSequence<dimensions>::Type{});
    }
    constexpr static StaticArrayView<dimensions, const T> to(const StridedDimensions<dimensions, T>& dimensions_) {
        return StaticArrayView<dimensions, const T>{dimensions_.begin()};
    }
    template<std::size_t ...sequence> constexpr static StridedDimensions<dimensions, T> fromInternal(const T* data, Sequence<sequence...>) {
        return {data[sequence]...};
    }
};

}

#ifndef CORRADE_MSVC2015_COMPATIBILITY /* Multiple definitions still broken */
template<unsigned dimensions> using Size = StridedDimensions<dimensions, std::size_t>;
#endif

typedef Size<1> Size1D;

typedef Size<2> Size2D;

typedef Size<3> Size3D;

typedef Size<4> Size4D;

#ifndef CORRADE_MSVC2015_COMPATIBILITY /* Multiple definitions still broken */
template<unsigned dimensions> using Stride = StridedDimensions<dimensions, std::ptrdiff_t>;
#endif

typedef Stride<1> Stride1D;

typedef Stride<2> Stride2D;

typedef Stride<3> Stride3D;

typedef Stride<4> Stride4D;

}}

#endif
#ifndef Corrade_Containers_StridedArrayView_h
#define Corrade_Containers_StridedArrayView_h

namespace Corrade { namespace Containers {

namespace Implementation {
    template<class T, class U> struct StridedArrayViewSliceResultOf;
    template<class T, class R> struct StridedArrayViewSliceResultOf<T, R&(T::*)() const> {
        typedef R Type;
    };
    template<class T, class R> struct StridedArrayViewSliceResultOf<T, R&(T::*)() const &> {
        typedef R Type;
    };
    #if defined(CORRADE_TARGET_GCC) && !defined(CORRADE_TARGET_CLANG) && __GNUC__ < 5
    template<class T> struct IsMemberFunctionPointer: std::false_type {};
    template<class T, class R, class ...Args> struct IsMemberFunctionPointer<R(T::*)(Args...)>: std::true_type {};
    template<class T, class R, class ...Args> struct IsMemberFunctionPointer<R(T::*)(Args...) &>: std::true_type {};
    template<class T, class R, class ...Args> struct IsMemberFunctionPointer<R(T::*)(Args...) const>: std::true_type {};
    template<class T, class R, class ...Args> struct IsMemberFunctionPointer<R(T::*)(Args...) const &>: std::true_type {};
    #endif
}

template<unsigned dimensions, class T> class StridedArrayView {
    static_assert(dimensions, "can't have a zero-dimensional view");

    public:
        typedef T Type;

        typedef typename std::conditional<dimensions == 1, T&, StridedArrayView<dimensions - 1, T>>::type ElementType;

        typedef typename std::conditional<std::is_const<T>::value, const void, void>::type ErasedType;

        typedef typename std::conditional<std::is_const<T>::value, const char, char>::type ArithmeticType;

        enum: unsigned {
            Dimensions = dimensions
        };

        template<class U, typename std::enable_if<std::is_same<std::nullptr_t, U>::value, int>::type = 0> constexpr /*implicit*/ StridedArrayView(U) noexcept: _data{}, _size{}, _stride{} {}
        constexpr /*implicit*/ StridedArrayView() noexcept: _data{}, _size{}, _stride{} {}

        constexpr /*implicit*/ StridedArrayView(ArrayView<ErasedType> data, T* member, const Containers::Size<dimensions>& size, const Containers::Stride<dimensions>& stride) noexcept;

        constexpr /*implicit*/ StridedArrayView(ArrayView<T> data, const Containers::Size<dimensions>& size, const Containers::Stride<dimensions>& stride) noexcept: StridedArrayView{data, data.data(), size, stride} {}

        constexpr /*implicit*/ StridedArrayView(ArrayView<T> data, const Containers::Size<dimensions>& size) noexcept: StridedArrayView{data, data.data(), size, Implementation::strideForSize<dimensions>(size._data, sizeof(T), typename Implementation::GenerateSequence<dimensions>::Type{})} {}

        template<unsigned d = dimensions, typename std::enable_if<d == 1, int>::type = 0>
        constexpr /*implicit*/ StridedArrayView(T* data, std::size_t size) noexcept: _data{data}, _size{size}, _stride{sizeof(T)} {}

        template<class U, std::size_t size
            , typename std::enable_if<dimensions == 1 && std::is_convertible<U*, T*>::value, int>::type = 0
        > constexpr /*implicit*/ StridedArrayView(U(&data)[size]) noexcept: _data{data}, _size{size}, _stride{sizeof(T)} {
            static_assert(sizeof(T) == sizeof(U), "type sizes are not compatible");
        }

        template<class U
            , typename std::enable_if<std::is_convertible<U*, T*>::value, int>::type = 0
        > constexpr /*implicit*/ StridedArrayView(const StridedArrayView<dimensions, U>& view) noexcept: _data{view._data}, _size{view._size}, _stride{view._stride} {
            static_assert(sizeof(T) == sizeof(U), "type sizes are not compatible");
        }

        template<std::size_t size_, class U
            , typename std::enable_if<std::is_same<T, U>::value || std::is_same<T, const U>::value, int>::type = 0
        > /*implicit*/ StridedArrayView(const StridedArrayView<dimensions - 1, U[size_]>& other) noexcept;

        template<unsigned lessDimensions, class U
            , typename std::enable_if<(lessDimensions < dimensions) && (std::is_same<T, U>::value || std::is_same<T, const U>::value), int>::type = 0
        > /*implicit*/ StridedArrayView(const StridedArrayView<lessDimensions, U>& other) noexcept;

        template<class U
            , typename std::enable_if<dimensions == 1 && std::is_convertible<U*, T*>::value, int>::type = 0
        > constexpr /*implicit*/ StridedArrayView(ArrayView<U> view) noexcept: _data{view.data()}, _size{view.size()}, _stride{sizeof(T)} {
            static_assert(sizeof(T) == sizeof(U), "type sizes are not compatible");
        }

        template<std::size_t size, class U
            , typename std::enable_if<dimensions == 1 && std::is_convertible<U*, T*>::value, int>::type = 0
        > constexpr /*implicit*/ StridedArrayView(StaticArrayView<size, U> view) noexcept: _data{view.data()}, _size{size}, _stride{sizeof(T)} {
            static_assert(sizeof(U) == sizeof(T), "type sizes are not compatible");
        }

        template<class U, unsigned d = dimensions, class = typename std::enable_if<d == 1, decltype(Implementation::ArrayViewConverter<T, typename std::decay<U&&>::type>::from(std::declval<U&&>()))>::type> constexpr /*implicit*/ StridedArrayView(U&& other) noexcept: StridedArrayView{Implementation::ArrayViewConverter<T, typename std::decay<U&&>::type>::from(Utility::forward<U>(other))} {}

        constexpr explicit operator bool() const { return _data; }

        constexpr ErasedType* data() const { return _data; }

        constexpr typename std::conditional<dimensions == 1, std::size_t, const Containers::Size<dimensions>&>::type size() const { return _size; }

        constexpr typename std::conditional<dimensions == 1, std::ptrdiff_t, const Containers::Stride<dimensions>&>::type stride() const { return _stride; }

        constexpr StridedDimensions<dimensions, bool> isEmpty() const {
            return isEmptyInternal(typename Implementation::GenerateSequence<dimensions>::Type{});
        }

        template<unsigned dimension = 0> bool isContiguous() const;

        ArrayView<T> asContiguous() const;

        template<unsigned dimension> StridedArrayView<dimension + 1, T> asContiguous() const;

        StridedIterator<dimensions, T> begin() const { return {_data, _size, _stride, 0}; }
        StridedIterator<dimensions, T> cbegin() const { return {_data, _size, _stride, 0}; }

        StridedIterator<dimensions, T> end() const {
            return {_data, _size, _stride, _size[0]};
        }
        StridedIterator<dimensions, T> cend() const {
            return {_data, _size, _stride, _size[0]};
        }

        ElementType front() const;

        ElementType back() const;

        ElementType operator[](std::size_t i) const;

        T& operator[](const Containers::Size<dimensions>& i) const;

        StridedArrayView<dimensions, T> slice(std::size_t begin, std::size_t end) const;

        template<unsigned newDimensions = dimensions> StridedArrayView<newDimensions, T> slice(const Containers::Size<dimensions>& begin, const Containers::Size<dimensions>& end) const;

        StridedArrayView<dimensions, T> sliceSize(std::size_t begin, std::size_t size) const {
            return slice(begin, begin + size);
        }

        template<unsigned newDimensions = dimensions> StridedArrayView<newDimensions, T> sliceSize(const Containers::Size<dimensions>& begin, const Containers::Size<dimensions>& size) const;

        template<unsigned newDimensions = dimensions> StridedArrayView<newDimensions, T> slice() const {
            return slice<newDimensions>({}, _size);
        }

        template<class U, class V = T, typename std::enable_if<
            #if defined(CORRADE_TARGET_GCC) && !defined(CORRADE_TARGET_CLANG) && __GNUC__ < 5
            !Implementation::IsMemberFunctionPointer<U V::*>::value
            #else
            !std::is_member_function_pointer<U V::*>::value
            #endif
        , int>::type = 0> auto slice(U V::*member) const -> StridedArrayView<dimensions, typename std::conditional<std::is_const<T>::value, const U, U>::type> {
            return StridedArrayView<dimensions, typename std::conditional<std::is_const<T>::value, const U, U>::type>{_size, _stride, &(static_cast<T*>(_data)->*member)};
        }

        template<class U, class V = T, class W = T, typename std::enable_if<!std::is_const<W>::value, int>::type = 0> StridedArrayView<dimensions, U> slice(U&(V::*memberFunction)()) const;
        template<class U, class V = T, class W = T, typename std::enable_if<!std::is_const<W>::value, int>::type = 0> StridedArrayView<dimensions, U> slice(U&(V::*memberFunction)() &) const;
        template<class U, class V = T, class W = T, typename std::enable_if<std::is_const<W>::value, int>::type = 0> StridedArrayView<dimensions, const U> slice(const U&(V::*memberFunction)() const) const;
        template<class U, class V = T, class W = T,
            #if defined(CORRADE_TARGET_GCC) && !defined(CORRADE_TARGET_CLANG) && __GNUC__ < 7
            class = typename std::enable_if<std::is_const<W>::value>::type
            #else
            typename std::enable_if<std::is_const<W>::value, int>::type = 0
            #endif
        > StridedArrayView<dimensions, const U> slice(const U&(V::*memberFunction)() const &) const;
        template<class U> auto slice(U&& memberFunction) const -> StridedArrayView<dimensions, typename Implementation::StridedArrayViewSliceResultOf<T, U>::Type>;

        BasicStridedBitArrayView<dimensions, ArithmeticType> sliceBit(std::size_t index) const;

        StridedArrayView<dimensions, T> prefix(std::size_t size) const {
            return slice(0, size);
        }

        template<unsigned newDimensions = dimensions> StridedArrayView<newDimensions, T> prefix(const Containers::Size<dimensions>& size) const {
            return slice<newDimensions>({}, size);
        }

        StridedArrayView<dimensions, T> exceptPrefix(std::size_t size) const {
            return slice(size, _size._data[0]);
        }

        template<unsigned newDimensions = dimensions> StridedArrayView<newDimensions, T> exceptPrefix(const Containers::Size<dimensions>& size) const {
            return slice<newDimensions>(size, _size);
        }

        StridedArrayView<dimensions, T> exceptSuffix(std::size_t size) const {
            return slice({}, _size._data[0] - size);
        }

        template<unsigned newDimensions = dimensions> StridedArrayView<newDimensions, T> exceptSuffix(const Containers::Size<dimensions>& size) const;

        StridedArrayView<dimensions, T> every(std::ptrdiff_t skip) const;

        StridedArrayView<dimensions, T> every(const Containers::Stride<dimensions>& skip) const;

        template<unsigned dimensionA, unsigned dimensionB> StridedArrayView<dimensions, T> transposed() const;

        template<unsigned dimension> StridedArrayView<dimensions, T> flipped() const;

        template<unsigned dimension> StridedArrayView<dimensions, T> broadcasted(std::size_t size) const;

        template<unsigned dimension, unsigned count> StridedArrayView<dimensions + count - 1, T> expanded(const Containers::Size<count>& size) const;

        template<unsigned dimension, unsigned count> StridedArrayView<dimensions - count + 1, T> collapsed() const;

    private:
        template<unsigned, class> friend class StridedArrayView;

        template<unsigned, class> friend struct Implementation::StridedElement;
        template<int> friend struct Implementation::ArrayCastFlattenOrInflate;
        template<class U, unsigned dimensions_, class T_> friend StridedArrayView<dimensions_, U> arrayCast(const StridedArrayView<dimensions_, T_>&);
        template<class U, unsigned dimensions_> friend StridedArrayView<dimensions_, U> arrayCast(const StridedArrayView<dimensions_, void>&);
        template<class U, unsigned dimensions_> friend StridedArrayView<dimensions_, U> arrayCast(const StridedArrayView<dimensions_, const void>&);
        template<unsigned newDimensions, class U, unsigned dimensions_> StridedArrayView<newDimensions, U> friend arrayCast(const StridedArrayView<dimensions_, void>&, std::size_t);
        template<unsigned newDimensions, class U, unsigned dimensions_> StridedArrayView<newDimensions, U> friend arrayCast(const StridedArrayView<dimensions_, const void>&, std::size_t);

        constexpr /*implicit*/ StridedArrayView(const Containers::Size<dimensions>& size, const Containers::Stride<dimensions>& stride, ErasedType* data) noexcept: _data{data}, _size{size}, _stride{stride} {}

        template<std::size_t ...sequence> constexpr StridedDimensions<dimensions, bool> isEmptyInternal(Implementation::Sequence<sequence...>) const {
            return StridedDimensions<dimensions, bool>{(_size._data[sequence] == 0)...};
        }

        ErasedType* _data;
        Containers::Size<dimensions> _size;
        Containers::Stride<dimensions> _stride;
};

template<unsigned dimensions> class StridedArrayView<dimensions, void> {
    static_assert(dimensions, "can't have a zero-dimensional view");

    public:
        typedef void Type;

        typedef void ErasedType;

        enum: unsigned {
            Dimensions = dimensions
        };

        template<class U, typename std::enable_if<std::is_same<std::nullptr_t, U>::value, int>::type = 0> constexpr /*implicit*/ StridedArrayView(U) noexcept: _data{}, _size{}, _stride{} {}
        constexpr /*implicit*/ StridedArrayView() noexcept: _data{}, _size{}, _stride{} {}

        constexpr /*implicit*/ StridedArrayView(ArrayView<void> data, void* member, const Containers::Size<dimensions>& size, const Containers::Stride<dimensions>& stride) noexcept;

        constexpr /*implicit*/ StridedArrayView(ArrayView<void> data, const Containers::Size<dimensions>& size, const Containers::Stride<dimensions>& stride) noexcept: StridedArrayView{data, data.data(), size, stride} {}

        template<class T
            , unsigned d = dimensions, typename std::enable_if<d == 1, int>::type = 0
        > constexpr /*implicit*/ StridedArrayView(T* data, std::size_t size) noexcept: _data{data}, _size{size}, _stride{sizeof(T)} {}

        template<class T, std::size_t size
            , typename std::enable_if<dimensions == 1 && !std::is_const<T>::value, int>::type = 0
        > constexpr /*implicit*/ StridedArrayView(T(&data)[size]) noexcept: _data{data}, _size{size}, _stride{sizeof(T)} {}

        template<class T
            , typename std::enable_if<!std::is_const<T>::value, int>::type = 0
        > constexpr /*implicit*/ StridedArrayView(StridedArrayView<dimensions, T> view) noexcept: _data{view._data}, _size{view._size}, _stride{view._stride} {}

        template<class T
            , typename std::enable_if<dimensions == 1 && !std::is_const<T>::value, int>::type = 0
        > constexpr /*implicit*/ StridedArrayView(ArrayView<T> view) noexcept: _data{view.data()}, _size{view.size()}, _stride{sizeof(T)} {}

        template<std::size_t size, class T
            , typename std::enable_if<dimensions == 1 && !std::is_const<T>::value, int>::type = 0
        > constexpr /*implicit*/ StridedArrayView(StaticArrayView<size, T> view) noexcept: _data{view.data()}, _size{size}, _stride{sizeof(T)} {}

        template<class T, unsigned d = dimensions, class = typename std::enable_if<d == 1, decltype(Implementation::ErasedArrayViewConverter<typename std::decay<T&&>::type>::from(std::declval<T&&>()))>::type> constexpr /*implicit*/ StridedArrayView(T&& other) noexcept: StridedArrayView{Implementation::ErasedArrayViewConverter<typename std::decay<T&&>::type>::from(other)} {}

        constexpr explicit operator bool() const { return _data; }

        constexpr void* data() const { return _data; }

        constexpr typename std::conditional<dimensions == 1, std::size_t, const Containers::Size<dimensions>&>::type size() const { return _size; }

        constexpr typename std::conditional<dimensions == 1, std::ptrdiff_t, const Containers::Stride<dimensions>&>::type stride() const { return _stride; }

        constexpr StridedDimensions<dimensions, bool> isEmpty() const {
            return isEmptyInternal(typename Implementation::GenerateSequence<dimensions>::Type{});
        }

    private:
        template<unsigned, class> friend class StridedArrayView;

        constexpr /*implicit*/ StridedArrayView(const Containers::Size<dimensions>& size, const Containers::Stride<dimensions>& stride, void* data) noexcept: _data{data}, _size{size}, _stride{stride} {}

        template<std::size_t ...sequence> constexpr StridedDimensions<dimensions, bool> isEmptyInternal(Implementation::Sequence<sequence...>) const {
            return StridedDimensions<dimensions, bool>{(_size._data[sequence] == 0)...};
        }

        void* _data;
        Containers::Size<dimensions> _size;
        Containers::Stride<dimensions> _stride;
};

template<unsigned dimensions> class StridedArrayView<dimensions, const void> {
    static_assert(dimensions, "can't have a zero-dimensional view");

    public:
        typedef const void Type;

        typedef const void ErasedType;

        enum: unsigned {
            Dimensions = dimensions
        };

        template<class U, typename std::enable_if<std::is_same<std::nullptr_t, U>::value, int>::type = 0> constexpr /*implicit*/ StridedArrayView(U) noexcept: _data{}, _size{}, _stride{} {}
        constexpr /*implicit*/ StridedArrayView() noexcept: _data{}, _size{}, _stride{} {}

        constexpr /*implicit*/ StridedArrayView(ArrayView<const void> data, const void* member, const Containers::Size<dimensions>& size, const Containers::Stride<dimensions>& stride) noexcept;

        constexpr /*implicit*/ StridedArrayView(ArrayView<const void> data, const Containers::Size<dimensions>& size, const Containers::Stride<dimensions>& stride) noexcept: StridedArrayView{data, data.data(), size, stride} {}

        template<class T
            , unsigned d = dimensions, typename std::enable_if<d == 1, int>::type = 0
        > constexpr /*implicit*/ StridedArrayView(T* data, std::size_t size) noexcept: _data{data}, _size{size}, _stride{sizeof(T)} {}

        template<class T, std::size_t size
            , unsigned d = dimensions, typename std::enable_if<d == 1, int>::type = 0
        > constexpr /*implicit*/ StridedArrayView(T(&data)[size]) noexcept: _data{data}, _size{size}, _stride{sizeof(T)} {}

        template<class T> constexpr /*implicit*/ StridedArrayView(StridedArrayView<dimensions, T> view) noexcept: _data{view._data}, _size{view._size}, _stride{view._stride} {}

        template<class T
            , unsigned d = dimensions, typename std::enable_if<d == 1, int>::type = 0
        > constexpr /*implicit*/ StridedArrayView(ArrayView<T> view) noexcept: _data{view.data()}, _size{view.size()}, _stride{sizeof(T)} {}

        template<std::size_t size, class T
            , unsigned d = dimensions, typename std::enable_if<d == 1, int>::type = 0
        > constexpr /*implicit*/ StridedArrayView(StaticArrayView<size, T> view) noexcept: _data{view.data()}, _size{size}, _stride{sizeof(T)} {}

        template<class T, unsigned d = dimensions, class = typename std::enable_if<d == 1, decltype(Implementation::ErasedArrayViewConverter<const T>::from(std::declval<const T&>()))>::type> constexpr /*implicit*/ StridedArrayView(const T& other) noexcept: StridedArrayView{Implementation::ErasedArrayViewConverter<const T>::from(other)} {}

        constexpr explicit operator bool() const { return _data; }

        constexpr const void* data() const { return _data; }

        constexpr typename std::conditional<dimensions == 1, std::size_t, const Containers::Size<dimensions>&>::type size() const { return _size; }

        constexpr typename std::conditional<dimensions == 1, std::ptrdiff_t, const Containers::Stride<dimensions>&>::type stride() const { return _stride; }

        constexpr StridedDimensions<dimensions, bool> isEmpty() const {
            return isEmptyInternal(typename Implementation::GenerateSequence<dimensions>::Type{});
        }

    private:
        template<unsigned, class> friend class StridedArrayView;

        template<class U, unsigned dimensions_> friend StridedArrayView<dimensions_, U> arrayCast(const StridedArrayView<dimensions_, const void>&);
        template<unsigned newDimensions, class U, unsigned dimensions_> StridedArrayView<newDimensions, U> friend arrayCast(const StridedArrayView<dimensions_, const void>&, std::size_t);

        constexpr /*implicit*/ StridedArrayView(const Containers::Size<dimensions>& size, const Containers::Stride<dimensions>& stride, const void* data) noexcept: _data{data}, _size{size}, _stride{stride} {}

        template<std::size_t ...sequence> constexpr StridedDimensions<dimensions, bool> isEmptyInternal(Implementation::Sequence<sequence...>) const {
            return StridedDimensions<dimensions, bool>{(_size._data[sequence] == 0)...};
        }

        const void* _data;
        Containers::Size<dimensions> _size;
        Containers::Stride<dimensions> _stride;
};

#ifndef CORRADE_MSVC2015_COMPATIBILITY /* Multiple definitions still broken */
template<class T> using StridedArrayView1D = StridedArrayView<1, T>;

template<class T> using StridedArrayView2D = StridedArrayView<2, T>;

template<class T> using StridedArrayView3D = StridedArrayView<3, T>;

template<class T> using StridedArrayView4D = StridedArrayView<4, T>;
#endif

template<class T> constexpr StridedArrayView1D<T> stridedArrayView(ArrayView<typename StridedArrayView1D<T>::ErasedType> data, T* member, std::size_t size, std::ptrdiff_t stride) {
    return StridedArrayView1D<T>{data, member, size, stride};
}

#if defined(CORRADE_TARGET_GCC) && !defined(CORRADE_TARGET_CLANG) && __GNUC__ < 5
template<std::size_t size_, class T, class U> constexpr StridedArrayView1D<T> stridedArrayView(U(&data)[size_], T* member, std::size_t size, std::ptrdiff_t stride) {
    return StridedArrayView1D<T>{data, member, size, stride};
}
#endif

template<class T> constexpr StridedArrayView1D<T> stridedArrayView(ArrayView<T> data, std::size_t size, std::ptrdiff_t stride) {
    return {data, size, stride};
}

template<class T> constexpr StridedArrayView1D<T> stridedArrayView(T* data, std::size_t size) {
    return {data, size};
}

template<std::size_t size, class T> constexpr StridedArrayView1D<T> stridedArrayView(T(&data)[size]) {
    return StridedArrayView1D<T>{data};
}

template<class T> StridedArrayView1D<const T> stridedArrayView(std::initializer_list<T> list) {
    return StridedArrayView1D<const T>{arrayView(list)};
}

template<class T> constexpr StridedArrayView1D<T> stridedArrayView(ArrayView<T> view) {
    return view;
}

template<std::size_t size, class T> constexpr StridedArrayView1D<T> stridedArrayView(StaticArrayView<size, T> view) {
    return ArrayView<T>{view};
}

template<unsigned dimensions, class T> constexpr StridedArrayView<dimensions, T> stridedArrayView(StridedArrayView<dimensions, T> view) {
    return view;
}

template<class T, class U = decltype(stridedArrayView(Implementation::ErasedArrayViewConverter<typename std::remove_reference<T&&>::type>::from(std::declval<T&&>())))> constexpr U stridedArrayView(T&& other) {
    return Implementation::ErasedArrayViewConverter<typename std::remove_reference<T&&>::type>::from(Utility::forward<T>(other));
}

template<class U, unsigned dimensions, class T> StridedArrayView<dimensions, U> arrayCast(const StridedArrayView<dimensions, T>& view) {
    static_assert(std::is_standard_layout<T>::value, "the source type is not standard layout");
    static_assert(std::is_standard_layout<U>::value, "the target type is not standard layout");
    for(unsigned i = 0; i != dimensions; ++i) {
        CORRADE_ASSERT(!view._stride._data[i] || sizeof(U) <= std::size_t(view._stride._data[i] < 0 ? -view._stride._data[i] : view._stride._data[i]),
            "Containers::arrayCast(): can't fit a" << sizeof(U) << Utility::Debug::nospace << "-byte type into a stride of" << view._stride._data[i], {});
    }
    return StridedArrayView<dimensions, U>{view._size, view._stride, view._data};
}

template<class U, unsigned dimensions> StridedArrayView<dimensions, U> arrayCast(const StridedArrayView<dimensions, const void>& view) {
    static_assert(std::is_standard_layout<U>::value, "the target type is not standard layout");
    for(unsigned i = 0; i != dimensions; ++i) {
        CORRADE_ASSERT(!view._stride._data[i] || sizeof(U) <= std::size_t(view._stride._data[i] < 0 ? -view._stride._data[i] : view._stride._data[i]),
            "Containers::arrayCast(): can't fit a" << sizeof(U) << Utility::Debug::nospace << "-byte type into a stride of" << view._stride._data[i], {});
    }
    return StridedArrayView<dimensions, U>{view._size, view._stride, view._data};
}

template<class U, unsigned dimensions> StridedArrayView<dimensions, U> arrayCast(const StridedArrayView<dimensions, void>& view) {
    auto out = arrayCast<const U, dimensions>(StridedArrayView<dimensions, const void>{view});
    return StridedArrayView<dimensions, U>{out._size, out._stride, const_cast<void*>(out._data)};
}

namespace Implementation {

template<int dimensions> struct ArrayCastFlattenOrInflate {
    static_assert(dimensions == 0, "can only inflate into one more dimension or flatten into the same / one less dimension");
};
template<> struct ArrayCastFlattenOrInflate<-1> {
    template<class U, unsigned dimensions, class T> static StridedArrayView<dimensions - 1, U> cast(const StridedArrayView<dimensions, T>& view) {
        for(unsigned i = 0; i != dimensions - 1; ++i) {
            CORRADE_ASSERT(!view._stride._data[i] || sizeof(U) <= std::size_t(view._stride._data[i] < 0 ? -view._stride._data[i] : view._stride._data[i]),
                "Containers::arrayCast(): can't fit a" << sizeof(U) << Utility::Debug::nospace << "-byte type into a stride of" << view._stride._data[i], {});
        }
        CORRADE_ASSERT(sizeof(T) == std::size_t(view._stride._data[dimensions - 1]),
            "Containers::arrayCast(): last dimension needs to be contiguous in order to be flattened, expected stride" << sizeof(T) << "but got" << view.stride()[dimensions - 1], {});
        CORRADE_ASSERT(sizeof(T)*view._size._data[dimensions - 1] == sizeof(U),
            "Containers::arrayCast(): last dimension needs to have byte size equal to new type size in order to be flattened, expected" << sizeof(U) << "but got" << sizeof(T)*view._size._data[dimensions - 1], {});
        return StridedArrayView<dimensions - 1, U>{
            StaticArrayView<dimensions, const std::size_t>(view._size).template prefix<dimensions - 1>(),
            StaticArrayView<dimensions, const std::ptrdiff_t>(view._stride).template prefix<dimensions - 1>(),
            view._data};
    }
};
template<> struct ArrayCastFlattenOrInflate<0> {
    template<class U, unsigned dimensions, class T> static StridedArrayView<dimensions, U> cast(const StridedArrayView<dimensions, T>& view) {
        for(unsigned i = 0; i != dimensions - 1; ++i) {
            CORRADE_ASSERT(!view._stride._data[i] || sizeof(U) <= std::size_t(view._stride._data[i] < 0 ? -view._stride._data[i] : view._stride._data[i]),
                "Containers::arrayCast(): can't fit a" << sizeof(U) << Utility::Debug::nospace << "-byte type into a stride of" << view._stride._data[i], {});
        }
        CORRADE_ASSERT(sizeof(T) == std::size_t(view._stride._data[dimensions - 1]),
            "Containers::arrayCast(): last dimension needs to be contiguous in order to be flattened, expected stride" << sizeof(T) << "but got" << view.stride()[dimensions - 1], {});
        CORRADE_ASSERT(sizeof(T)*view._size._data[dimensions - 1] % sizeof(U) == 0,
            "Containers::arrayCast(): last dimension needs to have byte size divisible by new type size in order to be flattened, but for a" << sizeof(U) << Utility::Debug::nospace << "-byte type got" << sizeof(T)*view._size._data[dimensions - 1], {});

        Size<dimensions> size;
        Stride<dimensions> stride;
        size._data[dimensions - 1] = sizeof(T)*view._size._data[dimensions - 1]/sizeof(U);
        stride._data[dimensions - 1] = sizeof(U);
        for(std::size_t i = 0; i != dimensions - 1; ++i) {
            size._data[i] = view._size._data[i];
            stride._data[i] = view._stride._data[i];
        }
        return StridedArrayView<dimensions, U>{size, stride, view._data};
    }
};
template<> struct ArrayCastFlattenOrInflate<+1> {
    template<class U, unsigned dimensions, class T> static StridedArrayView<dimensions + 1, U> cast(const StridedArrayView<dimensions, T>& view) {
        constexpr std::size_t lastDimensionSize = sizeof(T)/sizeof(U);
        static_assert(sizeof(T) % lastDimensionSize == 0, "original type not a multiply of inflated type");
        Size<dimensions + 1> size;
        Stride<dimensions + 1> stride;
        size._data[dimensions] = lastDimensionSize;
        stride._data[dimensions] = sizeof(U);
        for(std::size_t i = 0; i != dimensions; ++i) {
            size._data[i] = view._size._data[i];
            stride._data[i] = view._stride._data[i];
        }
        return StridedArrayView<dimensions + 1, U>{size, stride, view._data};
    }
};

}

template<unsigned newDimensions, class U, unsigned dimensions, class T> StridedArrayView<newDimensions, U> arrayCast(const StridedArrayView<dimensions, T>& view) {
    static_assert(std::is_standard_layout<T>::value, "the source type is not standard layout");
    static_assert(std::is_standard_layout<U>::value, "the target type is not standard layout");
    return Implementation::ArrayCastFlattenOrInflate<int(newDimensions) - int(dimensions)>::template cast<U>(view);
}

template<unsigned newDimensions, class U, class T> inline StridedArrayView<newDimensions, U> arrayCast(const ArrayView<T>& view) {
    return arrayCast<newDimensions, U, 1, T>(view);
}

template<unsigned newDimensions, class U, unsigned dimensions> StridedArrayView<newDimensions, U> arrayCast(const StridedArrayView<dimensions, const void>& view, std::size_t lastDimensionSize) {
    static_assert(std::is_standard_layout<U>::value, "the target type is not standard layout");
    static_assert(newDimensions == dimensions + 1, "can inflate only into one more dimension");
    for(unsigned i = 0; i != dimensions - 1; ++i) {
        CORRADE_ASSERT(!view._stride._data[i] || sizeof(U) <= std::size_t(view._stride._data[i] < 0 ? -view._stride._data[i] : view._stride._data[i]),
            "Containers::arrayCast(): can't fit a" << sizeof(U) << Utility::Debug::nospace << "-byte type into a stride of" << view._stride._data[i], {});
    }
    CORRADE_ASSERT(!view._stride._data[dimensions - 1] || lastDimensionSize*sizeof(U) <= std::size_t(view._stride._data[dimensions - 1] < 0 ? -view._stride._data[dimensions - 1] : view._stride._data[dimensions - 1]),
        "Containers::arrayCast(): can't fit" << lastDimensionSize << sizeof(U) << Utility::Debug::nospace << "-byte items into a stride of" << view._stride._data[dimensions - 1], {});
    std::size_t size[newDimensions];
    std::ptrdiff_t stride[newDimensions];
    size[dimensions] = lastDimensionSize;
    stride[dimensions] = sizeof(U);
    for(std::size_t i = 0; i != dimensions; ++i) {
        size[i] = view._size._data[i];
        stride[i] = view._stride._data[i];
    }
    return StridedArrayView<newDimensions, U>{
        StaticArrayView<newDimensions, const std::size_t>(size), StaticArrayView<newDimensions, const std::ptrdiff_t>(stride),
        view._data};
}

template<unsigned newDimensions, class U, unsigned dimensions> StridedArrayView<newDimensions, U> arrayCast(const StridedArrayView<dimensions, void>& view, std::size_t lastDimensionSize) {
    auto out = arrayCast<newDimensions, const U, dimensions>(StridedArrayView<dimensions, const void>{view}, lastDimensionSize);
    return StridedArrayView<newDimensions, U>{out._size, out._stride, const_cast<void*>(out._data)};
}

template<unsigned dimensions, class T> std::size_t arraySize(const StridedArrayView<dimensions, T>& view) {
    return Size<dimensions>{view.size()}[0];
}

template<unsigned dimensions, class T> class StridedIterator {
    public:
        typedef T Type;

        typedef typename std::conditional<dimensions == 1, T&, StridedArrayView<dimensions - 1, T>>::type ElementType;

        /*implicit*/ StridedIterator(typename std::conditional<std::is_const<T>::value, const void, void>::type* data, const Size<dimensions>& size, const Stride<dimensions>& stride, std::size_t i) noexcept: _data{data}, _size{size}, _stride{stride}, _i{i} {}

        #ifdef CORRADE_MSVC2015_COMPATIBILITY
        /*implicit*/ StridedIterator(): _data{}, _size{}, _stride{}, _i{} {}
        #endif

        bool operator==(const StridedIterator<dimensions, T>& other) const {
            return _data == other._data && _stride == other._stride && _i == other._i;
        }

        bool operator!=(const StridedIterator<dimensions, T>& other) const {
            return _data != other._data || _stride != other._stride || _i != other._i;
        }

        bool operator<(const StridedIterator<dimensions, T>& other) const {
            return _data == other._data && _stride == other._stride && _i < other._i;
        }

        bool operator<=(const StridedIterator<dimensions, T>& other) const {
            return _data == other._data && _stride == other._stride && _i <= other._i;
        }

        bool operator>(const StridedIterator<dimensions, T>& other) const {
            return _data == other._data && _stride == other._stride && _i > other._i;
        }

        bool operator>=(const StridedIterator<dimensions, T>& other) const {
            return _data == other._data && _stride == other._stride && _i >= other._i;
        }

        StridedIterator<dimensions, T> operator+(std::ptrdiff_t i) const {
            return {_data, _size, _stride, _i + i};
        }

        StridedIterator<dimensions, T>& operator+=(std::ptrdiff_t i) {
            _i += i;
            return *this;
        }

        StridedIterator<dimensions, T> operator-(std::ptrdiff_t i) const {
            return {_data, _size, _stride, _i - i};
        }

        StridedIterator<dimensions, T>& operator-=(std::ptrdiff_t i) {
            _i -= i;
            return *this;
        }

        std::ptrdiff_t operator-(const StridedIterator<dimensions, T>& it) const {
            return _i - it._i;
        }

        StridedIterator<dimensions, T>& operator--() {
            --_i;
            return *this;
        }

        StridedIterator<dimensions, T>& operator++() {
            ++_i;
            return *this;
        }

        ElementType operator*() const {
            return Implementation::StridedElement<dimensions, T>::get(_data, _size, _stride, _i);
        }

    private:
        typename std::conditional<std::is_const<T>::value, const void, void>::type* _data;
        Size<dimensions> _size;
        Stride<dimensions> _stride;
        std::size_t _i;
};

template<unsigned dimensions, class T> inline StridedIterator<dimensions, T> operator+(std::ptrdiff_t i, StridedIterator<dimensions, T> it) {
    return it + i;
}

template<unsigned dimensions, class T> constexpr StridedArrayView<dimensions, T>::StridedArrayView(ArrayView<ErasedType> data, T* member, const Containers::Size<dimensions>& size, const Containers::Stride<dimensions>& stride) noexcept: _data{(
    CORRADE_CONSTEXPR_ASSERT(Implementation::isAnyDimensionZero(size._data, typename Implementation::GenerateSequence<dimensions>::Type{}) || Implementation::largestStride(size._data, stride._data, typename Implementation::GenerateSequence<dimensions>::Type{}) <= data.size(),
        "Containers::StridedArrayView: data size" << data.size() << "is not enough for" << size << "elements of stride" << stride),
    member)}, _size{size}, _stride{stride} {}

template<unsigned dimensions> constexpr StridedArrayView<dimensions, void>::StridedArrayView(ArrayView<void> data, void* member, const Containers::Size<dimensions>& size, const Containers::Stride<dimensions>& stride) noexcept: _data{(
    CORRADE_CONSTEXPR_ASSERT(Implementation::isAnyDimensionZero(size._data, typename Implementation::GenerateSequence<dimensions>::Type{}) || Implementation::largestStride(size._data, stride._data, typename Implementation::GenerateSequence<dimensions>::Type{}) <= data.size(),
        "Containers::StridedArrayView: data size" << data.size() << "is not enough for" << size << "elements of stride" << stride),
    member)}, _size{size}, _stride{stride} {}

template<unsigned dimensions> constexpr StridedArrayView<dimensions, const void>::StridedArrayView(ArrayView<const void> data, const void* member, const Containers::Size<dimensions>& size, const Containers::Stride<dimensions>& stride) noexcept: _data{(
    CORRADE_CONSTEXPR_ASSERT(Implementation::isAnyDimensionZero(size._data, typename Implementation::GenerateSequence<dimensions>::Type{}) || Implementation::largestStride(size._data, stride._data, typename Implementation::GenerateSequence<dimensions>::Type{}) <= data.size(),
        "Containers::StridedArrayView: data size" << data.size() << "is not enough for" << size << "elements of stride" << stride),
    member)}, _size{size}, _stride{stride} {}

template<unsigned dimensions, class T> template<std::size_t size_, class U, typename std::enable_if<std::is_same<T, U>::value || std::is_same<T, const U>::value, int>::type> StridedArrayView<dimensions, T>::StridedArrayView(const StridedArrayView<dimensions - 1, U[size_]>& other) noexcept: _data{other._data}, _size{Corrade::NoInit}, _stride{Corrade::NoInit} {
    for(std::size_t i = 0; i != dimensions - 1; ++i) {
        _size._data[i] = other._size._data[i];
        _stride._data[i] = other._stride._data[i];
    }
    _size._data[dimensions - 1] = size_;
    _stride._data[dimensions - 1] = sizeof(T);
}

template<unsigned dimensions, class T> template<unsigned lessDimensions, class U, typename std::enable_if<(lessDimensions < dimensions) && (std::is_same<T, U>::value || std::is_same<T, const U>::value), int>::type> StridedArrayView<dimensions, T>::StridedArrayView(const StridedArrayView<lessDimensions, U>& other) noexcept: _data{other._data}, _size{Corrade::NoInit}, _stride{Corrade::NoInit} {
    constexpr std::size_t extraDimensions = dimensions - lessDimensions;
    const std::ptrdiff_t stride = std::ptrdiff_t(other._size._data[0])*other._stride._data[0];
    for(std::size_t i = 0; i != extraDimensions; ++i) {
        _size._data[i] = 1;
        _stride._data[i] = stride;
    }
    for(std::size_t i = 0; i != lessDimensions; ++i) {
        _size._data[extraDimensions + i] = other._size._data[i];
        _stride._data[extraDimensions + i] = other._stride._data[i];
    }
}

template<unsigned dimensions, class T> template<unsigned dimension> bool StridedArrayView<dimensions, T>::isContiguous() const {
    static_assert(dimension < dimensions, "dimension out of range");
    std::size_t nextDimensionSize = sizeof(T);
    for(std::size_t i = dimensions; i != dimension; --i) {
        if(std::size_t(_stride._data[i - 1]) != nextDimensionSize)
            return false;
        nextDimensionSize *= _size._data[i - 1];
    }

    return true;
}

template<unsigned dimensions, class T> ArrayView<T> StridedArrayView<dimensions, T>::asContiguous() const {
    CORRADE_DEBUG_ASSERT(isContiguous(),
        "Containers::StridedArrayView::asContiguous(): the view is not contiguous", {});
    std::size_t size = 1;
    for(std::size_t i = 0; i != dimensions; ++i) size *= _size._data[i];
    return {static_cast<T*>(_data), size};
}

template<unsigned dimensions, class T> template<unsigned dimension> StridedArrayView<dimension + 1, T> StridedArrayView<dimensions, T>::asContiguous() const {
    static_assert(dimension < dimensions, "dimension out of range");
    CORRADE_DEBUG_ASSERT(isContiguous<dimension>(),
        "Containers::StridedArrayView::asContiguous(): the view is not contiguous from dimension" << dimension, {});

    Containers::Size<dimension + 1> size;
    Containers::Stride<dimension + 1> stride;
    for(std::size_t i = 0; i != dimension; ++i) {
        size._data[i] = _size._data[i];
        stride._data[i] = _stride._data[i];
    }

    size._data[dimension] = 1;
    stride._data[dimension] = sizeof(T);
    for(std::size_t i = dimension; i != dimensions; ++i)
        size._data[dimension] *= _size._data[i];

    return {size, stride, _data};
}

namespace Implementation {
    template<unsigned dimensions, class T> struct StridedElement {
        static StridedArrayView<dimensions - 1, T> get(typename std::conditional<std::is_const<T>::value, const void, void>::type* data, const Size<dimensions>& size, const Stride<dimensions>& stride, std::size_t i) {
            return StridedArrayView<dimensions - 1, T>{
                Size<dimensions - 1>(size._data + 1, typename Implementation::GenerateSequence<dimensions - 1>::Type{}),
                Stride<dimensions - 1>(stride._data + 1, typename Implementation::GenerateSequence<dimensions - 1>::Type{}),
                static_cast<typename std::conditional<std::is_const<T>::value, const char, char>::type*>(data) + std::ptrdiff_t(i)*stride._data[0]};
        }
    };
    template<class T> struct StridedElement<1, T> {
        static T& get(typename std::conditional<std::is_const<T>::value, const void, void>::type* data, const Size1D&, const Stride1D& stride, std::size_t i) {
            return *reinterpret_cast<T*>(static_cast<typename std::conditional<std::is_const<T>::value, const char, char>::type*>(data) + std::ptrdiff_t(i)*stride._data[0]);
        }
    };
}

template<unsigned dimensions, class T> auto StridedArrayView<dimensions, T>::front() const -> ElementType {
    CORRADE_DEBUG_ASSERT(_size._data[0], "Containers::StridedArrayView::front(): view is empty", (Implementation::StridedElement<dimensions, T>::get(_data, _size, _stride, 0)));
    return Implementation::StridedElement<dimensions, T>::get(_data, _size, _stride, 0);
}

template<unsigned dimensions, class T> auto StridedArrayView<dimensions, T>::back() const -> ElementType {
    CORRADE_DEBUG_ASSERT(_size._data[0], "Containers::StridedArrayView::back(): view is empty", (Implementation::StridedElement<dimensions, T>::get(_data, _size, _stride, _size._data[0] - 1)));
    return Implementation::StridedElement<dimensions, T>::get(_data, _size, _stride, _size._data[0] - 1);
}

template<unsigned dimensions, class T> auto StridedArrayView<dimensions, T>::operator[](const std::size_t i) const -> ElementType {
    CORRADE_DEBUG_ASSERT(i < _size._data[0], "Containers::StridedArrayView::operator[](): index" << i << "out of range for" << _size._data[0] << "elements", (Implementation::StridedElement<dimensions, T>::get(_data, _size, _stride, i)));
    return Implementation::StridedElement<dimensions, T>::get(_data, _size, _stride, i);
}

template<unsigned dimensions, class T> T& StridedArrayView<dimensions, T>::operator[](const Containers::Size<dimensions>& i) const {
    auto data = static_cast<ArithmeticType*>(_data);
    for(std::size_t j = 0; j != dimensions; ++j) {
        CORRADE_DEBUG_ASSERT(i._data[j] < _size._data[j],
            "Containers::StridedArrayView::operator[](): index" << i << "out of range for" << _size << "elements", *reinterpret_cast<T*>(static_cast<ArithmeticType*>(_data)));
        data += std::ptrdiff_t(i._data[j])*_stride._data[j];
    }

    return *reinterpret_cast<T*>(data);
}

template<unsigned dimensions, class T> StridedArrayView<dimensions, T> StridedArrayView<dimensions, T>::slice(std::size_t begin, std::size_t end) const {
    CORRADE_DEBUG_ASSERT(begin <= end && end <= _size._data[0],
        "Containers::StridedArrayView::slice(): slice [" << Utility::Debug::nospace
        << begin << Utility::Debug::nospace << ":"
        << Utility::Debug::nospace << end << Utility::Debug::nospace
        << "] out of range for" << _size._data[0] << "elements", {});
    Containers::Size<dimensions> size = _size;
    size._data[0] = std::size_t(end - begin);
    return StridedArrayView<dimensions, T>{size, _stride,
        static_cast<ArithmeticType*>(_data) + std::ptrdiff_t(begin)*_stride._data[0]};
}

template<unsigned dimensions, class T> template<unsigned newDimensions> StridedArrayView<newDimensions, T> StridedArrayView<dimensions, T>::slice(const Containers::Size<dimensions>& begin, const Containers::Size<dimensions>& end) const {
    constexpr unsigned minDimensions = dimensions < newDimensions ? dimensions : newDimensions;
    Containers::Size<newDimensions> size{Corrade::NoInit};
    Containers::Stride<newDimensions> stride{Corrade::NoInit};
    auto data = static_cast<ArithmeticType*>(_data);

    for(std::size_t i = 0; i != dimensions; ++i) {
        CORRADE_ASSERT(begin._data[i] <= end._data[i] && end._data[i] <= _size._data[i],
            "Containers::StridedArrayView::slice(): slice [" << Utility::Debug::nospace
            << begin << Utility::Debug::nospace << ":"
            << Utility::Debug::nospace << end << Utility::Debug::nospace
            << "] out of range for" << _size << "elements in dimension" << i,
            {});
        data += std::ptrdiff_t(begin._data[i])*_stride._data[i];
    }

    for(std::size_t i = 0; i != minDimensions; ++i) {
        size._data[i] = std::size_t(end._data[i] - begin._data[i]);
        stride._data[i] = _stride._data[i];
    }

    for(std::size_t i = minDimensions; i < newDimensions; ++i) {
        size._data[i] = 1;
        stride._data[i] = sizeof(T);
    }

    return StridedArrayView<newDimensions, T>{size, stride, data};
}

namespace Implementation {

template<class T, class U, class V> std::size_t memberFunctionSliceOffset(V U::*memberFunction) {
    static_assert(std::is_base_of<U, T>::value, "expected a member function pointer to the view type or its base classes");

    alignas(U) typename std::conditional<std::is_const<U>::value, const char, char>::type storage[sizeof(U)]{};
    #if defined(CORRADE_TARGET_GCC) && !defined(CORRADE_TARGET_CLANG) && __GNUC__ >= 12
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
    #endif
    const std::size_t offset = reinterpret_cast<const char*>(&(reinterpret_cast<U*>(storage)->*memberFunction)()) - storage;
    #if defined(CORRADE_TARGET_GCC) && !defined(CORRADE_TARGET_CLANG) && __GNUC__ >= 12
    #pragma GCC diagnostic pop
    #endif
    CORRADE_ASSERT(offset < sizeof(U),
        "Containers::StridedArrayView::slice(): member function slice returned offset" << std::ptrdiff_t(offset) << "for a" << sizeof(U) << Utility::Debug::nospace << "-byte type", {});
    return offset;
}

#if defined(CORRADE_TARGET_MSVC) && !defined(CORRADE_TARGET_CLANG_CL)
template<class T, class U, class V, std::size_t size> std::size_t memberFunctionSliceOffset(V(&(U::*memberFunction)())[size]) {
    return memberFunctionSliceOffset<T, U>(reinterpret_cast<V&(U::*)()>(memberFunction));
}
template<class T, class U, class V, std::size_t size> std::size_t memberFunctionSliceOffset(V(&(U::*memberFunction)() &)[size]) {
    return memberFunctionSliceOffset<T, U>(reinterpret_cast<V&(U::*)() &>(memberFunction));
}
template<class T, class U, class V, std::size_t size> std::size_t memberFunctionSliceOffset(V(&(U::*memberFunction)() const)[size]) {
    return memberFunctionSliceOffset<T, U>(reinterpret_cast<V&(U::*)() const>(memberFunction));
}
template<class T, class U, class V, std::size_t size> std::size_t memberFunctionSliceOffset(V(&(U::*memberFunction)() const &)[size]) {
    return memberFunctionSliceOffset<T, U>(reinterpret_cast<V&(U::*)() const &>(memberFunction));
}
#endif

}

template<unsigned dimensions, class T> template<class U, class V, class W, typename std::enable_if<!std::is_const<W>::value, int>::type> StridedArrayView<dimensions, U> StridedArrayView<dimensions, T>::slice(U&(V::*memberFunction)()) const {
    return StridedArrayView<dimensions, U>{_size, _stride, reinterpret_cast<U*>(static_cast<ArithmeticType*>(_data) +
        Implementation::memberFunctionSliceOffset<T>(memberFunction))
    };
}

template<unsigned dimensions, class T> template<class U, class V, class W, typename std::enable_if<!std::is_const<W>::value, int>::type> StridedArrayView<dimensions, U> StridedArrayView<dimensions, T>::slice(U&(V::*memberFunction)() &) const {
    return StridedArrayView<dimensions, U>{_size, _stride, reinterpret_cast<U*>(static_cast<ArithmeticType*>(_data) +
        Implementation::memberFunctionSliceOffset<T>(memberFunction))
    };
}

template<unsigned dimensions, class T> template<class U, class V, class W, typename std::enable_if<std::is_const<W>::value, int>::type> StridedArrayView<dimensions, const U> StridedArrayView<dimensions, T>::slice(const U&(V::*memberFunction)() const) const {
    return StridedArrayView<dimensions, const U>{_size, _stride, reinterpret_cast<const U*>(static_cast<ArithmeticType*>(_data) +
        Implementation::memberFunctionSliceOffset<T>(memberFunction))
    };
}

template<unsigned dimensions, class T> template<class U, class V, class W,
    #if defined(CORRADE_TARGET_GCC) && !defined(CORRADE_TARGET_CLANG) && __GNUC__ < 7
    class
    #else
    typename std::enable_if<std::is_const<W>::value, int>::type
    #endif
> StridedArrayView<dimensions, const U> StridedArrayView<dimensions, T>::slice(const U&(V::*memberFunction)() const &) const {
    return StridedArrayView<dimensions, const U>{_size, _stride, reinterpret_cast<const U*>(static_cast<ArithmeticType*>(_data) +
        Implementation::memberFunctionSliceOffset<T>(memberFunction))
    };
}

template<unsigned dimensions, class T> template<class U> auto StridedArrayView<dimensions, T>::slice(U&& memberFunction) const ->  StridedArrayView<dimensions, typename Implementation::StridedArrayViewSliceResultOf<T, U>::Type> {
    return StridedArrayView<dimensions, typename Implementation::StridedArrayViewSliceResultOf<T, U>::Type>{_size, _stride, reinterpret_cast<typename Implementation::StridedArrayViewSliceResultOf<T, U>::Type*>(static_cast<ArithmeticType*>(_data) +
        Implementation::memberFunctionSliceOffset<T>(memberFunction))
    };
}

template<unsigned dimensions, class T> auto StridedArrayView<dimensions, T>::sliceBit(const std::size_t index) const -> BasicStridedBitArrayView<dimensions, ArithmeticType> {
    CORRADE_ASSERT(index < sizeof(T)*8,
        "Containers::StridedArrayView::sliceBit(): index" << index << "out of range for a" << sizeof(T)*8 << Utility::Debug::nospace << "-bit type", {});

    Containers::Size<dimensions> sizeOffset{Corrade::NoInit};
    Containers::Stride<dimensions> stride{Corrade::NoInit};
    for(std::size_t i = 0; i != dimensions; ++i) {
        CORRADE_DEBUG_ASSERT(_size._data[i] < std::size_t{1} << (sizeof(std::size_t)*8 - 3),
            "Containers::StridedArrayView::sliceBit(): size expected to be smaller than 2^" << Utility::Debug::nospace << (sizeof(std::size_t)*8 - 3) << "bits, got" << _size, {});
        sizeOffset._data[i] = _size._data[i] << 3;
        stride._data[i] = _stride._data[i] << 3;
    }

    ArithmeticType* const data = static_cast<ArithmeticType*>(_data) + (index >> 3);

    sizeOffset._data[0] |= index & 0x07;

    return BasicStridedBitArrayView<dimensions, ArithmeticType>{sizeOffset, stride, data};
}

template<unsigned dimensions, class T> template<unsigned newDimensions> StridedArrayView<newDimensions, T> StridedArrayView<dimensions, T>::sliceSize(const Containers::Size<dimensions>& begin, const Containers::Size<dimensions>& size) const {
    Containers::Size<dimensions> end{Corrade::NoInit};
    for(std::size_t i = 0; i != dimensions; ++i)
        end._data[i] = begin._data[i] + size._data[i];
    return slice<newDimensions>(begin, end);
}

template<unsigned dimensions, class T> template<unsigned newDimensions> StridedArrayView<newDimensions, T> StridedArrayView<dimensions, T>::exceptSuffix(const Containers::Size<dimensions>& size) const {
    Containers::Size<dimensions> end{Corrade::NoInit};
    for(std::size_t i = 0; i != dimensions; ++i)
        end._data[i] = _size._data[i] - size._data[i];
    return slice<newDimensions>({}, end);
}

template<unsigned dimensions, class T> StridedArrayView<dimensions, T> StridedArrayView<dimensions, T>::every(const std::ptrdiff_t step) const {
    Containers::Stride<dimensions> steps;
    steps[0] = step;
    for(std::size_t i = 1; i != dimensions; ++i) steps[i] = 1;
    return every(steps);
}

template<unsigned dimensions, class T> StridedArrayView<dimensions, T> StridedArrayView<dimensions, T>::every(const Containers::Stride<dimensions>& step) const {
    CORRADE_ASSERT(!Implementation::isAnyDimensionZero(step._data, typename Implementation::GenerateSequence<dimensions>::Type{}),
        "Containers::StridedArrayView::every(): expected a non-zero step, got" << step, {});

    ErasedType* data = _data;
    Containers::Size<dimensions> size{Corrade::NoInit};
    Containers::Stride<dimensions> stride = _stride;
    for(std::size_t dimension = 0; dimension != dimensions; ++dimension) {
        std::size_t divisor;
        if(step._data[dimension] < 0) {
            data = static_cast<ArithmeticType*>(data) + _stride._data[dimension]*std::ptrdiff_t(_size._data[dimension] ? _size._data[dimension] - 1 : 0);
            divisor = -step._data[dimension];
        } else divisor = step._data[dimension];

        size._data[dimension] = (_size._data[dimension] + divisor - 1)/divisor;
        stride._data[dimension] *= step._data[dimension];
    }

    return StridedArrayView<dimensions, T>{size, stride, data};
}

template<unsigned dimensions, class T> template<unsigned dimensionA, unsigned dimensionB> StridedArrayView<dimensions, T> StridedArrayView<dimensions, T>::transposed() const {
    static_assert(dimensionA < dimensions && dimensionB < dimensions,
        "dimensions out of range");

    Containers::Size<dimensions> size = _size;
    Containers::Stride<dimensions> stride = _stride;
    Utility::swap(size._data[dimensionA], size._data[dimensionB]);
    Utility::swap(stride._data[dimensionA], stride._data[dimensionB]);
    return StridedArrayView<dimensions, T>{size, stride, _data};
}

template<unsigned dimensions, class T> template<unsigned dimension> StridedArrayView<dimensions, T> StridedArrayView<dimensions, T>::flipped() const {
    static_assert(dimension < dimensions, "dimension out of range");

    ErasedType* data = static_cast<ArithmeticType*>(_data) + _stride._data[dimension]*std::ptrdiff_t(_size._data[dimension] ? _size._data[dimension] - 1 : 0);
    Containers::Stride<dimensions> stride = _stride;
    stride._data[dimension] *= -1;
    return StridedArrayView<dimensions, T>{_size, stride, data};
}

template<unsigned dimensions, class T> template<unsigned dimension> StridedArrayView<dimensions, T> StridedArrayView<dimensions, T>::broadcasted(std::size_t size) const {
    static_assert(dimension < dimensions, "dimension out of range");
    CORRADE_ASSERT(_size._data[dimension] == 1,
        "Containers::StridedArrayView::broadcasted(): can't broadcast dimension" << dimension << "with" << _size._data[dimension] << "elements", {});

    Containers::Size<dimensions> size_ = _size;
    size_._data[dimension] = size;
    Containers::Stride<dimensions> stride = _stride;
    stride._data[dimension] = 0;
    return StridedArrayView<dimensions, T>{size_, stride, _data};
}

template<unsigned dimensions, class T> template<unsigned dimension, unsigned count> StridedArrayView<dimensions + count - 1, T> StridedArrayView<dimensions, T>::expanded(const Containers::Size<count>& size) const {
    static_assert(dimension < dimensions, "dimension out of range");

    Containers::Size<dimensions + count - 1> size_{Corrade::NoInit};
    Containers::Stride<dimensions + count - 1> stride_{Corrade::NoInit};

    for(std::size_t i = 0; i != dimension; ++i) {
        size_._data[i] = _size._data[i];
        stride_._data[i] = _stride._data[i];
    }
    for(std::size_t i = dimension + 1; i != dimensions; ++i) {
        size_._data[i + count - 1] = _size._data[i];
        stride_._data[i + count - 1] = _stride._data[i];
    }

    std::size_t totalSize = 1;
    const std::ptrdiff_t baseStride = _stride._data[dimension];
    for(std::size_t i = count; i != 0; --i) {
        size_._data[dimension + i - 1] = size._data[i - 1];
        stride_._data[dimension + i - 1] = baseStride*std::ptrdiff_t(totalSize);
        totalSize *= size._data[i - 1];
    }
    CORRADE_ASSERT(totalSize == _size._data[dimension],
        "Containers::StridedArrayView::expanded(): product of" << size << "doesn't match" << _size._data[dimension] << "elements in dimension" << dimension, {});

    return StridedArrayView<dimensions + count - 1, T>{size_, stride_, _data};
}

template<unsigned dimensions, class T> template<unsigned dimension, unsigned count> StridedArrayView<dimensions - count + 1, T> StridedArrayView<dimensions, T>::collapsed() const {
    static_assert(dimension + count <= dimensions, "dimension + count out of range");

    Containers::Size<dimensions - count + 1> size_{Corrade::NoInit};
    Containers::Stride<dimensions - count + 1> stride_{Corrade::NoInit};

    for(std::size_t i = 0; i != dimension; ++i) {
        size_._data[i] = _size._data[i];
        stride_._data[i] = _stride._data[i];
    }
    for(std::size_t i = dimension + count; i != dimensions; ++i) {
        size_._data[i - count + 1] = _size._data[i];
        stride_._data[i - count + 1] = _stride._data[i];
    }

    std::size_t totalSize = 1;
    const std::ptrdiff_t baseStride = _stride._data[dimension + count - 1];
    for(std::size_t i = dimension + count; i != dimension; --i) {
        CORRADE_ASSERT(_stride._data[i - 1] == std::ptrdiff_t(totalSize)*baseStride,
            "Containers::StridedArrayView::collapsed(): expected dimension" << i - 1 << "stride to be" << std::ptrdiff_t(totalSize)*baseStride << "but got" << _stride._data[i - 1], {});
        totalSize *= _size._data[i - 1];
    }
    size_._data[dimension] = totalSize;
    stride_._data[dimension] = baseStride;

    return StridedArrayView<dimensions - count + 1, T>{size_, stride_, _data};
}

}}

#endif
#ifdef CORRADE_STRUCTURED_BINDINGS
namespace std {

#ifndef Corrade_Containers_StructuredBindings_StridedDimensions_h
#define Corrade_Containers_StructuredBindings_StridedDimensions_h
template<unsigned dimensions, class T> struct tuple_size<Corrade::Containers::StridedDimensions<dimensions, T>>: integral_constant<size_t, dimensions> {};
template<unsigned dimensions, class T> struct tuple_size<const Corrade::Containers::StridedDimensions<dimensions, T>>: integral_constant<size_t, dimensions> {};
template<size_t index, unsigned dimensions, class T> struct tuple_element<index, Corrade::Containers::StridedDimensions<dimensions, T>> { typedef T type; };
template<size_t index, unsigned dimensions, class T> struct tuple_element<index, const Corrade::Containers::StridedDimensions<dimensions, T>> { typedef const T type; };
#endif

}
#endif
