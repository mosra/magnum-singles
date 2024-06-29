/*
    Corrade::Containers::Array
    Corrade::Containers::StaticArray
        — lightweight alternatives to std::vector / std::array

    https://doc.magnum.graphics/corrade/classCorrade_1_1Containers_1_1Array.html
    https://doc.magnum.graphics/corrade/classCorrade_1_1Containers_1_1StaticArray.html

    Depends on CorradeArrayView.h.

    This is a single-header library generated from the Corrade project. With
    the goal being easy integration, it's deliberately free of all comments
    to keep the file size small. More info, detailed changelogs and docs here:

    -   Project homepage — https://magnum.graphics/corrade/
    -   Documentation — https://doc.magnum.graphics/corrade/
    -   GitHub project page — https://github.com/mosra/corrade
    -   GitHub Singles repository — https://github.com/mosra/magnum-singles

    Structured bindings for StaticArray on C++17 are opt-in due to reliance on
    a potentially heavy STL header --- `#define CORRADE_STRUCTURED_BINDINGS`
    before including the file. Including it multiple times with different
    macros defined works too.

    v2020.06-1687-g6b5f (2024-06-29)
    -   Ability to InPlaceInit-construct an Array from an ArrayView, in
        addition to  std::initializer_list
    -   StaticArray is now trivially copyable and constexpr if the underlying
        type is
    -   Structured bindings for StaticArray on C++17
    v2020.06-1506-g43e1c (2023-09-13)
    -   Preventing a conflict with the Array declaration in Corrade's
        Containers.h due to default template arguments being used in both
    v2020.06-1454-gfc3b7 (2023-08-27)
    -   New exceptPrefix() API, the except() API is renamed to exceptSuffix().
        The suffix() API, which took an offset, is removed and will be
        eventually reintroduced again but taking suffix size, consistently with
        prefix() that takes prefix size.
    -   New sliceSize() API, taking a begin + size instead of begin + end
    -   Element access with operator[](), front() and back() is now
        bounds-checked with assertions
    -   Convenience Array2, Array3, Array4 aliases for StaticArray
    -   The DefaultInit, ValueInit, NoInit, DirectInit and InPlaceInit tags
        were moved from Containers to the root namespace
    -   Renamed empty() to isEmpty() for consistency with other bool-returning
        APIs
    -   MSVC 2022 compatibility
    -   Further workarounds for various compiler-specific issues and standard
        defects when using {}-initialization for aggregate types
    -   Removed dependency on <utility>, resulting in about ~450 preprocessed
        lines less
    v2020.06-0-g61d1b58c (2020-06-27)
    -   Default initialization got changed to ValueInit, which means builtin
        types are zero-initialized instead of kept uninitialized
    -   Working around various compiler-specific issues and standard defects
        when using {}-initialization for aggregate types
    v2019.10-0-g162d6a7d (2019-10-24)
    -   StaticArray is now copy/movable if the underlying type is
    v2019.01-301-gefe8d740 (2019-08-05)
    -   MSVC 2019 compatibility
    -   Added except() for taking everything except last N elements
    -   Added StaticArray::slice() and suffix() with compile-time begin / end
    v2019.01-173-ge663b49c (2019-04-30)
    -   Different implementation for Array-to-view conversion
    v2019.01-107-g80d9f347 (2019-03-23)
    -   Added missing <initializer_list> include
    v2019.01-47-g524c127e (2019-02-18)
    -   Initial release

    Generated from Corrade v2020.06-1687-g6b5f (2024-06-29), 1060 / 2727 LoC
*/

