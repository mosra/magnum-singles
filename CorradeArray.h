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

    v2019.01-107-g80d9f347 (2019-03-23)
    -   Added missing <initializer_list> include
    v2019.01-47-g524c127e (2019-02-18)
    -   Initial release

    Generated from Corrade v2019.01-107-g80d9f347 (2019-03-23), 598 / 3225 LoC
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

#include <initializer_list>
#include <new>
#include <type_traits>
#include <utility>

#include "CorradeArrayView.h"

#ifndef CorradeArray_h
#define CorradeArray_h
namespace Corrade { namespace Containers {

template<class T, class = void(*)(T*, std::size_t)> class Array;
template<std::size_t, class> class StaticArray;

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
#ifndef Corrade_Containers_StaticArray_h
#define Corrade_Containers_StaticArray_h

namespace Corrade { namespace Containers {

template<std::size_t size_, class T> class StaticArray {
    public:
        enum: std::size_t {
            Size = size_
        };
        typedef T Type;

        explicit StaticArray(DefaultInitT): StaticArray{DefaultInit, std::integral_constant<bool, std::is_pod<T>::value>{}} {}

        explicit StaticArray(ValueInitT): _data{} {}

        explicit StaticArray(NoInitT) {}

        template<class ...Args> explicit StaticArray(DirectInitT, Args&&... args);

        template<class ...Args> explicit StaticArray(InPlaceInitT, Args&&... args): _data{std::forward<Args>(args)...} {
            static_assert(sizeof...(args) == size_, "Containers::StaticArray: wrong number of initializers");
        }

        explicit StaticArray(): StaticArray{DefaultInit} {}

        template<class First, class ...Next> /*implicit*/ StaticArray(First&& first, Next&&... next): StaticArray{InPlaceInit, std::forward<First>(first), std::forward<Next>(next)...} {}

        StaticArray(const StaticArray<size_, T>&) = delete;

        StaticArray(StaticArray<size_, T>&&) = delete;

        ~StaticArray();

        StaticArray<size_, T>& operator=(const StaticArray<size_, T>&) = delete;

        StaticArray<size_, T>& operator=(StaticArray<size_, T>&&) = delete;

        template<class U, class V = typename std::enable_if<!std::is_void<U>::value && std::is_convertible<T*, U*>::value>::type>
        /*implicit*/ operator ArrayView<U>() noexcept {
            static_assert(sizeof(T) == sizeof(U), "type sizes are not compatible");
            return {_data, size_};
        }

        template<class U, class V = typename std::enable_if<std::is_convertible<T*, U*>::value || std::is_convertible<T*, const U*>::value>::type>
        /*implicit*/ operator ArrayView<const U>() const noexcept {
            static_assert(sizeof(T) == sizeof(U), "type sizes are not compatible");
            return {_data, size_};
        }

        /*implicit*/ operator ArrayView<const void>() const noexcept {
            return {_data, size_};
        }

        template<class U, class V = typename std::enable_if<std::is_convertible<T*, U*>::value>::type>
        /*implicit*/ operator StaticArrayView<size_, U>() noexcept {
            static_assert(sizeof(T) == sizeof(U), "type sizes are not compatible");
            return StaticArrayView<size_, U>{_data};
        }

        template<class U, class V = typename std::enable_if<std::is_convertible<T*, U*>::value || std::is_convertible<T*, const U*>::value>::type>
        /*implicit*/ operator StaticArrayView<size_, const U>() const noexcept {
            static_assert(sizeof(T) == sizeof(U), "type sizes are not compatible");
            return StaticArrayView<size_, const U>{_data};
        }

        template<class U, class = decltype(Implementation::StaticArrayViewConverter<size_, T, U>::to(std::declval<StaticArrayView<size_, T>>()))> /*implicit*/ operator U() {
            return Implementation::StaticArrayViewConverter<size_, T, U>::to(*this);
        }

        template<class U, class = decltype(Implementation::StaticArrayViewConverter<size_, const T, U>::to(std::declval<StaticArrayView<size_, const T>>()))> constexpr /*implicit*/ operator U() const {
            return Implementation::StaticArrayViewConverter<size_, const T, U>::to(*this);
        }

        /*implicit*/ operator T*() & { return _data; }

        /*implicit*/ operator const T*() const & { return _data; }

        T* data() { return _data; }
        const T* data() const { return _data; }

        constexpr std::size_t size() const { return size_; }

        constexpr bool empty() const { return !size_; }

        T* begin() { return _data; }
        const T* begin() const { return _data; }
        const T* cbegin() const { return _data; }

        T* end() { return _data + size_; }
        const T* end() const { return _data + size_; }
        const T* cend() const { return _data + size_; }

        T& front() { return _data[0]; }
        const T& front() const { return _data[0]; }

        T& back() { return _data[size_ - 1]; }
        const T& back() const { return _data[size_ - 1]; }

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

        template<std::size_t viewSize> StaticArrayView<viewSize, T> slice(T* begin) {
            return ArrayView<T>(*this).template slice<viewSize>(begin);
        }
        template<std::size_t viewSize> StaticArrayView<viewSize, const T> slice(const T* begin) const {
            return ArrayView<const T>(*this).template slice<viewSize>(begin);
        }
        template<std::size_t viewSize> StaticArrayView<viewSize, T> slice(std::size_t begin) {
            return ArrayView<T>(*this).template slice<viewSize>(begin);
        }
        template<std::size_t viewSize> StaticArrayView<viewSize, const T> slice(std::size_t begin) const {
            return ArrayView<const T>(*this).template slice<viewSize>(begin);
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

        template<std::size_t viewSize> StaticArrayView<viewSize, T> prefix();
        template<std::size_t viewSize> StaticArrayView<viewSize, const T> prefix() const;

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

    private:
        explicit StaticArray(DefaultInitT, std::true_type) {}
        #if !defined(__GNUC__) || defined(__clang__)
        explicit StaticArray(DefaultInitT, std::false_type): _data{} {}
        #else
        explicit StaticArray(DefaultInitT, std::false_type) {
            for(T& i: _data) new(&i) T{};
        }
        #endif

        union {
            T _data[size_];
        };
};

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

template<std::size_t size_, class T> template<class ...Args> StaticArray<size_, T>::StaticArray(DirectInitT, Args&&... args): StaticArray{NoInit} {
    for(T& i: _data) {
        new(&i) T{std::forward<Args>(args)...};
    }
}

template<std::size_t size_, class T> StaticArray<size_, T>::~StaticArray() {
    for(T& i: _data) i.~T();
}

template<std::size_t size_, class T> template<std::size_t viewSize> StaticArrayView<viewSize, T> StaticArray<size_, T>::prefix() {
    static_assert(viewSize <= size_, "prefix size too large");
    return StaticArrayView<viewSize, T>{_data};
}

template<std::size_t size_, class T> template<std::size_t viewSize> StaticArrayView<viewSize, const T> StaticArray<size_, T>::prefix() const {
    static_assert(viewSize <= size_, "prefix size too large");
    return StaticArrayView<viewSize, const T>{_data};
}

}}

#endif
