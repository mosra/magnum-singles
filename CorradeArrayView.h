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

    v2019.01-41-g39c08d7c (2019-02-18)
    -   Initial release

    Generated from Corrade v2019.01-41-g39c08d7c (2019-02-18), 558 / 2453 LoC
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

#include <cassert>
#include <cstddef>
#include <type_traits>
#include <utility>

#if defined(_MSC_VER) && _MSC_VER <= 1920
#define CORRADE_MSVC2017_COMPATIBILITY
#endif

#ifndef CorradeArrayView_h
#define CorradeArrayView_h

namespace Corrade { namespace Containers {

template<class> class ArrayView;
template<std::size_t, class> class StaticArrayView;

}}
#endif
#ifndef CORRADE_ASSERT
#ifdef NDEBUG
#define CORRADE_ASSERT(condition, message, returnValue) do {} while(0)
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

        constexpr /*implicit*/ ArrayView(std::nullptr_t) noexcept: _data(nullptr), _size(0) {}

        constexpr /*implicit*/ ArrayView() noexcept: _data(nullptr), _size(0) {}

        constexpr /*implicit*/ ArrayView(T* data, std::size_t size) noexcept: _data(data), _size(size) {}

        template<class U, std::size_t size, class = typename std::enable_if<std::is_convertible<U*, T*>::value>::type>
        constexpr /*implicit*/ ArrayView(U(&data)[size]) noexcept: _data{data}, _size{size} {
            static_assert(sizeof(T) == sizeof(U), "type sizes are not compatible");
        }

        template<class U, class = typename std::enable_if<std::is_convertible<U*, T*>::value>::type>
        constexpr /*implicit*/ ArrayView(ArrayView<U> view) noexcept: _data{view}, _size{view.size()} {
            static_assert(sizeof(T) == sizeof(U), "type sizes are not compatible");
        }

        template<std::size_t size, class U, class = typename std::enable_if<std::is_convertible<U*, T*>::value>::type>
        constexpr /*implicit*/ ArrayView(StaticArrayView<size, U> view) noexcept: _data{view}, _size{size} {
            static_assert(sizeof(U) == sizeof(T), "type sizes are not compatible");
        }

        template<class U, class = decltype(Implementation::ArrayViewConverter<T, typename std::decay<U&&>::type>::from(std::declval<U&&>()))> constexpr /*implicit*/ ArrayView(U&& other) noexcept: ArrayView{Implementation::ArrayViewConverter<T, typename std::decay<U&&>::type>::from(std::forward<U>(other))} {}

        template<class U, class = decltype(Implementation::ArrayViewConverter<T, U>::to(std::declval<ArrayView<T>>()))> constexpr /*implicit*/ operator U() const {
            return Implementation::ArrayViewConverter<T, U>::to(*this);
        }

        #ifndef CORRADE_MSVC2017_COMPATIBILITY
        constexpr explicit operator bool() const { return _data; }
        #endif

        constexpr /*implicit*/ operator T*() const { return _data; }

        constexpr T* data() const { return _data; }

        constexpr std::size_t size() const { return _size; }

        constexpr bool empty() const { return !_size; }

        constexpr T* begin() const { return _data; }
        constexpr T* cbegin() const { return _data; }

        constexpr T* end() const { return _data+_size; }
        constexpr T* cend() const { return _data+_size; }

        T& front() const;

        T& back() const;

        constexpr ArrayView<T> slice(T* begin, T* end) const;

        constexpr ArrayView<T> slice(std::size_t begin, std::size_t end) const;

        template<std::size_t viewSize> constexpr StaticArrayView<viewSize, T> slice(T* begin) const;

        template<std::size_t viewSize> constexpr StaticArrayView<viewSize, T> slice(std::size_t begin) const;

        constexpr ArrayView<T> prefix(T* end) const {
            return end ? slice(_data, end) : nullptr;
        }

        constexpr ArrayView<T> prefix(std::size_t end) const {
            return slice(0, end);
        }

        template<std::size_t viewSize> constexpr StaticArrayView<viewSize, T> prefix() const {
            return slice<viewSize>(_data);
        }

        constexpr ArrayView<T> suffix(T* begin) const {
            return _data && !begin ? nullptr : slice(begin, _data + _size);
        }

        constexpr ArrayView<T> suffix(std::size_t begin) const {
            return slice(begin, _size);
        }

    private:
        T* _data;
        std::size_t _size;
};