/*
    This file is part of Corrade.

    Copyright © 2007, 2008, 2009, 2010, 2011, 2012, 2013, 2014, 2015, 2016,
                2017, 2018, 2019, 2020, 2021, 2022, 2023
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

#include <initializer_list>
#include <new>
#include <type_traits>

#include "CorradeArrayView.h"

#ifdef __GNUC__
#define CORRADE_TARGET_GCC
#endif
#ifdef __clang__
#define CORRADE_TARGET_CLANG
#endif
#ifdef _MSC_VER
#define CORRADE_TARGET_MSVC
#endif
#if defined(_MSC_VER) && _MSC_VER < 1920
#define CORRADE_MSVC2017_COMPATIBILITY
#endif
#if defined(CORRADE_TARGET_LIBSTDCXX) && __GNUC__ < 5 && _GLIBCXX_RELEASE < 7
#define CORRADE_NO_STD_IS_TRIVIALLY_TRAITS
#endif

#ifndef CORRADE_DEBUG_ASSERT
#define CORRADE_DEBUG_ASSERT(condition, message, returnValue)               \
    CORRADE_ASSERT(condition, message, returnValue)
#endif
#ifndef CorradeArray_h
#define CorradeArray_h
namespace Corrade { namespace Containers {

#ifndef Corrade_Containers_Containers_h
template<class T, class = void(*)(T*, std::size_t)> class Array;
#endif
template<std::size_t, class> class StaticArray;
template<class T> using Array2 = StaticArray<2, T>;
template<class T> using Array3 = StaticArray<3, T>;
template<class T> using Array4 = StaticArray<4, T>;

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
#ifndef Corrade_Containers_Array_h
#define Corrade_Containers_Array_h

namespace Corrade { namespace Containers {

namespace Implementation {
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

    template<class T> T* noInitAllocate(std::size_t size, typename std::enable_if<std::is_trivial<T>::value>::type* = nullptr) {
        return new T[size];
    }
    template<class T> T* noInitAllocate(std::size_t size, typename std::enable_if<!std::is_trivial<T>::value>::type* = nullptr) {
        return reinterpret_cast<T*>(new char[size*sizeof(T)]);
    }

    template<class T> auto noInitDeleter(typename std::enable_if<std::is_trivial<T>::value>::type* = nullptr) -> void(*)(T*, std::size_t) {
        return nullptr;
    }
    template<class T> auto noInitDeleter(typename std::enable_if<!std::is_trivial<T>::value>::type* = nullptr) -> void(*)(T*, std::size_t) {
        return [](T* data, std::size_t size) {
            if(data) for(T *it = data, *end = data + size; it != end; ++it)
                it->~T();
            delete[] reinterpret_cast<char*>(data);
        };
    }
}

template<class T, class D>
class Array {

    public:
        typedef T Type;

        typedef D Deleter;

        template<class U, class = typename std::enable_if<std::is_same<std::nullptr_t, U>::value>::type> /*implicit*/ Array(U) noexcept: _data{nullptr}, _size{0}, _deleter{} {}

        /*implicit*/ Array() noexcept: _data(nullptr), _size(0), _deleter{} {}

        explicit Array(Corrade::DefaultInitT, std::size_t size): _data{size ? new T[size] : nullptr}, _size{size}, _deleter{nullptr} {}

        explicit Array(Corrade::ValueInitT, std::size_t size): _data{size ? new T[size]() : nullptr}, _size{size}, _deleter{nullptr} {}

        explicit Array(Corrade::NoInitT, std::size_t size): _data{size ? Implementation::noInitAllocate<T>(size) : nullptr}, _size{size}, _deleter{Implementation::noInitDeleter<T>()} {}

        template<class ...Args> explicit Array(Corrade::DirectInitT, std::size_t size, Args&&... args);

        /*implicit*/ Array(Corrade::InPlaceInitT, ArrayView<const T> list);

        /*implicit*/ Array(Corrade::InPlaceInitT, std::initializer_list<T> list);

        explicit Array(std::size_t size): Array{Corrade::ValueInit, size} {}

        explicit Array(T* data, std::size_t size, D deleter = {}) noexcept: _data{data}, _size{size}, _deleter(deleter) {}

        explicit Array(ArrayView<T> view, D deleter) noexcept: Array{view.data(), view.size(), deleter} {}

        Array(const Array<T, D>&) = delete;

        Array(Array<T, D>&& other) noexcept;

        ~Array() { Implementation::CallDeleter<T, D>{}(_deleter, _data, _size); }

        Array<T, D>& operator=(const Array<T, D>&) = delete;

        Array<T, D>& operator=(Array<T, D>&& other) noexcept;

        template<class U, class = decltype(Implementation::ArrayViewConverter<T, U>::to(std::declval<ArrayView<T>>()))> /*implicit*/ operator U() {
            return Implementation::ArrayViewConverter<T, U>::to(*this);
        }

        template<class U, class = decltype(Implementation::ArrayViewConverter<const T, U>::to(std::declval<ArrayView<const T>>()))> constexpr /*implicit*/ operator U() const {
            return Implementation::ArrayViewConverter<const T, U>::to(*this);
        }

        #ifndef CORRADE_MSVC_COMPATIBILITY
        explicit operator bool() const { return _data; }
        #endif

        /*implicit*/ operator T*() & { return _data; }

        /*implicit*/ operator const T*() const & { return _data; }

        T* data() { return _data; }
        const T* data() const { return _data; }

        D deleter() const { return _deleter; }

        std::size_t size() const { return _size; }

        bool isEmpty() const { return !_size; }

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

        template<class U, class = typename std::enable_if<std::is_convertible<U, std::size_t>::value>::type> T& operator[](U i);
        template<class U, class = typename std::enable_if<std::is_convertible<U, std::size_t>::value>::type> const T& operator[](U i) const;

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

        template<class U, class = typename std::enable_if<std::is_convertible<U, T*>::value && !std::is_convertible<U, std::size_t>::value>::type> ArrayView<T> sliceSize(U begin, std::size_t size) {
            return ArrayView<T>{*this}.sliceSize(begin, size);
        }
        template<class U, class = typename std::enable_if<std::is_convertible<U, const T*>::value && !std::is_convertible<U, std::size_t>::value>::type> ArrayView<const T> sliceSize(const U begin, std::size_t size) const {
            return ArrayView<const T>{*this}.sliceSize(begin, size);
        }
        ArrayView<T> sliceSize(std::size_t begin, std::size_t size) {
            return ArrayView<T>{*this}.sliceSize(begin, size);
        }
        ArrayView<const T> sliceSize(std::size_t begin, std::size_t size) const {
            return ArrayView<const T>{*this}.sliceSize(begin, size);
        }

        template<std::size_t size_, class U, class = typename std::enable_if<std::is_convertible<U, T*>::value && !std::is_convertible<U, std::size_t>::value>::type> StaticArrayView<size_, T> slice(U begin) {
            return ArrayView<T>(*this).template slice<size_>(begin);
        }
        template<std::size_t size_, class U, class = typename std::enable_if<std::is_convertible<U, const T*>::value && !std::is_convertible<U, std::size_t>::value>::type> StaticArrayView<size_, const T> slice(U begin) const {
            return ArrayView<const T>(*this).template slice<size_>(begin);
        }
        template<std::size_t size_> StaticArrayView<size_, T> slice(std::size_t begin) {
            return ArrayView<T>(*this).template slice<size_>(begin);
        }
        template<std::size_t size_> StaticArrayView<size_, const T> slice(std::size_t begin) const {
            return ArrayView<const T>(*this).template slice<size_>(begin);
        }

        template<std::size_t begin_, std::size_t end_> StaticArrayView<end_ - begin_, T> slice() {
            return ArrayView<T>(*this).template slice<begin_, end_>();
        }

        template<std::size_t begin_, std::size_t end_> StaticArrayView<end_ - begin_, const T> slice() const {
            return ArrayView<const T>(*this).template slice<begin_, end_>();
        }

        template<std::size_t begin_, std::size_t size_> StaticArrayView<size_, T> sliceSize() {
            return ArrayView<T>(*this).template sliceSize<begin_, size_>();
        }

        template<std::size_t begin_, std::size_t size_> StaticArrayView<size_, const T> sliceSize() const {
            return ArrayView<const T>(*this).template sliceSize<begin_, size_>();
        }

        template<class U, class = typename std::enable_if<std::is_convertible<U, T*>::value && !std::is_convertible<U, std::size_t>::value>::type>
        ArrayView<T> prefix(U end) {
            return ArrayView<T>(*this).prefix(end);
        }
        template<class U, class = typename std::enable_if<std::is_convertible<U, const T*>::value && !std::is_convertible<U, std::size_t>::value>::type>
        ArrayView<const T> prefix(U end) const {
            return ArrayView<const T>(*this).prefix(end);
        }

        ArrayView<T> suffix(T* begin) {
            return ArrayView<T>(*this).suffix(begin);
        }
        ArrayView<const T> suffix(const T* begin) const {
            return ArrayView<const T>(*this).suffix(begin);
        }

        ArrayView<T> prefix(std::size_t size) {
            return ArrayView<T>(*this).prefix(size);
        }
        ArrayView<const T> prefix(std::size_t size) const {
            return ArrayView<const T>(*this).prefix(size);
        }

        template<std::size_t size_> StaticArrayView<size_, T> prefix() {
            return ArrayView<T>(*this).template prefix<size_>();
        }
        template<std::size_t size_> StaticArrayView<size_, const T> prefix() const {
            return ArrayView<const T>(*this).template prefix<size_>();
        }

        template<std::size_t size_> StaticArrayView<size_, T> suffix() {
            return ArrayView<T>(*this).template suffix<size_>();
        }
        template<std::size_t size_> StaticArrayView<size_, const T> suffix() const {
            return ArrayView<const T>(*this).template suffix<size_>();
        }

        ArrayView<T> exceptPrefix(std::size_t size_) {
            return ArrayView<T>(*this).exceptPrefix(size_);
        }
        ArrayView<const T> exceptPrefix(std::size_t size_) const {
            return ArrayView<const T>(*this).exceptPrefix(size_);
        }

        ArrayView<T> exceptSuffix(std::size_t size) {
            return ArrayView<T>(*this).exceptSuffix(size);
        }
        ArrayView<const T> exceptSuffix(std::size_t size) const {
            return ArrayView<const T>(*this).exceptSuffix(size);
        }

        T* release();

    private:
        T* _data;
        std::size_t _size;
        D _deleter;
};

