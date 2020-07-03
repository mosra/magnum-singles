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

    Generated from Corrade v2020.06-0-g61d1b58c (2020-06-27), 911 / 3245 LoC
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

#include "CorradeArrayView.h"

#if defined(_MSC_VER) && _MSC_VER <= 1900
#define CORRADE_MSVC2015_COMPATIBILITY
#endif

#ifndef CorradeStridedArrayView_h
#define CorradeStridedArrayView_h

namespace Corrade { namespace Containers {

template<unsigned, class> class StridedDimensions;
template<unsigned, class> class StridedArrayView;
template<unsigned, class> class StridedIterator;
template<class T> using StridedArrayView1D = StridedArrayView<1, T>;
template<class T> using StridedArrayView2D = StridedArrayView<2, T>;
template<class T> using StridedArrayView3D = StridedArrayView<3, T>;
template<class T> using StridedArrayView4D = StridedArrayView<4, T>;

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
#ifndef Corrade_Containers_StridedArrayView_h
#define Corrade_Containers_StridedArrayView_h

namespace Corrade { namespace Containers {

namespace Implementation {
    template<std::size_t ...> struct Sequence {};

    template<std::size_t N, std::size_t ...sequence> struct GenerateSequence:
        GenerateSequence<N-1, N-1, sequence...> {};

    template<std::size_t ...sequence> struct GenerateSequence<0, sequence...> {
        typedef Sequence<sequence...> Type;
    };

    template<unsigned, class> struct StridedElement;
    template<int> struct ArrayCastFlattenOrInflate;

    template<unsigned dimensions> constexpr bool isAnySizeZero(const StridedDimensions<dimensions, std::size_t>&, Sequence<>) {
        return false;
    }
    template<unsigned dimensions, std::size_t first, std::size_t ...next> constexpr bool isAnySizeZero(const StridedDimensions<dimensions, std::size_t>& size, Sequence<first, next...>) {
        return !size[first] || isAnySizeZero(size, Sequence<next...>{});
    }
    template<unsigned dimensions> constexpr std::size_t largestStride(const StridedDimensions<dimensions, std::size_t>&, const StridedDimensions<dimensions, std::ptrdiff_t>&, Sequence<>) {
        return 0;
    }
    constexpr std::size_t largerStride(std::size_t a, std::size_t b) {
        return a < b ? b : a;
    }
    template<unsigned dimensions, std::size_t first, std::size_t ...next> constexpr std::size_t largestStride(const StridedDimensions<dimensions, std::size_t>& size, const StridedDimensions<dimensions, std::ptrdiff_t>& stride, Sequence<first, next...>) {
        return largerStride(size[first]*std::size_t(stride[first] < 0 ? -stride[first] : stride[first]),
            largestStride(size, stride, Sequence<next...>{}));
    }