template<> class ArrayView<const void> {
    public:
        typedef const void Type;

        constexpr /*implicit*/ ArrayView(std::nullptr_t) noexcept: _data(nullptr), _size(0) {}

        constexpr /*implicit*/ ArrayView() noexcept: _data(nullptr), _size(0) {}

        constexpr /*implicit*/ ArrayView(const void* data, std::size_t size) noexcept: _data(data), _size(size) {}

        template<class T> constexpr /*implicit*/ ArrayView(const T* data, std::size_t size) noexcept: _data(data), _size(size*sizeof(T)) {}

        template<class T, std::size_t size> constexpr /*implicit*/ ArrayView(T(&data)[size]) noexcept: _data(data), _size(size*sizeof(T)) {}

        template<class T> constexpr /*implicit*/ ArrayView(const ArrayView<T>& array) noexcept: _data(array), _size(array.size()*sizeof(T)) {}

        template<std::size_t size, class T> constexpr /*implicit*/ ArrayView(const StaticArrayView<size, T>& array) noexcept: _data{array}, _size{size*sizeof(T)} {}

        template<class T, class = decltype(Implementation::ErasedArrayViewConverter<const T>::from(std::declval<const T&>()))> constexpr /*implicit*/ ArrayView(const T& other) noexcept: ArrayView{Implementation::ErasedArrayViewConverter<const T>::from(other)} {}

        #ifndef CORRADE_MSVC2017_COMPATIBILITY
        constexpr explicit operator bool() const { return _data; }
        #endif

        constexpr /*implicit*/ operator const void*() const { return _data; }

        constexpr const void* data() const { return _data; }

        constexpr std::size_t size() const { return _size; }

        constexpr bool empty() const { return !_size; }

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

template<std::size_t size, class T> constexpr ArrayView<T> arrayView(StaticArrayView<size, T> view) {
    return ArrayView<T>{view};
}

template<class T> constexpr ArrayView<T> arrayView(ArrayView<T> view) {
    return view;
}

template<class T, class U = decltype(Implementation::ErasedArrayViewConverter<typename std::remove_reference<T&&>::type>::from(std::declval<T&&>()))> constexpr U arrayView(T&& other) {
    return Implementation::ErasedArrayViewConverter<typename std::remove_reference<T&&>::type>::from(std::forward<T>(other));
}

template<class U, class T> ArrayView<U> arrayCast(ArrayView<T> view) {
    static_assert(std::is_standard_layout<T>::value, "the source type is not standard layout");
    static_assert(std::is_standard_layout<U>::value, "the target type is not standard layout");
    const std::size_t size = view.size()*sizeof(T)/sizeof(U);
    CORRADE_ASSERT(size*sizeof(U) == view.size()*sizeof(T),
        "Containers::arrayCast(): can't reinterpret" << view.size() << sizeof(T) << Utility::Debug::nospace << "-byte items into a" << sizeof(U) << Utility::Debug::nospace << "-byte type", {});
    return {reinterpret_cast<U*>(view.begin()), size};
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

        constexpr /*implicit*/ StaticArrayView(std::nullptr_t) noexcept: _data(nullptr) {}

        constexpr /*implicit*/ StaticArrayView() noexcept: _data(nullptr) {}

        template<class U, class = typename std::enable_if<std::is_pointer<U>::value && !std::is_same<U, T(&)[size_]>::value>::type> constexpr explicit StaticArrayView(U data)
        noexcept: _data(data) {}

        template<class U, class = typename std::enable_if<std::is_convertible<U*, T*>::value>::type>
        constexpr /*implicit*/ StaticArrayView(U(&data)[size_]) noexcept: _data{data} {}

        template<class U, class = typename std::enable_if<std::is_convertible<U*, T*>::value>::type>
        constexpr /*implicit*/ StaticArrayView(StaticArrayView<size_, U> view) noexcept: _data{view} {
            static_assert(sizeof(T) == sizeof(U), "type sizes are not compatible");
        }

        template<class U, class = decltype(Implementation::StaticArrayViewConverter<size_, T, typename std::decay<U&&>::type>::from(std::declval<U&&>()))> constexpr /*implicit*/ StaticArrayView(U&& other) noexcept: StaticArrayView{Implementation::StaticArrayViewConverter<size_, T, typename std::decay<U&&>::type>::from(std::forward<U>(other))} {}

        template<class U, class = decltype(Implementation::StaticArrayViewConverter<size_, T, U>::to(std::declval<StaticArrayView<size_, T>>()))> constexpr /*implicit*/ operator U() const {
            return Implementation::StaticArrayViewConverter<size_, T, U>::to(*this);
        }

        #ifndef CORRADE_MSVC2017_COMPATIBILITY
        constexpr explicit operator bool() const { return _data; }
        #endif

        constexpr /*implicit*/ operator T*() const { return _data; }

        constexpr T* data() const { return _data; }

        constexpr std::size_t size() const { return size_; }

        constexpr bool empty() const { return !size_; }

        constexpr T* begin() const { return _data; }
        constexpr T* cbegin() const { return _data; }

        constexpr T* end() const { return _data + size_; }
        constexpr T* cend() const { return _data + size_; }

        T& front() const;

        T& back() const;

        constexpr ArrayView<T> slice(T* begin, T* end) const {
            return ArrayView<T>(*this).slice(begin, end);
        }
        constexpr ArrayView<T> slice(std::size_t begin, std::size_t end) const {
            return ArrayView<T>(*this).slice(begin, end);
        }

        template<std::size_t viewSize> constexpr StaticArrayView<viewSize, T> slice(T* begin) const {
            return ArrayView<T>(*this).template slice<viewSize>(begin);
        }
        template<std::size_t viewSize> constexpr StaticArrayView<viewSize, T> slice(std::size_t begin) const {
            return ArrayView<T>(*this).template slice<viewSize>(begin);
        }

        constexpr ArrayView<T> prefix(T* end) const {
            return ArrayView<T>(*this).prefix(end);
        }
        constexpr ArrayView<T> prefix(std::size_t end) const {
            return ArrayView<T>(*this).prefix(end);
        }

        template<std::size_t viewSize> constexpr StaticArrayView<viewSize, T> prefix() const;

        constexpr ArrayView<T> suffix(T* begin) const {
            return ArrayView<T>(*this).suffix(begin);
        }
        constexpr ArrayView<T> suffix(std::size_t begin) const {
            return ArrayView<T>(*this).suffix(begin);
        }

    private:
        T* _data;
};

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
    return Implementation::ErasedStaticArrayViewConverter<typename std::remove_reference<T&&>::type>::from(std::forward<T>(other));
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

template<class T> T& ArrayView<T>::front() const {
    CORRADE_ASSERT(_size, "Containers::ArrayView::front(): view is empty", _data[0]);
    return _data[0];
}

template<class T> T& ArrayView<T>::back() const {
    CORRADE_ASSERT(_size, "Containers::ArrayView::back(): view is empty", _data[_size - 1]);
    return _data[_size - 1];
}

template<class T> constexpr ArrayView<T> ArrayView<T>::slice(T* begin, T* end) const {
    return CORRADE_CONSTEXPR_ASSERT(_data <= begin && begin <= end && end <= _data + _size,
            "Containers::ArrayView::slice(): slice ["
            << Utility::Debug::nospace << begin - _data
            << Utility::Debug::nospace << ":"
            << Utility::Debug::nospace << end - _data
            << Utility::Debug::nospace << "] out of range for" << _size
            << "elements"),
        ArrayView<T>{begin, std::size_t(end - begin)};
}

template<class T> constexpr ArrayView<T> ArrayView<T>::slice(std::size_t begin, std::size_t end) const {
    return CORRADE_CONSTEXPR_ASSERT(begin <= end && end <= _size,
            "Containers::ArrayView::slice(): slice ["
            << Utility::Debug::nospace << begin
            << Utility::Debug::nospace << ":"
            << Utility::Debug::nospace << end
            << Utility::Debug::nospace << "] out of range for" << _size
            << "elements"),
        ArrayView<T>{_data + begin, end - begin};
}

template<std::size_t size_, class T> T& StaticArrayView<size_, T>::front() const {
    static_assert(size_, "view is empty");
    return _data[0];
}

template<std::size_t size_, class T> T& StaticArrayView<size_, T>::back() const {
    static_assert(size_, "view is empty");
    return _data[size_ - 1];
}

template<class T> template<std::size_t viewSize> constexpr StaticArrayView<viewSize, T> ArrayView<T>::slice(T* begin) const {
    return CORRADE_CONSTEXPR_ASSERT(_data <= begin && begin + viewSize <= _data + _size,
            "Containers::ArrayView::slice(): slice ["
            << Utility::Debug::nospace << begin - _data
            << Utility::Debug::nospace << ":"
            << Utility::Debug::nospace << begin + viewSize - _data
            << Utility::Debug::nospace << "] out of range for" << _size
            << "elements"),
        StaticArrayView<viewSize, T>{begin};
}

template<class T> template<std::size_t viewSize> constexpr StaticArrayView<viewSize, T> ArrayView<T>::slice(std::size_t begin) const {
    return CORRADE_CONSTEXPR_ASSERT(begin + viewSize <= _size,
            "Containers::ArrayView::slice(): slice ["
            << Utility::Debug::nospace << begin
            << Utility::Debug::nospace << ":"
            << Utility::Debug::nospace << begin + viewSize
            << Utility::Debug::nospace << "] out of range for" << _size
            << "elements"),
        StaticArrayView<viewSize, T>{_data + begin};
}

template<std::size_t size_, class T> template<std::size_t viewSize> constexpr StaticArrayView<viewSize, T> StaticArrayView<size_, T>::prefix() const {
    static_assert(viewSize <= size_, "prefix size too large");
    return StaticArrayView<viewSize, T>{_data};
}

}}