template<class T> inline Array<T> array(ArrayView<const T> list) {
    return Array<T>{Corrade::InPlaceInit, list};
}

template<class T> inline Array<T> array(std::initializer_list<T> list) {
    return Array<T>{Corrade::InPlaceInit, list};
}

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
    other._deleter = D{};
}

template<class T, class D> template<class ...Args> Array<T, D>::Array(Corrade::DirectInitT, std::size_t size, Args&&... args): Array{Corrade::NoInit, size} {
    for(std::size_t i = 0; i != size; ++i)
        Implementation::construct(_data[i], Utility::forward<Args>(args)...);
}

template<class T, class D> Array<T, D>::Array(Corrade::InPlaceInitT, const ArrayView<const T> list): Array{Corrade::NoInit, list.size()} {
    std::size_t i = 0;
    for(const T& item: list)
        #if defined(CORRADE_TARGET_GCC) && !defined(CORRADE_TARGET_CLANG) && __GNUC__ < 5
        Implementation::construct(_data[i++], item);
        #else
        new(_data + i++) T{item};
        #endif
}

template<class T, class D> Array<T, D>::Array(Corrade::InPlaceInitT, std::initializer_list<T> list): Array{Corrade::InPlaceInit, arrayView(list)} {}

template<class T, class D> inline Array<T, D>& Array<T, D>::operator=(Array<T, D>&& other) noexcept {
    using Utility::swap;
    swap(_data, other._data);
    swap(_size, other._size);
    swap(_deleter, other._deleter);
    return *this;
}

template<class T, class D> template<class U, class> const T& Array<T, D>::operator[](const U i) const {
    CORRADE_DEBUG_ASSERT(std::size_t(i) < _size,
        "Containers::Array::operator[](): index" << i << "out of range for" << _size << "elements", _data[0]);
    return _data[i];
}

template<class T, class D> const T& Array<T, D>::front() const {
    CORRADE_DEBUG_ASSERT(_size, "Containers::Array::front(): array is empty", _data[0]);
    return _data[0];
}

template<class T, class D> const T& Array<T, D>::back() const {
    CORRADE_DEBUG_ASSERT(_size, "Containers::Array::back(): array is empty", _data[_size - 1]);
    return _data[_size - 1];
}

template<class T, class D> template<class U, class> T& Array<T, D>::operator[](const U i) {
    return const_cast<T&>(static_cast<const Array<T, D>&>(*this)[i]);
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
    _deleter = D{};
    return data;
}