    template<unsigned dimensions> constexpr std::ptrdiff_t strideForSizeInternal(const StridedDimensions<dimensions, std::size_t>&, std::size_t, Sequence<>) {
        return 1;
    }
    template<unsigned dimensions, std::size_t first, std::size_t ...next> constexpr std::ptrdiff_t strideForSizeInternal(const StridedDimensions<dimensions, std::size_t>& size, std::size_t index, Sequence<first, next...>) {
        return (first > index ? size[first] : 1)*strideForSizeInternal(size, index, Sequence<next...>{});
    }
    template<unsigned dimensions, std::size_t ...index> constexpr StridedDimensions<dimensions, std::ptrdiff_t> strideForSize(const StridedDimensions<dimensions, std::size_t>& size, std::size_t typeSize, Sequence<index...>) {
        return {std::ptrdiff_t(typeSize)*strideForSizeInternal(size, index, typename GenerateSequence<dimensions>::Type{})...};
    }
}

template<unsigned dimensions, class T> class StridedDimensions {
    public:
        constexpr /*implicit*/ StridedDimensions() noexcept: _data{} {}

        constexpr explicit StridedDimensions(ValueInitT) noexcept: _data{} {}

        explicit StridedDimensions(NoInitT) noexcept {}

        template<class ...Args> constexpr /*implicit*/ StridedDimensions(T first, Args... next) noexcept: _data{T(first), T(next)...} {
            static_assert(sizeof...(Args) + 1 == dimensions, "wrong value count");
        }

        constexpr /*implicit*/ StridedDimensions(StaticArrayView<dimensions, const T> values) noexcept: StridedDimensions{values.data(), typename Implementation::GenerateSequence<dimensions>::Type{}} {}

        constexpr /*implicit*/ StridedDimensions(const T(&values)[dimensions]) noexcept: StridedDimensions{values, typename Implementation::GenerateSequence<dimensions>::Type{}} {}

        constexpr /*implicit*/ operator StaticArrayView<dimensions, const T>() const {
            return staticArrayView(_data);
        }

        template<unsigned d = dimensions, class = typename std::enable_if<d == 1>::type>
        constexpr /*implicit*/ operator T() const { return _data[0]; }

        bool operator==(const StridedDimensions<dimensions, T>& other) const {
            for(std::size_t i = 0; i != dimensions; ++i)
                if(_data[i] != other._data[i]) return false;
            return true;
        }

        bool operator!=(const StridedDimensions<dimensions, T>& other) const {
            return !operator==(other);
        }

        constexpr T operator[](std::size_t i) const {
            return CORRADE_CONSTEXPR_ASSERT(i < dimensions,
                "Containers::StridedDimensions::operator[](): dimension" << i << "out of range for" << dimensions << "dimensions"), _data[i];
        }

        T& operator[](std::size_t i) {
            CORRADE_ASSERT(i < dimensions,
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
        template<unsigned, class> friend struct Implementation::StridedElement;
        template<int> friend struct Implementation::ArrayCastFlattenOrInflate;
        template<class U, unsigned dimensions_, class T_> friend StridedArrayView<dimensions_, U> arrayCast(const StridedArrayView<dimensions_, T_>&);
        template<class U, unsigned dimensions_> friend StridedArrayView<dimensions_, U> arrayCast(const StridedArrayView<dimensions_, void>&);
        template<class U, unsigned dimensions_> friend StridedArrayView<dimensions_, U> arrayCast(const StridedArrayView<dimensions_, const void>&);
        template<unsigned newDimensions, class U, unsigned dimensions_> StridedArrayView<newDimensions, U> friend arrayCast(const StridedArrayView<dimensions_, void>&, std::size_t);
        template<unsigned newDimensions, class U, unsigned dimensions_> StridedArrayView<newDimensions, U> friend arrayCast(const StridedArrayView<dimensions_, const void>&, std::size_t);

        template<class U, std::size_t ...sequence> constexpr explicit StridedDimensions(const U* values, Implementation::Sequence<sequence...>) noexcept: _data{T(values[sequence])...} {}

        T _data[dimensions];
};

template<unsigned dimensions, class T> class StridedArrayView {
    static_assert(dimensions, "can't have a zero-dimensional view");

    public:
        typedef T Type;

        typedef typename std::conditional<dimensions == 1, T&, StridedArrayView<dimensions - 1, T>>::type ElementType;

        typedef typename std::conditional<std::is_const<T>::value, const void, void>::type ErasedType;

        typedef StridedDimensions<dimensions, std::size_t> Size;

        typedef StridedDimensions<dimensions, std::ptrdiff_t> Stride;

        enum: unsigned {
            Dimensions = dimensions
        };

        constexpr /*implicit*/ StridedArrayView(std::nullptr_t) noexcept: _data{}, _size{}, _stride{} {}

        constexpr /*implicit*/ StridedArrayView() noexcept: _data{}, _size{}, _stride{} {}

        constexpr /*implicit*/ StridedArrayView(ArrayView<ErasedType> data, T* member, const Size& size, const Stride& stride) noexcept: _data{(
            CORRADE_CONSTEXPR_ASSERT(Implementation::isAnySizeZero(size, typename Implementation::GenerateSequence<dimensions>::Type{}) || Implementation::largestStride(size, stride, typename Implementation::GenerateSequence<dimensions>::Type{}) <= data.size(),
                "Containers::StridedArrayView: data size" << data.size() << "is not enough for" << size << "elements of stride" << stride)
            , member)}, _size{size}, _stride{stride} {}

        constexpr /*implicit*/ StridedArrayView(ArrayView<T> data, const Size& size, const Stride& stride) noexcept: StridedArrayView{data, data.data(), size, stride} {}

        constexpr /*implicit*/ StridedArrayView(ArrayView<T> data, const Size& size) noexcept: StridedArrayView{data, data.data(), size, Implementation::strideForSize(size, sizeof(T), typename Implementation::GenerateSequence<dimensions>::Type{})} {}

        template<class U, std::size_t size, unsigned d = dimensions, class = typename std::enable_if<d == 1 && std::is_convertible<U*, T*>::value>::type>
        constexpr /*implicit*/ StridedArrayView(U(&data)[size]) noexcept: _data{data}, _size{size}, _stride{sizeof(T)} {
            static_assert(sizeof(T) == sizeof(U), "type sizes are not compatible");
        }

        template<class U, class = typename std::enable_if<std::is_convertible<U*, T*>::value>::type>
        constexpr /*implicit*/ StridedArrayView(StridedArrayView<dimensions, U> view) noexcept: _data{view._data}, _size{view._size}, _stride{view._stride} {
            static_assert(sizeof(T) == sizeof(U), "type sizes are not compatible");
        }

        template<class U, unsigned d = dimensions, class = typename std::enable_if<d == 1 && std::is_convertible<U*, T*>::value>::type>
        constexpr /*implicit*/ StridedArrayView(ArrayView<U> view) noexcept: _data{view.data()}, _size{view.size()}, _stride{sizeof(T)} {
            static_assert(sizeof(T) == sizeof(U), "type sizes are not compatible");
        }

        template<std::size_t size, class U, unsigned d = dimensions, class = typename std::enable_if<d == 1 && std::is_convertible<U*, T*>::value>::type>
        constexpr /*implicit*/ StridedArrayView(StaticArrayView<size, U> view) noexcept: _data{view.data()}, _size{size}, _stride{sizeof(T)} {
            static_assert(sizeof(U) == sizeof(T), "type sizes are not compatible");
        }

        template<class U, unsigned d = dimensions, class = typename std::enable_if<d == 1, decltype(Implementation::ArrayViewConverter<T, typename std::decay<U&&>::type>::from(std::declval<U&&>()))>::type> constexpr /*implicit*/ StridedArrayView(U&& other) noexcept: StridedArrayView{Implementation::ArrayViewConverter<T, typename std::decay<U&&>::type>::from(std::forward<U>(other))} {}

        constexpr explicit operator bool() const { return _data; }

        constexpr ErasedType* data() const { return _data; }

        constexpr typename std::conditional<dimensions == 1, std::size_t, const Size&>::type size() const { return _size; }

        constexpr typename std::conditional<dimensions == 1, std::ptrdiff_t, const Stride&>::type stride() const { return _stride; }

        constexpr StridedDimensions<dimensions, bool> empty() const {
            return emptyInternal(typename Implementation::GenerateSequence<dimensions>::Type{});
        }

        template<unsigned dimension = 0> bool isContiguous() const;

        ArrayView<T> asContiguous() const;

        ElementType operator[](std::size_t i) const;

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

        StridedArrayView<dimensions, T> slice(std::size_t begin, std::size_t end) const;

        template<unsigned newDimensions = dimensions> StridedArrayView<newDimensions, T> slice(const Size& begin, const Size& end) const;

        template<unsigned newDimensions = dimensions> StridedArrayView<newDimensions, T> slice() const {
            return slice<newDimensions>({}, _size);
        }

        StridedArrayView<dimensions, T> prefix(std::size_t end) const {
            return slice(0, end);
        }

        template<unsigned newDimensions = dimensions> StridedArrayView<newDimensions, T> prefix(const Size& end) const {
            return slice<newDimensions>({}, end);
        }

        StridedArrayView<dimensions, T> suffix(std::size_t begin) const {
            return slice(begin, _size._data[0]);
        }

        template<unsigned newDimensions = dimensions> StridedArrayView<newDimensions, T> suffix(const Size& begin) const {
            return slice<newDimensions>(begin, _size);
        }

        StridedArrayView<dimensions, T> except(std::size_t count) const {
            return slice({}, _size._data[0] - count);
        }

        template<unsigned newDimensions = dimensions> StridedArrayView<newDimensions, T> except(const Size& count) const;

        StridedArrayView<dimensions, T> every(std::ptrdiff_t skip) const;

        StridedArrayView<dimensions, T> every(const Stride& skip) const;

        template<unsigned dimensionA, unsigned dimensionB> StridedArrayView<dimensions, T> transposed() const;

        template<unsigned dimension> StridedArrayView<dimensions, T> flipped() const;

        template<unsigned dimension> StridedArrayView<dimensions, T> broadcasted(std::size_t size) const;

    private:
        template<unsigned, class> friend class StridedArrayView;

        template<unsigned, class> friend struct Implementation::StridedElement;
        template<int> friend struct Implementation::ArrayCastFlattenOrInflate;
        template<class U, unsigned dimensions_, class T_> friend StridedArrayView<dimensions_, U> arrayCast(const StridedArrayView<dimensions_, T_>&);
        template<class U, unsigned dimensions_> friend StridedArrayView<dimensions_, U> arrayCast(const StridedArrayView<dimensions_, void>&);
        template<class U, unsigned dimensions_> friend StridedArrayView<dimensions_, U> arrayCast(const StridedArrayView<dimensions_, const void>&);
        template<unsigned newDimensions, class U, unsigned dimensions_> StridedArrayView<newDimensions, U> friend arrayCast(const StridedArrayView<dimensions_, void>&, std::size_t);
        template<unsigned newDimensions, class U, unsigned dimensions_> StridedArrayView<newDimensions, U> friend arrayCast(const StridedArrayView<dimensions_, const void>&, std::size_t);

        constexpr /*implicit*/ StridedArrayView(const Size& size, const Stride& stride, ErasedType* data) noexcept: _data{data}, _size{size}, _stride{stride} {}

        template<std::size_t ...sequence> constexpr StridedDimensions<dimensions, bool> emptyInternal(Implementation::Sequence<sequence...>) const {
            return StridedDimensions<dimensions, bool>{(_size._data[sequence] == 0)...};
        }

        ErasedType* _data;
        Size _size;
        Stride _stride;
};

template<unsigned dimensions> class StridedArrayView<dimensions, void> {
    static_assert(dimensions, "can't have a zero-dimensional view");

    public:
        typedef void Type;

        typedef void ErasedType;

        typedef StridedDimensions<dimensions, std::size_t> Size;

        typedef StridedDimensions<dimensions, std::ptrdiff_t> Stride;

        enum: unsigned {
            Dimensions = dimensions
        };

        constexpr /*implicit*/ StridedArrayView(std::nullptr_t) noexcept: _data{}, _size{}, _stride{} {}

        constexpr /*implicit*/ StridedArrayView() noexcept: _data{}, _size{}, _stride{} {}

        constexpr /*implicit*/ StridedArrayView(ArrayView<void> data, void* member, const Size& size, const Stride& stride) noexcept: _data{(
            CORRADE_CONSTEXPR_ASSERT(Implementation::isAnySizeZero(size, typename Implementation::GenerateSequence<dimensions>::Type{}) || Implementation::largestStride(size, stride, typename Implementation::GenerateSequence<dimensions>::Type{}) <= data.size(),
                "Containers::StridedArrayView: data size" << data.size() << "is not enough for" << size << "elements of stride" << stride)
            , member)}, _size{size}, _stride{stride} {}

        constexpr /*implicit*/ StridedArrayView(ArrayView<void> data, const Size& size, const Stride& stride) noexcept: StridedArrayView{data, data.data(), size, stride} {}

        template<class T, std::size_t size
            , unsigned d = dimensions, class = typename std::enable_if<d == 1 && !std::is_const<T>::value>::type
        > constexpr /*implicit*/ StridedArrayView(T(&data)[size]) noexcept: _data{data}, _size{size}, _stride{sizeof(T)} {}

        template<class T
            , class = typename std::enable_if<!std::is_const<T>::value>::type
        > constexpr /*implicit*/ StridedArrayView(StridedArrayView<dimensions, T> view) noexcept: _data{view._data}, _size{view._size}, _stride{view._stride} {}

        template<class T
            , unsigned d = dimensions, class = typename std::enable_if<d == 1 && !std::is_const<T>::value>::type
        > constexpr /*implicit*/ StridedArrayView(ArrayView<T> view) noexcept: _data{view.data()}, _size{view.size()}, _stride{sizeof(T)} {}

        template<std::size_t size, class T
            , unsigned d = dimensions, class = typename std::enable_if<d == 1 && !std::is_const<T>::value>::type
        > constexpr /*implicit*/ StridedArrayView(StaticArrayView<size, T> view) noexcept: _data{view.data()}, _size{size}, _stride{sizeof(T)} {}

        template<class T, unsigned d = dimensions, class = typename std::enable_if<d == 1, decltype(Implementation::ErasedArrayViewConverter<typename std::decay<T&&>::type>::from(std::declval<T&&>()))>::type> constexpr /*implicit*/ StridedArrayView(T&& other) noexcept: StridedArrayView{Implementation::ErasedArrayViewConverter<typename std::decay<T&&>::type>::from(other)} {}

        constexpr explicit operator bool() const { return _data; }

        constexpr void* data() const { return _data; }

        constexpr typename std::conditional<dimensions == 1, std::size_t, const Size&>::type size() const { return _size; }

        constexpr typename std::conditional<dimensions == 1, std::ptrdiff_t, const Stride&>::type stride() const { return _stride; }

        constexpr StridedDimensions<dimensions, bool> empty() const {
            return emptyInternal(typename Implementation::GenerateSequence<dimensions>::Type{});
        }

    private:
        template<unsigned, class> friend class StridedArrayView;

        constexpr /*implicit*/ StridedArrayView(const Size& size, const Stride& stride, void* data) noexcept: _data{data}, _size{size}, _stride{stride} {}

        template<std::size_t ...sequence> constexpr StridedDimensions<dimensions, bool> emptyInternal(Implementation::Sequence<sequence...>) const {
            return StridedDimensions<dimensions, bool>{(_size._data[sequence] == 0)...};
        }

        void* _data;
        Size _size;
        Stride _stride;
};

template<unsigned dimensions> class StridedArrayView<dimensions, const void> {
    static_assert(dimensions, "can't have a zero-dimensional view");

    public:
        typedef const void Type;

        typedef const void ErasedType;

        typedef StridedDimensions<dimensions, std::size_t> Size;

        typedef StridedDimensions<dimensions, std::ptrdiff_t> Stride;

        enum: unsigned {
            Dimensions = dimensions
        };

        constexpr /*implicit*/ StridedArrayView(std::nullptr_t) noexcept: _data{}, _size{}, _stride{} {}

        constexpr /*implicit*/ StridedArrayView() noexcept: _data{}, _size{}, _stride{} {}

        constexpr /*implicit*/ StridedArrayView(ArrayView<const void> data, const void* member, const Size& size, const Stride& stride) noexcept: _data{(
            CORRADE_CONSTEXPR_ASSERT(Implementation::isAnySizeZero(size, typename Implementation::GenerateSequence<dimensions>::Type{}) || Implementation::largestStride(size, stride, typename Implementation::GenerateSequence<dimensions>::Type{}) <= data.size(),
                "Containers::StridedArrayView: data size" << data.size() << "is not enough for" << size << "elements of stride" << stride)
            , member)}, _size{size}, _stride{stride} {}

        constexpr /*implicit*/ StridedArrayView(ArrayView<const void> data, const Size& size, const Stride& stride) noexcept: StridedArrayView{data, data.data(), size, stride} {}

        template<class T, std::size_t size, unsigned d = dimensions, class = typename std::enable_if<d == 1>::type>
        constexpr /*implicit*/ StridedArrayView(T(&data)[size]) noexcept: _data{data}, _size{size}, _stride{sizeof(T)} {}

        template<class T> constexpr /*implicit*/ StridedArrayView(StridedArrayView<dimensions, T> view) noexcept: _data{view._data}, _size{view._size}, _stride{view._stride} {}

        template<class T, unsigned d = dimensions, class = typename std::enable_if<d == 1>::type>
        constexpr /*implicit*/ StridedArrayView(ArrayView<T> view) noexcept: _data{view.data()}, _size{view.size()}, _stride{sizeof(T)} {}

        template<std::size_t size, class T, unsigned d = dimensions, class = typename std::enable_if<d == 1>::type>
        constexpr /*implicit*/ StridedArrayView(StaticArrayView<size, T> view) noexcept: _data{view.data()}, _size{size}, _stride{sizeof(T)} {}

        template<class T, unsigned d = dimensions, class = typename std::enable_if<d == 1, decltype(Implementation::ErasedArrayViewConverter<const T>::from(std::declval<const T&>()))>::type> constexpr /*implicit*/ StridedArrayView(const T& other) noexcept: StridedArrayView{Implementation::ErasedArrayViewConverter<const T>::from(other)} {}

        constexpr explicit operator bool() const { return _data; }

        constexpr const void* data() const { return _data; }

        constexpr typename std::conditional<dimensions == 1, std::size_t, const Size&>::type size() const { return _size; }

        constexpr typename std::conditional<dimensions == 1, std::ptrdiff_t, const Stride&>::type stride() const { return _stride; }

        constexpr StridedDimensions<dimensions, bool> empty() const {
            return emptyInternal(typename Implementation::GenerateSequence<dimensions>::Type{});
        }

    private:
        template<unsigned, class> friend class StridedArrayView;

        template<class U, unsigned dimensions_> friend StridedArrayView<dimensions_, U> arrayCast(const StridedArrayView<dimensions_, const void>&);
        template<unsigned newDimensions, class U, unsigned dimensions_> StridedArrayView<newDimensions, U> friend arrayCast(const StridedArrayView<dimensions_, const void>&, std::size_t);

        constexpr /*implicit*/ StridedArrayView(const Size& size, const Stride& stride, const void* data) noexcept: _data{data}, _size{size}, _stride{stride} {}

        template<std::size_t ...sequence> constexpr StridedDimensions<dimensions, bool> emptyInternal(Implementation::Sequence<sequence...>) const {
            return StridedDimensions<dimensions, bool>{(_size._data[sequence] == 0)...};
        }

        const void* _data;
        Size _size;
        Stride _stride;
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
    return Implementation::ErasedArrayViewConverter<typename std::remove_reference<T&&>::type>::from(std::forward<T>(other));
}

template<class U, unsigned dimensions, class T> StridedArrayView<dimensions, U> arrayCast(const StridedArrayView<dimensions, T>& view) {
    static_assert(std::is_standard_layout<T>::value, "the source type is not standard layout");
    static_assert(std::is_standard_layout<U>::value, "the target type is not standard layout");
    return StridedArrayView<dimensions, U>{view._size, view._stride, view._data};
}

template<class U, unsigned dimensions> StridedArrayView<dimensions, U> arrayCast(const StridedArrayView<dimensions, const void>& view) {
    static_assert(std::is_standard_layout<U>::value, "the target type is not standard layout");
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
        CORRADE_ASSERT(sizeof(T) == std::size_t(view._stride[dimensions - 1]),
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
        CORRADE_ASSERT(sizeof(T) == std::size_t(view._stride[dimensions - 1]),
            "Containers::arrayCast(): last dimension needs to be contiguous in order to be flattened, expected stride" << sizeof(T) << "but got" << view.stride()[dimensions - 1], {});
        CORRADE_ASSERT(sizeof(T)*view._size._data[dimensions - 1] % sizeof(U) == 0,
            "Containers::arrayCast(): last dimension needs to have byte size divisible by new type size in order to be flattened, but for a" << sizeof(U) << Utility::Debug::nospace << "-byte type got" << sizeof(T)*view._size._data[dimensions - 1], {});

        StridedDimensions<dimensions, std::size_t> size;
        StridedDimensions<dimensions, std::ptrdiff_t> stride;
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
        StridedDimensions<dimensions + 1, std::size_t> size;
        StridedDimensions<dimensions + 1, std::ptrdiff_t> stride;
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

template<unsigned dimensions, class T> class StridedIterator {
    public:
        typedef T Type;

        typedef typename std::conditional<dimensions == 1, T&, StridedArrayView<dimensions - 1, T>>::type ElementType;

        /*implicit*/ StridedIterator(typename std::conditional<std::is_const<T>::value, const void, void>::type* data, const StridedDimensions<dimensions, std::size_t>& size, const StridedDimensions<dimensions, std::ptrdiff_t>& stride, std::size_t i) noexcept: _data{data}, _size{size}, _stride{stride}, _i{i} {}

        bool operator==(StridedIterator<dimensions, T> other) const {
            return _data == other._data && _i == other._i;
        }

        bool operator!=(StridedIterator<dimensions, T> other) const {
            return _data != other._data || _i != other._i;
        }

        bool operator<(StridedIterator<dimensions, T> other) const {
            return _data == other._data && _i < other._i;
        }

        bool operator<=(StridedIterator<dimensions, T> other) const {
            return _data == other._data && _i <= other._i;
        }

        bool operator>(StridedIterator<dimensions, T> other) const {
            return _data == other._data && _i > other._i;
        }

        bool operator>=(StridedIterator<dimensions, T> other) const {
            return _data == other._data && _i >= other._i;
        }

        StridedIterator<dimensions, T> operator+(std::ptrdiff_t i) const {
            return {_data, _size, _stride, _i + i};
        }

        StridedIterator<dimensions, T> operator-(std::ptrdiff_t i) const {
            return {_data, _size, _stride, _i - i};
        }

        std::ptrdiff_t operator-(StridedIterator<dimensions, T> it) const {
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
        StridedDimensions<dimensions, std::size_t> _size;
        StridedDimensions<dimensions, std::ptrdiff_t> _stride;
        std::size_t _i;
};

template<unsigned dimensions, class T> inline StridedIterator<dimensions, T> operator+(std::ptrdiff_t i, StridedIterator<dimensions, T> it) {
    return it + i;
}

template<unsigned dimensions, class T> template<unsigned dimension> bool StridedArrayView<dimensions, T>::isContiguous() const {
    static_assert(dimension < dimensions, "dimension out of bounds");
    std::size_t nextDimensionSize = sizeof(T);
    for(std::size_t i = dimensions; i != dimension; --i) {
        if(std::size_t(_stride[i - 1]) != nextDimensionSize) return false;
        nextDimensionSize *= _size[i - 1];
    }

    return true;
}

template<unsigned dimensions, class T> ArrayView<T> StridedArrayView<dimensions, T>::asContiguous() const {
    CORRADE_ASSERT(isContiguous(), "Containers::StridedArrayView::asContiguous(): the view is not contiguous", {});
    std::size_t size = _size[0];
    for(std::size_t i = 1; i != dimensions; ++i) size *= _size[i];
    return {static_cast<T*>(_data), size};
}

namespace Implementation {
    template<unsigned dimensions, class T> struct StridedElement {
        static StridedArrayView<dimensions - 1, T> get(typename std::conditional<std::is_const<T>::value, const void, void>::type* data, const StridedDimensions<dimensions, std::size_t>& size, const StridedDimensions<dimensions, std::ptrdiff_t>& stride, std::size_t i) {
            return StridedArrayView<dimensions - 1, T>{
                StridedDimensions<dimensions - 1, std::size_t>(size._data + 1, typename Implementation::GenerateSequence<dimensions - 1>::Type{}),
                StridedDimensions<dimensions - 1, std::ptrdiff_t>(stride._data + 1, typename Implementation::GenerateSequence<dimensions - 1>::Type{}),
                static_cast<typename std::conditional<std::is_const<T>::value, const char, char>::type*>(data) + i*stride._data[0]};
        }
    };
    template<class T> struct StridedElement<1, T> {
        static T& get(typename std::conditional<std::is_const<T>::value, const void, void>::type* data, const StridedDimensions<1, std::size_t>&, const StridedDimensions<1, std::ptrdiff_t>& stride, std::size_t i) {
            return *(reinterpret_cast<T*>(static_cast<typename std::conditional<std::is_const<T>::value, const char, char>::type*>(data) + i*stride._data[0]));
        }
    };
}

template<unsigned dimensions, class T> auto StridedArrayView<dimensions, T>::operator[](const std::size_t i) const -> ElementType {
    CORRADE_ASSERT(i < _size._data[0], "Containers::StridedArrayView::operator[](): index" << i << "out of range for" << _size._data[0] << "elements", (Implementation::StridedElement<dimensions, T>::get(_data, _size, _stride, i)));
    return Implementation::StridedElement<dimensions, T>::get(_data, _size, _stride, i);
}

template<unsigned dimensions, class T> auto StridedArrayView<dimensions, T>::front() const -> ElementType {
    CORRADE_ASSERT(_size[0], "Containers::StridedArrayView::front(): view is empty", (Implementation::StridedElement<dimensions, T>::get(_data, _size, _stride, 0)));
    return Implementation::StridedElement<dimensions, T>::get(_data, _size, _stride, 0);
}

template<unsigned dimensions, class T> auto StridedArrayView<dimensions, T>::back() const -> ElementType {
    CORRADE_ASSERT(_size[0], "Containers::StridedArrayView::back(): view is empty", (Implementation::StridedElement<dimensions, T>::get(_data, _size, _stride, _size._data[0] - 1)));
    return Implementation::StridedElement<dimensions, T>::get(_data, _size, _stride, _size._data[0] - 1);
}

template<unsigned dimensions, class T> StridedArrayView<dimensions, T> StridedArrayView<dimensions, T>::slice(std::size_t begin, std::size_t end) const {
    CORRADE_ASSERT(begin <= end && end <= _size._data[0],
        "Containers::StridedArrayView::slice(): slice [" << Utility::Debug::nospace
        << begin << Utility::Debug::nospace << ":"
        << Utility::Debug::nospace << end << Utility::Debug::nospace
        << "] out of range for" << _size._data[0] << "elements", {});
    Size size = _size;
    size._data[0] = std::size_t(end - begin);
    return StridedArrayView<dimensions, T>{size, _stride,
        static_cast<typename std::conditional<std::is_const<T>::value, const char, char>::type*>(_data) + begin*_stride[0]};
}

template<unsigned dimensions, class T> template<unsigned newDimensions> StridedArrayView<newDimensions, T> StridedArrayView<dimensions, T>::slice(const Size& begin, const Size& end) const {
    constexpr unsigned minDimensions = dimensions < newDimensions ? dimensions : newDimensions;
    StridedDimensions<newDimensions, std::size_t> size{NoInit};
    StridedDimensions<newDimensions, std::ptrdiff_t> stride{NoInit};
    auto data = static_cast<typename std::conditional<std::is_const<T>::value, const char, char>::type*>(_data);

    for(std::size_t i = 0; i != dimensions; ++i) {
        CORRADE_ASSERT(begin._data[i] <= end._data[i] && end._data[i] <= _size._data[i],
            "Containers::StridedArrayView::slice(): slice [" << Utility::Debug::nospace
            << begin << Utility::Debug::nospace << ":"
            << Utility::Debug::nospace << end << Utility::Debug::nospace
            << "] out of range for" << _size << "elements in dimension" << i,
            {});
        data += begin._data[i]*_stride[i];
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

template<unsigned dimensions, class T> template<unsigned newDimensions> StridedArrayView<newDimensions, T> StridedArrayView<dimensions, T>::except(const Size& count) const {
    Size end{NoInit};
    for(std::size_t i = 0; i != dimensions; ++i)
        end._data[i] = _size._data[i] - count._data[i];
    return slice<newDimensions>({}, end);
}

template<unsigned dimensions, class T> StridedArrayView<dimensions, T> StridedArrayView<dimensions, T>::every(const std::ptrdiff_t step) const {
    Stride steps;
    steps[0] = step;
    for(std::size_t i = 1; i != dimensions; ++i) steps[i] = 1;
    return every(steps);
}

template<unsigned dimensions, class T> StridedArrayView<dimensions, T> StridedArrayView<dimensions, T>::every(const Stride& step) const {
    ErasedType* data = _data;
    Size size = _size;
    Stride stride = _stride;
    for(std::size_t dimension = 0; dimension != dimensions; ++dimension) {
        CORRADE_ASSERT(step[dimension], "Containers::StridedArrayView::every(): step in dimension" << dimension << "is zero", {});

        std::size_t divisor;
        if(step[dimension] < 0) {
            data = static_cast<typename std::conditional<std::is_const<T>::value, const char, char>::type*>(data) + _stride._data[dimension]*(_size._data[dimension] ? _size._data[dimension] - 1 : 0);
            divisor = -step[dimension];
        } else divisor = step[dimension];

        size[dimension] = (size[dimension] + divisor - 1)/divisor;
        stride[dimension] *= step[dimension];
    }

    return StridedArrayView<dimensions, T>{size, stride, data};
}

template<unsigned dimensions, class T> template<unsigned dimensionA, unsigned dimensionB> StridedArrayView<dimensions, T> StridedArrayView<dimensions, T>::transposed() const {
    static_assert(dimensionA < dimensions && dimensionB < dimensions,
        "dimensions out of range");

    Size size = _size;
    Stride stride = _stride;
    std::swap(size._data[dimensionA], size._data[dimensionB]);
    std::swap(stride._data[dimensionA], stride._data[dimensionB]);
    return StridedArrayView{size, stride, _data};
}

template<unsigned dimensions, class T> template<unsigned dimension> StridedArrayView<dimensions, T> StridedArrayView<dimensions, T>::flipped() const {
    static_assert(dimension < dimensions, "dimension out of range");

    ErasedType* data = static_cast<typename std::conditional<std::is_const<T>::value, const char, char>::type*>(_data) + _stride._data[dimension]*(_size._data[dimension] ? _size._data[dimension] - 1 : 0);
    Stride stride = _stride;
    stride._data[dimension] *= -1;
    return StridedArrayView{_size, stride, data};
}

template<unsigned dimensions, class T> template<unsigned dimension> StridedArrayView<dimensions, T> StridedArrayView<dimensions, T>::broadcasted(std::size_t size) const {
    static_assert(dimension < dimensions, "dimension out of range");
    CORRADE_ASSERT(_size._data[dimension] == 1,
        "Containers::StridedArrayView::broadcasted(): can't broadcast dimension" << dimension << "with" << _size._data[dimension] << "elements", {});

    Size size_ = _size;
    size_._data[dimension] = size;
    Stride stride = _stride;
    stride._data[dimension] = 0;
    return StridedArrayView{size_, stride, _data};
}

}}

#endif