#endif
#ifdef CORRADE_ARRAYVIEW_STL_COMPATIBILITY
#include <array>
#include <vector>
#ifndef Corrade_Containers_ArrayViewStl_h
#define Corrade_Containers_ArrayViewStl_h

namespace Corrade { namespace Containers { namespace Implementation {

template<std::size_t size, class T> struct ArrayViewConverter<T, std::array<T, size>> {
    constexpr static ArrayView<T> from(std::array<T, size>& other) {
        return {&other[0], other.size()};
    }
};
template<std::size_t size, class T> struct ArrayViewConverter<const T, std::array<T, size>> {
    constexpr static ArrayView<const T> from(const std::array<T, size>& other) {
        return {&other[0], other.size()};
    }
};
template<std::size_t size, class T> struct ErasedArrayViewConverter<std::array<T, size>>: ArrayViewConverter<T, std::array<T, size>> {};
template<std::size_t size, class T> struct ErasedArrayViewConverter<const std::array<T, size>>: ArrayViewConverter<const T, std::array<T, size>> {};

template<class T, class Allocator> struct ArrayViewConverter<T, std::vector<T, Allocator>> {
    static ArrayView<T> from(std::vector<T, Allocator>& other) {
        return {&other[0], other.size()};
    }
};
template<class T, class Allocator> struct ArrayViewConverter<const T, std::vector<T, Allocator>> {
    static ArrayView<const T> from(const std::vector<T, Allocator>& other) {
        return {&other[0], other.size()};
    }
};
template<class T, class Allocator> struct ErasedArrayViewConverter<std::vector<T, Allocator>>: ArrayViewConverter<T, std::vector<T, Allocator>> {};
template<class T, class Allocator> struct ErasedArrayViewConverter<const std::vector<T, Allocator>>: ArrayViewConverter<const T, std::vector<T, Allocator>> {};

template<std::size_t size, class T> struct StaticArrayViewConverter<size, T, std::array<T, size>> {
    constexpr static StaticArrayView<size, T> from(std::array<T, size>& other) {
        return StaticArrayView<size, T>{&other[0]};
    }
};
template<std::size_t size, class T> struct StaticArrayViewConverter<size, const T, std::array<T, size>> {
    constexpr static StaticArrayView<size, const T> from(const std::array<T, size>& other) {
        return StaticArrayView<size, const T>(&other[0]);
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

template<class T, std::ptrdiff_t Extent> struct ArrayViewConverter<T, std::span<T, Extent>> {
    constexpr static ArrayView<T> from(std::span<T, Extent> other) {
        return {other.data(), std::size_t(other.size())};
    }
};
template<class T, std::ptrdiff_t Extent> struct ArrayViewConverter<const T, std::span<T, Extent>> {
    constexpr static ArrayView<T> from(std::span<T, Extent> other) {
        return {other.data(), std::size_t(other.size())};
    }
};
template<class T, std::ptrdiff_t Extent> struct ArrayViewConverter<T, const std::span<T, Extent>>: ArrayViewConverter<T, std::span<T, Extent>> {};
template<class T, std::ptrdiff_t Extent> struct ErasedArrayViewConverter<std::span<T, Extent>>: ArrayViewConverter<T, std::span<T, Extent>> {};
template<class T, std::ptrdiff_t Extent> struct ErasedArrayViewConverter<const std::span<T, Extent>>: ArrayViewConverter<T, std::span<T, Extent>> {};

template<std::size_t size, class T> struct StaticArrayViewConverter<size, T, std::span<T, std::ptrdiff_t(size)>> {
    constexpr static StaticArrayView<size, T> from(std::span<T> other) {
        return StaticArrayView<size, T>{other.data()};
    }
};
template<std::size_t size, class T> struct StaticArrayViewConverter<size, T, const std::span<T, std::ptrdiff_t(size)>>: StaticArrayViewConverter<size, T, std::span<T, std::ptrdiff_t(size)>> {};
template<std::size_t size, class T> struct StaticArrayViewConverter<size, const T, std::span<T, std::ptrdiff_t(size)>> {
    constexpr static StaticArrayView<size, const T> from(std::span<T> other) {
        return StaticArrayView<size, const T>{other.data()};
    }
};
template<class T, std::ptrdiff_t Extent> struct ErasedStaticArrayViewConverter<std::span<T, Extent>>: StaticArrayViewConverter<std::size_t(Extent), T, std::span<T, Extent>> {
    static_assert(Extent >= 0, "can't convert dynamic std::span to StaticArrayView");
};
template<class T, std::ptrdiff_t Extent> struct ErasedStaticArrayViewConverter<const std::span<T, Extent>>: StaticArrayViewConverter<std::size_t(Extent), T, std::span<T, Extent>> {
    static_assert(Extent >= 0, "can't convert dynamic std::span to StaticArrayView");
};

}}}

#endif
#endif