namespace Implementation {

template<class U, class T, class D> struct ArrayViewConverter<U, Array<T, D>> {
    template<class V = U> constexpr static typename std::enable_if<std::is_convertible<T*, V*>::value, ArrayView<U>>::type from(Array<T, D>& other) {
        static_assert(sizeof(T) == sizeof(U), "types are not compatible");
        return {other.data(), other.size()};
    }
    template<class V = U> constexpr static typename std::enable_if<std::is_convertible<T*, V*>::value, ArrayView<U>>::type from(Array<T, D>&& other) {
        static_assert(sizeof(T) == sizeof(U), "types are not compatible");
        return {other.data(), other.size()};
    }
};
template<class U, class T, class D> struct ArrayViewConverter<const U, Array<T, D>> {
    template<class V = U> constexpr static typename std::enable_if<std::is_convertible<T*, V*>::value, ArrayView<const U>>::type from(const Array<T, D>& other) {
        static_assert(sizeof(T) == sizeof(U), "types are not compatible");
        return {other.data(), other.size()};
    }
};
template<class U, class T, class D> struct ArrayViewConverter<const U, Array<const T, D>> {
    template<class V = U> constexpr static typename std::enable_if<std::is_convertible<T*, V*>::value, ArrayView<const U>>::type from(const Array<const T, D>& other) {
        static_assert(sizeof(T) == sizeof(U), "types are not compatible");
        return {other.data(), other.size()};
    }
};
template<class T, class D> struct ErasedArrayViewConverter<Array<T, D>>: ArrayViewConverter<T, Array<T, D>> {};
template<class T, class D> struct ErasedArrayViewConverter<const Array<T, D>>: ArrayViewConverter<const T, Array<T, D>> {};

}

}}

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
#if CORRADE_CXX_STANDARD >= 201402 && !defined(CORRADE_MSVC2015_COMPATIBILITY)
#define CORRADE_CONSTEXPR14 constexpr
#else
#define CORRADE_CONSTEXPR14
#endif
#ifndef Corrade_Containers_StaticArray_h
#define Corrade_Containers_StaticArray_h

namespace Corrade { namespace Containers {

namespace Implementation {

template<std::size_t size_, class T, bool trivial> struct StaticArrayData;
template<std::size_t size_, class T> struct StaticArrayData<size_, T, true> {
    template<class U = T, typename std::enable_if<!std::is_constructible<U, Corrade::NoInitT>::value>::type* = nullptr> explicit StaticArrayData(Corrade::NoInitT) {}
    template<class U = T, typename std::enable_if<std::is_constructible<U, Corrade::NoInitT>::value>::type* = nullptr> explicit StaticArrayData(Corrade::NoInitT): StaticArrayData{Corrade::NoInit, typename GenerateSequence<size_>::Type{}} {}
    template<std::size_t... sequence, class U = T, typename std::enable_if<std::is_constructible<U, Corrade::NoInitT>::value>::type* = nullptr> explicit StaticArrayData(Corrade::NoInitT noInit, Sequence<sequence...>): _data{T{(&noInit)[0*sequence]}...} {}

    #if !defined(CORRADE_TARGET_MSVC) || defined(CORRADE_TARGET_CLANG) || (_MSC_VER >= 1910 && _MSC_VER < 1920)
    constexpr explicit StaticArrayData(Corrade::DefaultInitT) {}
    #else
    template<class U = T, typename std::enable_if<std::is_trivially_constructible<U>::value>::type* = nullptr> explicit StaticArrayData(Corrade::DefaultInitT) {}
    template<class U = T, typename std::enable_if<!std::is_trivially_constructible<U>::value>::type* = nullptr> constexpr explicit StaticArrayData(Corrade::DefaultInitT): _data{} {}
    #endif

    constexpr explicit StaticArrayData(Corrade::ValueInitT): _data{} {}

    template<class ...Args> constexpr explicit StaticArrayData(Corrade::InPlaceInitT, Args&&... args): _data{Utility::forward<Args>(args)...} {}
    template<std::size_t ...sequence> constexpr explicit StaticArrayData(Corrade::InPlaceInitT, Sequence<sequence...>, const T(&data)[sizeof...(sequence)]): _data{data[sequence]...} {}
    #ifndef CORRADE_MSVC2017_COMPATIBILITY
    template<std::size_t ...sequence> constexpr explicit StaticArrayData(Corrade::InPlaceInitT, Sequence<sequence...>, T(&&data)[sizeof...(sequence)]): _data{Utility::move(data[sequence])...} {}
    #endif

    T _data[size_];
};

template<std::size_t size_, class T> struct StaticArrayData<size_, T, false> {
    explicit StaticArrayData(Corrade::NoInitT) {}

    explicit StaticArrayData(Corrade::DefaultInitT)
        #if !defined(CORRADE_TARGET_GCC) || defined(CORRADE_TARGET_CLANG) || __GNUC__*100 + __GNUC_MINOR__ >= 10003
        : _data() {}
        #else
        {
            for(T& i: _data) new(&i) T();
        }
        #endif

    explicit StaticArrayData(Corrade::ValueInitT): _data() {}

    template<class ...Args> explicit StaticArrayData(Corrade::InPlaceInitT, Args&&... args): _data{Utility::forward<Args>(args)...} {}
    template<std::size_t ...sequence> explicit StaticArrayData(Corrade::InPlaceInitT, Implementation::Sequence<sequence...>, const T(&data)[sizeof...(sequence)]): _data{data[sequence]...} {}
    #ifndef CORRADE_MSVC2017_COMPATIBILITY
    template<std::size_t ...sequence> explicit StaticArrayData(Corrade::InPlaceInitT, Implementation::Sequence<sequence...>, T(&&data)[sizeof...(sequence)]): _data{Utility::move(data[sequence])...} {}
    #endif

    StaticArrayData(const StaticArrayData<size_, T, false>& other) noexcept(std::is_nothrow_copy_constructible<T>::value);
    StaticArrayData(StaticArrayData<size_, T, false>&& other) noexcept(std::is_nothrow_move_constructible<T>::value);
    ~StaticArrayData();
    StaticArrayData<size_, T, false>& operator=(const StaticArrayData<size_, T, false>&) noexcept(std::is_nothrow_copy_constructible<T>::value);
    StaticArrayData<size_, T, false>& operator=(StaticArrayData<size_, T, false>&&) noexcept(std::is_nothrow_move_constructible<T>::value);

    union {
        T _data[size_];
    };
};

template<std::size_t size_, class T> using StaticArrayDataFor = StaticArrayData<size_, T,
    #ifdef CORRADE_NO_STD_IS_TRIVIALLY_TRAITS
    __has_trivial_constructor(T)
    #else
    std::is_trivially_constructible<T>::value
    #endif
    || std::is_constructible<T, Corrade::NoInitT>::value>;

}

template<std::size_t size_, class T> class StaticArray: Implementation::StaticArrayDataFor<size_, T> {

    public:
        enum: std::size_t {
            Size = size_
        };
        typedef T Type;

        constexpr explicit StaticArray(Corrade::DefaultInitT): Implementation::StaticArrayDataFor<size_, T>{Corrade::DefaultInit} {}

        constexpr explicit StaticArray(Corrade::ValueInitT): Implementation::StaticArrayDataFor<size_, T>{Corrade::ValueInit} {}

        explicit StaticArray(Corrade::NoInitT): Implementation::StaticArrayDataFor<size_, T>{Corrade::NoInit} {}

        template<class ...Args> explicit StaticArray(Corrade::DirectInitT, Args&&... args);

        template<class ...Args> constexpr explicit StaticArray(Corrade::InPlaceInitT, Args&&... args): Implementation::StaticArrayDataFor<size_, T>{Corrade::InPlaceInit, Utility::forward<Args>(args)...} {
            static_assert(sizeof...(args) == size_, "Containers::StaticArray: wrong number of initializers");
        }

        #if !defined(CORRADE_TARGET_GCC) || defined(CORRADE_TARGET_CLANG) || __GNUC__ >= 5
        template<std::size_t size> constexpr explicit StaticArray(Corrade::InPlaceInitT, const T(&data)[size]): Implementation::StaticArrayDataFor<size_, T>{Corrade::InPlaceInit, typename Implementation::GenerateSequence<size>::Type{}, data} {
            static_assert(size == size_, "Containers::StaticArray: wrong number of initializers");
        }
        #else
        constexpr explicit StaticArray(Corrade::InPlaceInitT, const T(&data)[size_]): Implementation::StaticArrayDataFor<size_, T>{Corrade::InPlaceInit, typename Implementation::GenerateSequence<size_>::Type{}, data} {}
        #endif

        #ifndef CORRADE_MSVC2017_COMPATIBILITY
        #if !defined(CORRADE_TARGET_GCC) || defined(CORRADE_TARGET_CLANG) || __GNUC__ >= 5
        template<std::size_t size> constexpr explicit StaticArray(Corrade::InPlaceInitT, T(&&data)[size]): Implementation::StaticArrayDataFor<size_, T>{Corrade::InPlaceInit, typename Implementation::GenerateSequence<size>::Type{}, Utility::move(data)} {
            static_assert(size == size_, "Containers::StaticArray: wrong number of initializers");
        }
        #else
        constexpr explicit StaticArray(Corrade::InPlaceInitT, T(&&data)[size_]): Implementation::StaticArrayDataFor<size_, T>{Corrade::InPlaceInit, typename Implementation::GenerateSequence<size_>::Type{}, Utility::move(data)} {}
        #endif
        #endif

        constexpr explicit StaticArray(): Implementation::StaticArrayDataFor<size_, T>{Corrade::ValueInit} {}

        template<class First, class ...Next, class = typename std::enable_if<std::is_convertible<First&&, T>::value>::type> constexpr /*implicit*/ StaticArray(First&& first, Next&&... next): Implementation::StaticArrayDataFor<size_, T>{Corrade::InPlaceInit, Utility::forward<First>(first), Utility::forward<Next>(next)...} {
            static_assert(sizeof...(next) + 1 == size_, "Containers::StaticArray: wrong number of initializers");
        }

        #if !defined(CORRADE_TARGET_GCC) || defined(CORRADE_TARGET_CLANG) || __GNUC__ >= 5
        template<std::size_t size> constexpr explicit StaticArray(const T(&data)[size]): Implementation::StaticArrayDataFor<size_, T>{Corrade::InPlaceInit, typename Implementation::GenerateSequence<size>::Type{}, data} {
            static_assert(size == size_, "Containers::StaticArray: wrong number of initializers");
        }
        #else
        constexpr explicit StaticArray(const T(&data)[size_]): Implementation::StaticArrayDataFor<size_, T>{Corrade::InPlaceInit, typename Implementation::GenerateSequence<size_>::Type{}, data} {}
        #endif

        #ifndef CORRADE_MSVC2017_COMPATIBILITY
        #if !defined(CORRADE_TARGET_GCC) || defined(CORRADE_TARGET_CLANG) || __GNUC__ >= 5
        template<std::size_t size> constexpr explicit StaticArray(T(&&data)[size]): Implementation::StaticArrayDataFor<size_, T>{Corrade::InPlaceInit, typename Implementation::GenerateSequence<size>::Type{}, Utility::move(data)} {
            static_assert(size == size_, "Containers::StaticArray: wrong number of initializers");
        }
        #else
        constexpr explicit StaticArray(T(&&data)[size_]): Implementation::StaticArrayDataFor<size_, T>{Corrade::InPlaceInit, typename Implementation::GenerateSequence<size_>::Type{}, Utility::move(data)} {}
        #endif
        #endif

        template<class U, class = decltype(Implementation::StaticArrayViewConverter<size_, T, U>::to(std::declval<StaticArrayView<size_, T>>()))> /*implicit*/ operator U() {
            return Implementation::StaticArrayViewConverter<size_, T, U>::to(*this);
        }

        template<class U, class = decltype(Implementation::StaticArrayViewConverter<size_, const T, U>::to(std::declval<StaticArrayView<size_, const T>>()))> constexpr /*implicit*/ operator U() const {
            return Implementation::StaticArrayViewConverter<size_, const T, U>::to(*this);
        }

        #ifndef CORRADE_MSVC_COMPATIBILITY
        constexpr explicit operator bool() const { return true; }
        #endif

        /*implicit*/ operator T*() & { return this->_data; }

        constexpr /*implicit*/ operator const T*() const & { return this->_data; }

        T* data() { return this->_data; }
        constexpr const T* data() const { return this->_data; }

        constexpr std::size_t size() const { return size_; }

        constexpr bool isEmpty() const { return !size_; }

        T* begin() { return this->_data; }
        constexpr const T* begin() const { return this->_data; }
        constexpr const T* cbegin() const { return this->_data; }

        T* end() { return this->_data + size_; }
        constexpr const T* end() const { return this->_data + size_; }
        constexpr const T* cend() const { return this->_data + size_; }

        T& front() { return this->_data[0]; }
        constexpr const T& front() const { return this->_data[0]; }

        T& back() { return this->_data[size_ - 1]; }
        constexpr const T& back() const { return this->_data[size_ - 1]; }

        template<class U, class = typename std::enable_if<std::is_convertible<U, std::size_t>::value>::type> T& operator[](U i);
        template<class U, class = typename std::enable_if<std::is_convertible<U, std::size_t>::value>::type> constexpr const T& operator[](U i) const;

        ArrayView<T> slice(T* begin, T* end) {
            return ArrayView<T>(*this).slice(begin, end);
        }
        constexpr ArrayView<const T> slice(const T* begin, const T* end) const {
            return ArrayView<const T>(*this).slice(begin, end);
        }
        ArrayView<T> slice(std::size_t begin, std::size_t end) {
            return ArrayView<T>(*this).slice(begin, end);
        }
        constexpr ArrayView<const T> slice(std::size_t begin, std::size_t end) const {
            return ArrayView<const T>(*this).slice(begin, end);
        }

        template<class U, class = typename std::enable_if<std::is_convertible<U, T*>::value && !std::is_convertible<U, std::size_t>::value>::type> ArrayView<T> sliceSize(U begin, std::size_t size) {
            return ArrayView<T>{*this}.sliceSize(begin, size);
        }
        template<class U, class = typename std::enable_if<std::is_convertible<U, const T*>::value && !std::is_convertible<U, std::size_t>::value>::type> constexpr ArrayView<const T> sliceSize(const U begin, std::size_t size) const {
            return ArrayView<const T>{*this}.sliceSize(begin, size);
        }
        ArrayView<T> sliceSize(std::size_t begin, std::size_t size) {
            return ArrayView<T>{*this}.sliceSize(begin, size);
        }
        constexpr ArrayView<const T> sliceSize(std::size_t begin, std::size_t size) const {
            return ArrayView<const T>{*this}.sliceSize(begin, size);
        }

        template<std::size_t size__, class U, class = typename std::enable_if<std::is_convertible<U, T*>::value && !std::is_convertible<U, std::size_t>::value>::type> StaticArrayView<size__, T> slice(U begin) {
            return ArrayView<T>(*this).template slice<size__>(begin);
        }
        template<std::size_t size__, class U, class = typename std::enable_if<std::is_convertible<U, const T*>::value && !std::is_convertible<U, std::size_t>::value>::type> constexpr StaticArrayView<size__, const T> slice(U begin) const {
            return ArrayView<const T>(*this).template slice<size__>(begin);
        }
        template<std::size_t size__> StaticArrayView<size__, T> slice(std::size_t begin) {
            return ArrayView<T>(*this).template slice<size__>(begin);
        }
        template<std::size_t size__> constexpr StaticArrayView<size__, const T> slice(std::size_t begin) const {
            return ArrayView<const T>(*this).template slice<size__>(begin);
        }

        template<std::size_t begin_, std::size_t end_> StaticArrayView<end_ - begin_, T> slice() {
            return StaticArrayView<size_, T>(*this).template slice<begin_, end_>();
        }

        template<std::size_t begin_, std::size_t end_> constexpr StaticArrayView<end_ - begin_, const T> slice() const {
            return StaticArrayView<size_, const T>(*this).template slice<begin_, end_>();
        }

        template<std::size_t begin_, std::size_t size__> StaticArrayView<size__, T> sliceSize() {
            return StaticArrayView<size_, T>(*this).template sliceSize<begin_, size__>();
        }

        template<std::size_t begin_, std::size_t size__> constexpr StaticArrayView<size__, const T> sliceSize() const {
            return StaticArrayView<size_, const T>(*this).template sliceSize<begin_, size__>();
        }

        template<class U, class = typename std::enable_if<std::is_convertible<U, T*>::value && !std::is_convertible<U, std::size_t>::value>::type> ArrayView<T> prefix(U end) {
            return ArrayView<T>(*this).prefix(end);
        }
        template<class U, class = typename std::enable_if<std::is_convertible<U, const T*>::value && !std::is_convertible<U, std::size_t>::value>::type> constexpr ArrayView<const T> prefix(U end) const {
            return ArrayView<const T>(*this).prefix(end);
        }

        ArrayView<T> suffix(T* begin) {
            return ArrayView<T>(*this).suffix(begin);
        }
        constexpr ArrayView<const T> suffix(const T* begin) const {
            return ArrayView<const T>(*this).suffix(begin);
        }

        ArrayView<T> prefix(std::size_t size) {
            return ArrayView<T>(*this).prefix(size);
        }
        constexpr ArrayView<const T> prefix(std::size_t size) const {
            return ArrayView<const T>(*this).prefix(size);
        }

        template<std::size_t size__> StaticArrayView<size__, T> prefix();
        template<std::size_t size__> constexpr StaticArrayView<size__, const T> prefix() const;

        ArrayView<T> exceptPrefix(std::size_t size) {
            return ArrayView<T>(*this).exceptPrefix(size);
        }
        constexpr ArrayView<const T> exceptPrefix(std::size_t size) const {
            return ArrayView<const T>(*this).exceptPrefix(size);
        }

        template<std::size_t size__> StaticArrayView<size_ - size__, T> exceptPrefix() {
            return StaticArrayView<size_, T>(*this).template exceptPrefix<size__>();
        }
        template<std::size_t size__> constexpr StaticArrayView<size_ - size__, const T> exceptPrefix() const {
            return StaticArrayView<size_, const T>(*this).template exceptPrefix<size__>();
        }

        ArrayView<T> exceptSuffix(std::size_t size) {
            return ArrayView<T>(*this).exceptSuffix(size);
        }
        constexpr ArrayView<const T> exceptSuffix(std::size_t size) const {
            return ArrayView<const T>(*this).exceptSuffix(size);
        }

        template<std::size_t size__> StaticArrayView<size_ - size__, T> exceptSuffix() {
            return StaticArrayView<size_, T>(*this).template exceptSuffix<size__>();
        }
        template<std::size_t size__> constexpr StaticArrayView<size_ - size__, const T> exceptSuffix() const {
            return StaticArrayView<size_, const T>(*this).template exceptSuffix<size__>();
        }

    private:
        #if CORRADE_CXX_STANDARD > 201402
        template<std::size_t index> constexpr friend const T& get(const StaticArray<size_, T>& value) {
            return value._data[index];
        }
        template<std::size_t index> CORRADE_CONSTEXPR14 friend T& get(StaticArray<size_, T>& value) {
            return value._data[index];
        }
        template<std::size_t index> CORRADE_CONSTEXPR14 friend T&& get(StaticArray<size_, T>&& value) {
            return Utility::move(value._data[index]);
        }
        #endif
};

#ifndef CORRADE_MSVC2015_COMPATIBILITY /* Multiple definitions still broken */
template<class T> using Array1 = StaticArray<1, T>;

template<class T> using Array2 = StaticArray<2, T>;

template<class T> using Array3 = StaticArray<3, T>;

template<class T> using Array4 = StaticArray<4, T>;
#endif

template<std::size_t size, class T> constexpr ArrayView<T> arrayView(StaticArray<size, T>& array) {
    return ArrayView<T>{array};
}

template<std::size_t size, class T> constexpr ArrayView<const T> arrayView(const StaticArray<size, T>& array) {
    return ArrayView<const T>{array};
}

template<std::size_t size, class T> constexpr StaticArrayView<size, T> staticArrayView(StaticArray<size, T>& array) {
    return StaticArrayView<size, T>{array};
}

template<std::size_t size, class T> constexpr StaticArrayView<size, const T> staticArrayView(const StaticArray<size, T>& array) {
    return StaticArrayView<size, const T>{array};
}

template<class U, std::size_t size, class T> StaticArrayView<size*sizeof(T)/sizeof(U), U> arrayCast(StaticArray<size, T>& array) {
    return arrayCast<U>(staticArrayView(array));
}

template<class U, std::size_t size, class T> StaticArrayView<size*sizeof(T)/sizeof(U), const U> arrayCast(const StaticArray<size, T>& array) {
    return arrayCast<const U>(staticArrayView(array));
}

template<std::size_t size_, class T> constexpr std::size_t arraySize(const StaticArray<size_, T>&) {
    return size_;
}

template<std::size_t size_, class T> template<class ...Args> StaticArray<size_, T>::StaticArray(Corrade::DirectInitT, Args&&... args): StaticArray{Corrade::NoInit} {
    for(T& i: this->_data)
        Implementation::construct(i, Utility::forward<Args>(args)...);
}

namespace Implementation {

template<std::size_t size_, class T> StaticArrayData<size_, T, false>::StaticArrayData(const StaticArrayData<size_, T, false>& other) noexcept(std::is_nothrow_copy_constructible<T>::value): StaticArrayData{Corrade::NoInit} {
    for(std::size_t i = 0; i != size_; ++i)
        #if defined(CORRADE_TARGET_GCC) && !defined(CORRADE_TARGET_CLANG) && __GNUC__ < 5
        Implementation::construct(_data[i], other._data[i]);
        #else
        new(_data + i) T{other._data[i]};
        #endif
}

template<std::size_t size_, class T> StaticArrayData<size_, T, false>::StaticArrayData(StaticArrayData<size_, T, false>&& other) noexcept(std::is_nothrow_move_constructible<T>::value): StaticArrayData{Corrade::NoInit} {
    for(std::size_t i = 0; i != size_; ++i)
        #if defined(CORRADE_TARGET_GCC) && !defined(CORRADE_TARGET_CLANG) && __GNUC__ < 5
        Implementation::construct(_data[i], Utility::move(other._data[i]));
        #else
        new(&_data[i]) T{Utility::move(other._data[i])};
        #endif
}

template<std::size_t size_, class T> StaticArrayData<size_, T, false>::~StaticArrayData() {
    for(T& i: _data) {
        i.~T();
        #ifdef CORRADE_MSVC2015_COMPATIBILITY
        static_cast<void>(i);
        #endif
    }
}

template<std::size_t size_, class T> StaticArrayData<size_, T, false>& StaticArrayData<size_, T, false>::operator=(const StaticArrayData<size_, T, false>& other) noexcept(std::is_nothrow_copy_constructible<T>::value) {
    for(std::size_t i = 0; i != size_; ++i)
        _data[i] = other._data[i];
    return *this;
}

template<std::size_t size_, class T> StaticArrayData<size_, T, false>& StaticArrayData<size_, T, false>::operator=(StaticArrayData<size_, T, false>&& other) noexcept(std::is_nothrow_move_constructible<T>::value) {
    using Utility::swap;
    for(std::size_t i = 0; i != size_; ++i)
        swap(_data[i], other._data[i]);
    return *this;
}

}

template<std::size_t size_, class T> template<class U, class> constexpr const T& StaticArray<size_, T>::operator[](const U i) const {
    return CORRADE_CONSTEXPR_DEBUG_ASSERT(std::size_t(i) < size_,
        "Containers::StaticArray::operator[](): index" << i << "out of range for" << size_ << "elements"), this->_data[i];
}

template<std::size_t size_, class T> template<class U, class> T& StaticArray<size_, T>::operator[](const U i) {
    return const_cast<T&>(static_cast<const StaticArray<size_, T>&>(*this)[i]);
}

template<std::size_t size_, class T> template<std::size_t size__> StaticArrayView<size__, T> StaticArray<size_, T>::prefix() {
    static_assert(size__ <= size_, "prefix size too large");
    return StaticArrayView<size__, T>{this->_data};
}

template<std::size_t size_, class T> template<std::size_t size__> constexpr StaticArrayView<size__, const T> StaticArray<size_, T>::prefix() const {
    static_assert(size__ <= size_, "prefix size too large");
    return StaticArrayView<size__, const T>{this->_data};
}

namespace Implementation {

template<class U, std::size_t size, class T> struct ArrayViewConverter<U, StaticArray<size, T>> {
    template<class V = U> constexpr static typename std::enable_if<std::is_convertible<T*, V*>::value, ArrayView<U>>::type from(StaticArray<size, T>& other) {
        static_assert(sizeof(T) == sizeof(U), "types are not compatible");
        return {other.data(), other.size()};
    }
};
template<class U, std::size_t size, class T> struct ArrayViewConverter<const U, StaticArray<size, T>> {
    template<class V = U> constexpr static typename std::enable_if<std::is_convertible<T*, V*>::value, ArrayView<const U>>::type from(const StaticArray<size, T>& other) {
        static_assert(sizeof(T) == sizeof(U), "types are not compatible");
        return {other.data(), other.size()};
    }
};
template<class U, std::size_t size, class T> struct ArrayViewConverter<const U, StaticArray<size, const T>> {
    template<class V = U> constexpr static typename std::enable_if<std::is_convertible<T*, V*>::value, ArrayView<const U>>::type from(const StaticArray<size, const T>& other) {
        static_assert(sizeof(T) == sizeof(U), "types are not compatible");
        return {other.data(), other.size()};
    }
};
template<std::size_t size, class T> struct ErasedArrayViewConverter<StaticArray<size, T>>: ArrayViewConverter<T, StaticArray<size, T>> {};
template<std::size_t size, class T> struct ErasedArrayViewConverter<const StaticArray<size, T>>: ArrayViewConverter<const T, StaticArray<size, T>> {};

template<class U, std::size_t size, class T> struct StaticArrayViewConverter<size, U, StaticArray<size, T>> {
    template<class V = U> constexpr static typename std::enable_if<std::is_convertible<T*, V*>::value, StaticArrayView<size, U>>::type from(StaticArray<size, T>& other) {
        static_assert(sizeof(T) == sizeof(U), "types are not compatible");
        return StaticArrayView<size, T>{other.data()};
    }
};
template<class U, std::size_t size, class T> struct StaticArrayViewConverter<size, const U, StaticArray<size, T>> {
    template<class V = U> constexpr static typename std::enable_if<std::is_convertible<T*, V*>::value, StaticArrayView<size, const U>>::type from(const StaticArray<size, T>& other) {
        static_assert(sizeof(T) == sizeof(U), "types are not compatible");
        return StaticArrayView<size, const T>(other.data());
    }
};
template<class U, std::size_t size, class T> struct StaticArrayViewConverter<size, const U, StaticArray<size, const T>> {
    template<class V = U> constexpr static typename std::enable_if<std::is_convertible<T*, V*>::value, StaticArrayView<size, const U>>::type from(const StaticArray<size, const T>& other) {
        static_assert(sizeof(T) == sizeof(U), "types are not compatible");
        return StaticArrayView<size, const T>(other.data());
    }
};
template<std::size_t size, class T> struct ErasedStaticArrayViewConverter<StaticArray<size, T>>: StaticArrayViewConverter<size, T, StaticArray<size, T>> {};
template<std::size_t size, class T> struct ErasedStaticArrayViewConverter<const StaticArray<size, T>>: StaticArrayViewConverter<size, const T, StaticArray<size, T>> {};

}

}}

#endif
#ifdef CORRADE_STRUCTURED_BINDINGS
namespace std {

#ifndef Corrade_Containers_StructuredBindings_StaticArray_h
#define Corrade_Containers_StructuredBindings_StaticArray_h
template<size_t size_, class T> struct tuple_size<Corrade::Containers::StaticArray<size_, T>>: integral_constant<size_t, size_> {};
template<size_t index, size_t size_, class T> struct tuple_element<index, Corrade::Containers::StaticArray<size_, T>> { typedef T type; };
#endif

}
#endif
