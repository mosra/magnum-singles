/*
    Growable APIs for Corrade::Containers::Array

    https://doc.magnum.graphics/corrade/classCorrade_1_1Containers_1_1Array.html#Containers-Array-growable

    Depends on CorradeArray.h.

    This is a single-header library generated from the Corrade project. With
    the goal being easy integration, it's deliberately free of all comments
    to keep the file size small. More info, detailed changelogs and docs here:

    -   Project homepage — https://magnum.graphics/corrade/
    -   Documentation — https://doc.magnum.graphics/corrade/
    -   GitHub project page — https://github.com/mosra/corrade
    -   GitHub Singles repository — https://github.com/mosra/magnum-singles

    The library makes use of AddressSanitizer annotations if compiling with
    ASan enabled, you can `#define CORRADE_CONTAINERS_NO_SANITIZER_ANNOTATIONS`
    to disable them.

    v2020.06-1687-g6b5f (2024-06-29)
    -   Minor cleanup, some macro logic is now moved to CorradeArrayView.h
    v2020.06-1507-gfbd9 (2023-09-13)
    -   Initial release

    Generated from Corrade v2020.06-1687-g6b5f (2024-06-29), 1067 / 4311 LoC
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

#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <new>

#include "CorradeArray.h"

#ifndef Corrade_Containers_initializeHelpers_h
#define Corrade_Containers_initializeHelpers_h

namespace Corrade { namespace Containers { namespace Implementation {

enum: std::size_t {
    DefaultAllocationAlignment =
        #if defined(__STDCPP_DEFAULT_NEW_ALIGNMENT__) && !defined(CORRADE_TARGET_EMSCRIPTEN)
        __STDCPP_DEFAULT_NEW_ALIGNMENT__
        #else
        2*sizeof(std::size_t)
        #endif
};

template<class T, typename std::enable_if<
    #ifdef CORRADE_NO_STD_IS_TRIVIALLY_TRAITS
    __has_trivial_constructor(T)
    #else
    std::is_trivially_constructible<T>::value
    #endif
, int>::type = 0> inline void arrayConstruct(Corrade::DefaultInitT, T*, T*) {
}

template<class T, typename std::enable_if<!
    #ifdef CORRADE_NO_STD_IS_TRIVIALLY_TRAITS
    __has_trivial_constructor(T)
    #else
    std::is_trivially_constructible<T>::value
    #endif
, int>::type = 0> inline void arrayConstruct(Corrade::DefaultInitT, T* begin, T* const end) {
    for(; begin < end; ++begin) new(begin) T;
}

template<class T, typename std::enable_if<
    #ifdef CORRADE_NO_STD_IS_TRIVIALLY_TRAITS
    __has_trivial_constructor(T)
    #else
    std::is_trivially_constructible<T>::value
    #endif
, int>::type = 0> inline void arrayConstruct(Corrade::ValueInitT, T* const begin, T* const end) {
    if(begin < end) std::memset(begin, 0, (end - begin)*sizeof(T));
}

template<class T, typename std::enable_if<!
    #ifdef CORRADE_NO_STD_IS_TRIVIALLY_TRAITS
    __has_trivial_constructor(T)
    #else
    std::is_trivially_constructible<T>::value
    #endif
, int>::type = 0> inline void arrayConstruct(Corrade::ValueInitT, T* begin, T* const end) {
    for(; begin < end; ++begin) new(begin) T();
}

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
#ifndef Corrade_Containers_GrowableArray_h
#define Corrade_Containers_GrowableArray_h

#ifndef CORRADE_CONTAINERS_NO_SANITIZER_ANNOTATIONS
#ifdef __has_feature
#if __has_feature(address_sanitizer)
#define _CORRADE_CONTAINERS_SANITIZER_ENABLED
#endif
#endif
#ifdef __SANITIZE_ADDRESS__
#define _CORRADE_CONTAINERS_SANITIZER_ENABLED
#endif
#endif

#ifdef _CORRADE_CONTAINERS_SANITIZER_ENABLED
extern "C" void __sanitizer_annotate_contiguous_container(const void *beg,
    const void *end, const void *old_mid, const void *new_mid)
        #if defined(CORRADE_TARGET_DINKUMWARE) && _MSC_VER < 1936
        noexcept
        #endif
    ;
#endif

namespace Corrade { namespace Containers {

namespace Implementation {

template<class T> struct AllocatorTraits {
    enum: std::size_t {
        Offset = alignof(T) < sizeof(std::size_t) ? sizeof(std::size_t) :
            (alignof(T) < Implementation::DefaultAllocationAlignment ?
                alignof(T) : Implementation::DefaultAllocationAlignment)
    };
};

}

template<class T> struct ArrayNewAllocator {
    typedef T Type;

    enum: std::size_t {
        AllocationOffset = Implementation::AllocatorTraits<T>::Offset
    };

    static T* allocate(std::size_t capacity) {
        char* const memory = new char[capacity*sizeof(T) + AllocationOffset];
        reinterpret_cast<std::size_t*>(memory)[0] = capacity;
        return reinterpret_cast<T*>(memory + AllocationOffset);
    }

    static void reallocate(T*& array, std::size_t prevSize, std::size_t newCapacity);

    static void deallocate(T* data) {
        delete[] (reinterpret_cast<char*>(data) - AllocationOffset);
    }

    static std::size_t grow(T* array, std::size_t desired);

    static std::size_t capacity(T* array) {
        return *reinterpret_cast<std::size_t*>(reinterpret_cast<char*>(array) - AllocationOffset);
    }

    static void* base(T* array) {
        return reinterpret_cast<char*>(array) - AllocationOffset;
    }

    static void deleter(T* data, std::size_t size) {
        for(T *it = data, *end = data + size; it != end; ++it) it->~T();
        deallocate(data);
    }
};

template<class T> struct ArrayMallocAllocator {
    static_assert(
        #ifdef CORRADE_NO_STD_IS_TRIVIALLY_TRAITS
        __has_trivial_copy(T) && __has_trivial_destructor(T)
        #else
        std::is_trivially_copyable<T>::value
        #endif
        , "only trivially copyable types are usable with this allocator");

    typedef T Type;

    enum: std::size_t {
        AllocationOffset = Implementation::AllocatorTraits<T>::Offset
    };

    static T* allocate(std::size_t capacity) {
        const std::size_t inBytes = capacity*sizeof(T) + AllocationOffset;
        char* const memory = static_cast<char*>(std::malloc(inBytes));
        CORRADE_ASSERT(memory,
            "Containers::ArrayMallocAllocator: can't allocate" << inBytes << "bytes", {});
        reinterpret_cast<std::size_t*>(memory)[0] = inBytes;
        return reinterpret_cast<T*>(memory + AllocationOffset);
    }

    static void reallocate(T*& array, std::size_t prevSize, std::size_t newCapacity);

    static void deallocate(T* data) {
        if(data) std::free(reinterpret_cast<char*>(data) - AllocationOffset);
    }

    static std::size_t grow(T* array, std::size_t desired);

    static std::size_t capacity(T* array) {
        return (*reinterpret_cast<std::size_t*>(reinterpret_cast<char*>(array) - AllocationOffset) - AllocationOffset)/sizeof(T);
    }

    static void* base(T* array) {
        return reinterpret_cast<char*>(array) - AllocationOffset;
    }

    static void deleter(T* data, std::size_t size) {
        static_cast<void>(size);
        deallocate(data);
    }
};

template<class T> using ArrayAllocator = typename std::conditional<
    #ifdef CORRADE_NO_STD_IS_TRIVIALLY_TRAITS
    __has_trivial_copy(T) && __has_trivial_destructor(T)
    #else
    std::is_trivially_copyable<T>::value
    #endif
    , ArrayMallocAllocator<T>, ArrayNewAllocator<T>>::type;

template<class U, class T> Array<U> arrayAllocatorCast(Array<T>&& array);

template<class U, template<class> class Allocator, class T> Array<U> arrayAllocatorCast(Array<T>&& array) {
    static_assert(std::is_standard_layout<T>::value, "the source type is not standard layout");
    static_assert(std::is_standard_layout<U>::value, "the target type is not standard layout");
    static_assert(
        #ifdef CORRADE_NO_STD_IS_TRIVIALLY_TRAITS
        __has_trivial_copy(T) && __has_trivial_destructor(T) && __has_trivial_copy(U) && __has_trivial_destructor(U)
        #else
        std::is_trivially_copyable<T>::value && std::is_trivially_copyable<U>::value
        #endif
        , "only trivially copyable types can use the allocator cast");

    if(array.isEmpty() && !array.data() && !array.deleter())
        return {};

    CORRADE_ASSERT(array.deleter() == Allocator<T>::deleter && (std::is_base_of<ArrayMallocAllocator<T>, Allocator<T>>::value),
        "Containers::arrayAllocatorCast(): the array has to use the ArrayMallocAllocator or a derivative", {});
    const std::size_t size = array.size()*sizeof(T)/sizeof(U);
    CORRADE_ASSERT(size*sizeof(U) == array.size()*sizeof(T),
        "Containers::arrayAllocatorCast(): can't reinterpret" << array.size() << sizeof(T) << Utility::Debug::nospace << "-byte items into a" << sizeof(U) << Utility::Debug::nospace << "-byte type", {});
    return Array<U>{reinterpret_cast<U*>(array.release()), size, Allocator<U>::deleter};
}

template<class U, class T> Array<U> arrayAllocatorCast(Array<T>&& array) {
    return arrayAllocatorCast<U, ArrayAllocator, T>(Utility::move(array));
}

template<class T, class Allocator = ArrayAllocator<T>> bool arrayIsGrowable(Array<T>& array);

template<template<class T> class Allocator, class T> inline bool arrayIsGrowable(Array<T>& array) {
    return arrayIsGrowable<T, Allocator<T>>(array);
}

template<class T, class Allocator = ArrayAllocator<T>> std::size_t arrayCapacity(Array<T>& array);

template<template<class T> class Allocator, class T> inline std::size_t arrayCapacity(Array<T>& array) {
    return arrayCapacity<T, Allocator<T>>(array);
}

template<class T, class Allocator = ArrayAllocator<T>> std::size_t arrayReserve(Array<T>& array, std::size_t capacity);

template<template<class T> class Allocator, class T> inline std::size_t arrayReserve(Array<T>& array, std::size_t capacity) {
    return arrayReserve<T, Allocator<T>>(array, capacity);
}

template<class T, class Allocator = ArrayAllocator<T>> void arrayResize(Array<T>& array, Corrade::DefaultInitT, std::size_t size);

template<template<class> class Allocator, class T> inline void arrayResize(Array<T>& array, Corrade::DefaultInitT, std::size_t size) {
    arrayResize<T, Allocator<T>>(array, Corrade::DefaultInit, size);
}

template<class T, class Allocator = ArrayAllocator<T>> void arrayResize(Array<T>& array, Corrade::ValueInitT, std::size_t size);

template<template<class> class Allocator, class T> inline void arrayResize(Array<T>& array, Corrade::ValueInitT, std::size_t size) {
    arrayResize<T, Allocator<T>>(array, Corrade::ValueInit, size);
}

template<class T, class Allocator = ArrayAllocator<T>> inline void arrayResize(Array<T>& array, std::size_t size) {
    return arrayResize<T, Allocator>(array, Corrade::ValueInit, size);
}

template<template<class> class Allocator, class T> inline void arrayResize(Array<T>& array, std::size_t size) {
    arrayResize<T, Allocator<T>>(array, size);
}

template<class T, class Allocator = ArrayAllocator<T>> void arrayResize(Array<T>& array, Corrade::NoInitT, std::size_t size);

template<template<class> class Allocator, class T> inline void arrayResize(Array<T>& array, Corrade::NoInitT, std::size_t size) {
    arrayResize<T, Allocator<T>>(array, Corrade::NoInit, size);
}

template<class T, class ...Args> void arrayResize(Array<T>& array, Corrade::DirectInitT, std::size_t size, Args&&... args);

template<class T, class Allocator, class ...Args> void arrayResize(Array<T>& array, Corrade::DirectInitT, std::size_t size, Args&&... args);

template<template<class> class Allocator, class T, class ...Args> inline void arrayResize(Array<T>& array, Corrade::DirectInitT, std::size_t size, Args&&... args) {
    arrayResize<T, Allocator<T>>(array, Corrade::DirectInit, size, Utility::forward<Args>(args)...);
}

template<class T, class Allocator = ArrayAllocator<T>> inline void arrayResize(Array<T>& array, std::size_t size, const typename std::common_type<T>::type& value) {
    arrayResize<T, Allocator>(array, Corrade::DirectInit, size, value);
}

template<template<class> class Allocator, class T> inline void arrayResize(Array<T>& array, std::size_t size, const typename std::common_type<T>::type& value) {
    arrayResize<T, Allocator<T>>(array, size, value);
}

template<class T, class Allocator = ArrayAllocator<T>> T& arrayAppend(Array<T>& array, const typename std::common_type<T>::type& value);

template<template<class> class Allocator, class T> inline T& arrayAppend(Array<T>& array, const typename std::common_type<T>::type& value) {
    return arrayAppend<T, Allocator<T>>(array, value);
}

template<class T, class ...Args> T& arrayAppend(Array<T>& array, Corrade::InPlaceInitT, Args&&... args);

template<class T, class Allocator, class ...Args> T& arrayAppend(Array<T>& array, Corrade::InPlaceInitT, Args&&... args);

template<template<class> class Allocator, class T, class ...Args> inline T& arrayAppend(Array<T>& array, Corrade::InPlaceInitT, Args&&... args) {
    return arrayAppend<T, Allocator<T>>(array, Corrade::InPlaceInit, Utility::forward<Args>(args)...);
}

template<class T, class Allocator = ArrayAllocator<T>> inline T& arrayAppend(Array<T>& array, typename std::common_type<T>::type&& value) {
    return arrayAppend<T, Allocator>(array, Corrade::InPlaceInit, Utility::move(value));
}

template<template<class> class Allocator, class T> inline T& arrayAppend(Array<T>& array, typename std::common_type<T>::type&& value) {
    return arrayAppend<T, Allocator<T>>(array, Corrade::InPlaceInit, Utility::move(value));
}

template<class T, class Allocator = ArrayAllocator<T>> ArrayView<T> arrayAppend(Array<T>& array, typename std::common_type<ArrayView<const T>>::type values);

template<template<class> class Allocator, class T> inline ArrayView<T> arrayAppend(Array<T>& array, typename std::common_type<ArrayView<const T>>::type values) {
    return arrayAppend<T, Allocator<T>>(array, values);
}

template<class T, class Allocator = ArrayAllocator<T>> inline ArrayView<T>  arrayAppend(Array<T>& array, std::initializer_list<typename std::common_type<T>::type> values) {
    return arrayAppend<T, Allocator>(array, arrayView(values));
}

template<template<class> class Allocator, class T> inline ArrayView<T>  arrayAppend(Array<T>& array, std::initializer_list<typename std::common_type<T>::type> values) {
    return arrayAppend<T, Allocator<T>>(array, values);
}

template<class T, class Allocator = ArrayAllocator<T>> ArrayView<T> arrayAppend(Array<T>& array, Corrade::NoInitT, std::size_t count);

template<template<class> class Allocator, class T> inline ArrayView<T> arrayAppend(Array<T>& array, Corrade::NoInitT, std::size_t count) {
    return arrayAppend<T, Allocator<T>>(array, Corrade::NoInit, count);
}

template<class T, class Allocator = ArrayAllocator<T>> T& arrayInsert(Array<T>& array, std::size_t index, const typename std::common_type<T>::type& value);

template<template<class> class Allocator, class T> T& arrayInsert(Array<T>& array, std::size_t index, const typename std::common_type<T>::type& value) {
    return arrayInsert<T, Allocator<T>>(array, index, value);
}

template<class T, class ...Args> T& arrayInsert(Array<T>& array, std::size_t index, Corrade::InPlaceInitT, Args&&... args);

template<class T, class Allocator, class ...Args> T& arrayInsert(Array<T>& array, std::size_t index, Corrade::InPlaceInitT, Args&&... args);

template<template<class> class Allocator, class T, class ...Args> T& arrayInsert(Array<T>& array, std::size_t index, Corrade::InPlaceInitT, Args&&... args) {
    return arrayInsert<T, Allocator<T>>(array, index, Utility::forward<Args>(args)...);
}

template<class T, class Allocator = ArrayAllocator<T>> inline T& arrayInsert(Array<T>& array, std::size_t index, typename std::common_type<T>::type&& value) {
    return arrayInsert<T, Allocator>(array, index, Corrade::InPlaceInit, Utility::move(value));
}

template<template<class> class Allocator, class T> inline T& arrayInsert(Array<T>& array, std::size_t index, typename std::common_type<T>::type&& value) {
    return arrayInsert<T, Allocator<T>>(array, index, Corrade::InPlaceInit, Utility::move(value));
}

template<class T, class Allocator = ArrayAllocator<T>> ArrayView<T> arrayInsert(Array<T>& array, std::size_t index, typename std::common_type<ArrayView<const T>>::type values);

template<template<class> class Allocator, class T> inline ArrayView<T> arrayInsert(Array<T>& array, std::size_t index, typename std::common_type<ArrayView<const T>>::type values) {
    return arrayInsert<T, Allocator<T>>(array, index, values);
}

template<class T, class Allocator = ArrayAllocator<T>> ArrayView<T>  arrayInsert(Array<T>& array, std::size_t index, std::initializer_list<typename std::common_type<T>::type> values) {
    return arrayInsert<T, Allocator>(array, index, arrayView(values));
}

template<template<class> class Allocator, class T> inline ArrayView<T>  arrayInsert(Array<T>& array, std::size_t index, std::initializer_list<typename std::common_type<T>::type> values) {
    return arrayInsert<T, Allocator<T>>(array, index, values);
}

template<class T, class Allocator = ArrayAllocator<T>> ArrayView<T> arrayInsert(Array<T>& array, std::size_t index, Corrade::NoInitT, std::size_t count);

template<template<class> class Allocator, class T> inline ArrayView<T> arrayInsert(Array<T>& array, std::size_t index, Corrade::NoInitT, std::size_t count) {
    return arrayInsert<T, Allocator<T>>(array, index, Corrade::NoInit, count);
}

template<class T, class Allocator = ArrayAllocator<T>> void arrayRemove(Array<T>& array, std::size_t index, std::size_t count = 1);

template<template<class> class Allocator, class T> inline void arrayRemove(Array<T>& array, std::size_t index, std::size_t count = 1) {
    arrayRemove<T, Allocator<T>>(array, index, count);
}

template<class T, class Allocator = ArrayAllocator<T>> void arrayRemoveUnordered(Array<T>& array, std::size_t index, std::size_t count = 1);

template<template<class> class Allocator, class T> inline void arrayRemoveUnordered(Array<T>& array, std::size_t index, std::size_t count = 1) {
    arrayRemoveUnordered<T, Allocator<T>>(array, index, count);
}

template<class T, class Allocator = ArrayAllocator<T>> void arrayRemoveSuffix(Array<T>& array, std::size_t count = 1);

template<template<class> class Allocator, class T> inline void arrayRemoveSuffix(Array<T>& array, std::size_t count = 1) {
    arrayRemoveSuffix<T, Allocator<T>>(array, count);
}

template<class T, class Allocator = ArrayAllocator<T>> void arrayShrink(Array<T>& array, Corrade::NoInitT = Corrade::NoInit);

template<class T, class Allocator = ArrayAllocator<T>> void arrayShrink(Array<T>& array, Corrade::DefaultInitT);

template<template<class> class Allocator, class T> inline void arrayShrink(Array<T>& array) {
    arrayShrink<T, Allocator<T>>(array);
}

namespace Implementation {

template<class T> struct ArrayGuts {
    T* data;
    std::size_t size;
    void(*deleter)(T*, std::size_t);
};

template<class T, typename std::enable_if<
    #ifdef CORRADE_NO_STD_IS_TRIVIALLY_TRAITS
    __has_trivial_copy(T) && __has_trivial_destructor(T)
    #else
    std::is_trivially_copyable<T>::value
    #endif
, int>::type = 0> inline void arrayMoveConstruct(T* const src, T* const dst, const std::size_t count) {
    if(count) std::memcpy(dst, src, count*sizeof(T));
}

template<class T, typename std::enable_if<
    #ifdef CORRADE_NO_STD_IS_TRIVIALLY_TRAITS
    !__has_trivial_copy(T) || !__has_trivial_destructor(T)
    #else
    !std::is_trivially_copyable<T>::value
    #endif
, int>::type = 0> inline void arrayMoveConstruct(T* src, T* dst, const std::size_t count) {
    static_assert(std::is_nothrow_move_constructible<T>::value,
        "nothrow move-constructible type is required");
    for(T* end = src + count; src != end; ++src, ++dst)
        #if defined(CORRADE_TARGET_GCC) && !defined(CORRADE_TARGET_CLANG) && __GNUC__ < 5
        Implementation::construct(*dst, Utility::move(*src));
        #else
        new(dst) T{Utility::move(*src)};
        #endif
}

template<class T, typename std::enable_if<
    #ifdef CORRADE_NO_STD_IS_TRIVIALLY_TRAITS
    __has_trivial_copy(T) && __has_trivial_destructor(T)
    #else
    std::is_trivially_copyable<T>::value
    #endif
, int>::type = 0> inline void arrayMoveAssign(T* const src, T* const dst, const std::size_t count) {
    if(count) std::memcpy(dst, src, count*sizeof(T));
}

template<class T, typename std::enable_if<
    #ifdef CORRADE_NO_STD_IS_TRIVIALLY_TRAITS
    !__has_trivial_copy(T) || !__has_trivial_destructor(T)
    #else
    !std::is_trivially_copyable<T>::value
    #endif
, int>::type = 0> inline void arrayMoveAssign(T* src, T* dst, const std::size_t count) {
    static_assert(std::is_nothrow_move_assignable<T>::value,
        "nothrow move-assignable type is required");
    for(T* end = src + count; src != end; ++src, ++dst)
        *dst = Utility::move(*src);
}

template<class T, typename std::enable_if<
    #ifdef CORRADE_NO_STD_IS_TRIVIALLY_TRAITS
    __has_trivial_copy(T) && __has_trivial_destructor(T)
    #else
    std::is_trivially_copyable<T>::value
    #endif
, int>::type = 0> inline void arrayCopyConstruct(const T* const src, T* const dst, const std::size_t count) {
    if(count) std::memcpy(dst, src, count*sizeof(T));
}

template<class T, typename std::enable_if<
    #ifdef CORRADE_NO_STD_IS_TRIVIALLY_TRAITS
    !__has_trivial_copy(T) || !__has_trivial_destructor(T)
    #else
    !std::is_trivially_copyable<T>::value
    #endif
, int>::type = 0> inline void arrayCopyConstruct(const T* src, T* dst, const std::size_t count) {
    for(const T* end = src + count; src != end; ++src, ++dst)
        #if defined(CORRADE_TARGET_GCC) && !defined(CORRADE_TARGET_CLANG) &&  __GNUC__ < 5
        Implementation::construct(*dst, *src);
        #else
        new(dst) T{*src};
        #endif
}

template<class T> inline void arrayDestruct(T*, T*, typename std::enable_if<std::is_trivially_destructible<T>::value>::type* = nullptr) {
}

template<class T> inline void arrayDestruct(T* begin, T* const end, typename std::enable_if<!std::is_trivially_destructible<T>::value>::type* = nullptr) {
    for(; begin < end; ++begin) begin->~T();
}

template<class T> inline std::size_t arrayGrowth(const std::size_t currentCapacity, const std::size_t desiredCapacity) {
    const std::size_t currentCapacityInBytes = sizeof(T)*currentCapacity + Implementation::AllocatorTraits<T>::Offset;

    std::size_t grown;
    if(currentCapacityInBytes < DefaultAllocationAlignment)
        grown = DefaultAllocationAlignment;
    else if(currentCapacityInBytes < 64)
        grown = currentCapacityInBytes*2;
    else
        grown = currentCapacityInBytes + currentCapacityInBytes/2;

    const std::size_t candidate = (grown - Implementation::AllocatorTraits<T>::Offset)/sizeof(T);
    return desiredCapacity > candidate ? desiredCapacity : candidate;
}

}

template<class T> void ArrayNewAllocator<T>::reallocate(T*& array, const std::size_t prevSize, const std::size_t newCapacity) {
    T* newArray = allocate(newCapacity);
    static_assert(std::is_nothrow_move_constructible<T>::value,
        "nothrow move-constructible type is required");
    for(T *src = array, *end = src + prevSize, *dst = newArray; src != end; ++src, ++dst)
        #if defined(CORRADE_TARGET_GCC) && !defined(CORRADE_TARGET_CLANG) && __GNUC__ < 5
        Implementation::construct(*dst, Utility::move(*src));
        #else
        new(dst) T{Utility::move(*src)};
        #endif
    for(T *it = array, *end = array + prevSize; it < end; ++it) it->~T();
    deallocate(array);
    array = newArray;
}

template<class T> void ArrayMallocAllocator<T>::reallocate(T*& array, std::size_t, const std::size_t newCapacity) {
    const std::size_t inBytes = newCapacity*sizeof(T) + AllocationOffset;
    char* const memory = static_cast<char*>(std::realloc(reinterpret_cast<char*>(array) - AllocationOffset, inBytes));
    CORRADE_ASSERT(memory,
        "Containers::ArrayMallocAllocator: can't reallocate" << inBytes << "bytes", );
    reinterpret_cast<std::size_t*>(memory)[0] = inBytes;
    array = reinterpret_cast<T*>(memory + AllocationOffset);
}

template<class T> std::size_t ArrayNewAllocator<T>::grow(T* const array, const std::size_t desiredCapacity) {
    return Implementation::arrayGrowth<T>(array ? capacity(array) : 0, desiredCapacity);
}

template<class T> std::size_t ArrayMallocAllocator<T>::grow(T* const array, const std::size_t desiredCapacity) {
    return Implementation::arrayGrowth<T>(array ? capacity(array) : 0, desiredCapacity);
}

template<class T, class Allocator> bool arrayIsGrowable(Array<T>& array) {
    return array.deleter() == Allocator::deleter;
}

template<class T, class Allocator> std::size_t arrayCapacity(Array<T>& array) {
    if(array.deleter() == Allocator::deleter)
        return Allocator::capacity(array.data());
    return array.size();
}

template<class T, class Allocator> std::size_t arrayReserve(Array<T>& array, const std::size_t capacity) {
    auto& arrayGuts = reinterpret_cast<Implementation::ArrayGuts<T>&>(array);
    const bool hasGrowingDeleter = arrayGuts.deleter == Allocator::deleter;

    const std::size_t currentCapacity = arrayCapacity<T, Allocator>(array);
    if(currentCapacity >= capacity) return currentCapacity;

    if(!hasGrowingDeleter) {
        T* newArray = Allocator::allocate(capacity);
        Implementation::arrayMoveConstruct<T>(arrayGuts.data, newArray, arrayGuts.size);
        array = Array<T>{newArray, arrayGuts.size, Allocator::deleter};
    } else Allocator::reallocate(arrayGuts.data, arrayGuts.size, capacity);

    #ifdef _CORRADE_CONTAINERS_SANITIZER_ENABLED
    __sanitizer_annotate_contiguous_container(
        Allocator::base(arrayGuts.data),
        arrayGuts.data + capacity,
        arrayGuts.data + capacity,
        arrayGuts.data + arrayGuts.size);
    #endif

    return capacity;
}

template<class T, class Allocator> void arrayResize(Array<T>& array, Corrade::NoInitT, const std::size_t size) {
    auto& arrayGuts = reinterpret_cast<Implementation::ArrayGuts<T>&>(array);
    const bool hasGrowingDeleter = arrayGuts.deleter == Allocator::deleter;

    if(arrayGuts.size == size) return;

    if(!hasGrowingDeleter) {
        T* newArray = Allocator::allocate(size);
        Implementation::arrayMoveConstruct<T>(array, newArray,
            arrayGuts.size < size ? arrayGuts.size : size);
        array = Array<T>{newArray, size, Allocator::deleter};

        #ifdef _CORRADE_CONTAINERS_SANITIZER_ENABLED
        __sanitizer_annotate_contiguous_container(
            Allocator::base(arrayGuts.data),
            arrayGuts.data + arrayGuts.size,
            arrayGuts.data + arrayGuts.size,
            arrayGuts.data + arrayGuts.size);
        #endif

    } else if(Allocator::capacity(array) < size) {
        Allocator::reallocate(arrayGuts.data,
            arrayGuts.size < size ? arrayGuts.size : size, size);
        arrayGuts.size = size;

        #ifdef _CORRADE_CONTAINERS_SANITIZER_ENABLED
        __sanitizer_annotate_contiguous_container(
            Allocator::base(arrayGuts.data),
            arrayGuts.data + arrayGuts.size,
            arrayGuts.data + arrayGuts.size,
            arrayGuts.data + arrayGuts.size);
        #endif

    } else {
        Implementation::arrayDestruct<T>(arrayGuts.data + size, arrayGuts.data + arrayGuts.size);
        #ifdef _CORRADE_CONTAINERS_SANITIZER_ENABLED
        __sanitizer_annotate_contiguous_container(
            Allocator::base(arrayGuts.data),
            arrayGuts.data + Allocator::capacity(array),
            arrayGuts.data + arrayGuts.size,
            arrayGuts.data + size);
        #endif
        arrayGuts.size = size;
    }
}

template<class T, class Allocator> void arrayResize(Array<T>& array, Corrade::DefaultInitT, const std::size_t size) {
    const std::size_t prevSize = array.size();
    arrayResize<T, Allocator>(array, Corrade::NoInit, size);
    Implementation::arrayConstruct(Corrade::DefaultInit, array + prevSize, array.end());
}

template<class T, class Allocator> void arrayResize(Array<T>& array, Corrade::ValueInitT, const std::size_t size) {
    const std::size_t prevSize = array.size();
    arrayResize<T, Allocator>(array, Corrade::NoInit, size);
    Implementation::arrayConstruct(Corrade::ValueInit, array + prevSize, array.end());
}

template<class T, class Allocator, class ...Args> void arrayResize(Array<T>& array, Corrade::DirectInitT, const std::size_t size, Args&&... args) {
    const std::size_t prevSize = array.size();
    arrayResize<T, Allocator>(array, Corrade::NoInit, size);

    for(T* it = array + prevSize; it < array.end(); ++it)
        Implementation::construct(*it, Utility::forward<Args>(args)...);
}

template<class T, class ...Args> inline void arrayResize(Array<T>& array, Corrade::DirectInitT, const std::size_t size, Args&&... args) {
    arrayResize<T, ArrayAllocator<T>, Args...>(array, Corrade::DirectInit, size, Utility::forward<Args>(args)...);
}

namespace Implementation {

template<class T, class Allocator> T* arrayGrowBy(Array<T>& array, const std::size_t count) {
    auto& arrayGuts = reinterpret_cast<Implementation::ArrayGuts<T>&>(array);

    if(!count)
        return arrayGuts.data + arrayGuts.size;

    const std::size_t desiredCapacity = arrayGuts.size + count;
    std::size_t capacity;
    #ifdef _CORRADE_CONTAINERS_SANITIZER_ENABLED
    T* oldMid = nullptr;
    #endif
    if(arrayGuts.deleter != Allocator::deleter) {
        capacity = Allocator::grow(nullptr, desiredCapacity);
        T* const newArray = Allocator::allocate(capacity);
        arrayMoveConstruct<T>(arrayGuts.data, newArray, arrayGuts.size);
        array = Array<T>{newArray, arrayGuts.size, Allocator::deleter};

    } else {
        capacity = Allocator::capacity(arrayGuts.data);
        if(arrayGuts.size + count > capacity) {
            capacity = Allocator::grow(arrayGuts.data, desiredCapacity);
            Allocator::reallocate(arrayGuts.data, arrayGuts.size, capacity);
        } else {
            #ifdef _CORRADE_CONTAINERS_SANITIZER_ENABLED
            oldMid = arrayGuts.data + arrayGuts.size;
            #endif
        }
    }

    T* const it = arrayGuts.data + arrayGuts.size;
    #ifdef _CORRADE_CONTAINERS_SANITIZER_ENABLED
    __sanitizer_annotate_contiguous_container(
        Allocator::base(arrayGuts.data),
        arrayGuts.data + capacity,
        oldMid ? oldMid : arrayGuts.data + capacity,
        arrayGuts.data + arrayGuts.size + count);
    #endif
    arrayGuts.size += count;
    return it;
}

}

template<class T, class Allocator> inline T& arrayAppend(Array<T>& array, const typename std::common_type<T>::type& value) {
    T* const it = Implementation::arrayGrowBy<T, Allocator>(array, 1);
    #if defined(CORRADE_TARGET_GCC) && !defined(CORRADE_TARGET_CLANG) &&  __GNUC__ < 5
    Implementation::construct(*it, value);
    #else
    new(it) T{value};
    #endif
    return *it;
}

template<class T, class Allocator> inline ArrayView<T> arrayAppend(Array<T>& array, const typename std::common_type<ArrayView<const T>>::type values) {
    const std::size_t valueCount = values.size();

    T* const it = Implementation::arrayGrowBy<T, Allocator>(array, valueCount);
    Implementation::arrayCopyConstruct<T>(values.data(), it, valueCount);
    return {it, valueCount};
}

template<class T, class ...Args> inline T& arrayAppend(Array<T>& array, Corrade::InPlaceInitT, Args&&... args) {
    return arrayAppend<T, ArrayAllocator<T>>(array, Corrade::InPlaceInit, Utility::forward<Args>(args)...);
}

template<class T, class Allocator, class ...Args> T& arrayAppend(Array<T>& array, Corrade::InPlaceInitT, Args&&... args) {
    T* const it = Implementation::arrayGrowBy<T, Allocator>(array, 1);
    Implementation::construct(*it, Utility::forward<Args>(args)...);
    return *it;
}

template<class T, class Allocator> ArrayView<T> arrayAppend(Array<T>& array, Corrade::NoInitT, const std::size_t count) {
    T* const it = Implementation::arrayGrowBy<T, Allocator>(array, count);
    return {it, count};
}

namespace Implementation {

template<class T, typename std::enable_if<
    #ifdef CORRADE_NO_STD_IS_TRIVIALLY_TRAITS
    __has_trivial_copy(T) && __has_trivial_destructor(T)
    #else
    std::is_trivially_copyable<T>::value
    #endif
, int>::type = 0> inline void arrayShiftForward(T* const src, T* const dst, const std::size_t count) {
    if(count) std::memmove(dst, src, count*sizeof(T));
}

template<class T, typename std::enable_if<
    #ifdef CORRADE_NO_STD_IS_TRIVIALLY_TRAITS
    !__has_trivial_copy(T) || !__has_trivial_destructor(T)
    #else
    !std::is_trivially_copyable<T>::value
    #endif
, int>::type = 0> inline void arrayShiftForward(T* const src, T* const dst, const std::size_t count) {
    static_assert(std::is_nothrow_move_constructible<T>::value && std::is_nothrow_move_assignable<T>::value,
        "nothrow move-constructible and move-assignable type is required");

    const std::size_t nonOverlappingCount = src + count < dst ? count : dst - src;

    for(T *end = src + count - nonOverlappingCount, *constructSrc = src + count, *constructDst = dst + count; constructSrc > end; --constructSrc, --constructDst) {
        #if defined(CORRADE_TARGET_GCC) && !defined(CORRADE_TARGET_CLANG) &&  __GNUC__ < 5
        Implementation::construct(*(constructDst - 1), Utility::move(*(constructSrc - 1)));
        #else
        new(constructDst - 1) T{Utility::move(*(constructSrc - 1))};
        #endif
    }

    for(T *assignSrc = src + count - nonOverlappingCount, *assignDst = dst + count - nonOverlappingCount; assignSrc > src; --assignSrc, --assignDst)
        *(assignDst - 1) = Utility::move(*(assignSrc - 1));

    for(T *destructSrc = src + nonOverlappingCount; destructSrc != src; --destructSrc)
        (destructSrc - 1)->~T();
}

template<class T, class Allocator> T* arrayGrowAtBy(Array<T>& array, const std::size_t index, const std::size_t count) {
    auto& arrayGuts = reinterpret_cast<Implementation::ArrayGuts<T>&>(array);
    CORRADE_DEBUG_ASSERT(index <= arrayGuts.size, "Containers::arrayInsert(): can't insert at index" << index << "into an array of size" << arrayGuts.size, arrayGuts.data);

    if(!count)
        return arrayGuts.data + index;

    const std::size_t desiredCapacity = arrayGuts.size + count;
    std::size_t capacity;
    #ifdef _CORRADE_CONTAINERS_SANITIZER_ENABLED
    T* oldMid = nullptr;
    #endif
    bool needsShiftForward = false;
    if(arrayGuts.deleter != Allocator::deleter) {
        capacity = Allocator::grow(nullptr, desiredCapacity);
        T* const newArray = Allocator::allocate(capacity);
        arrayMoveConstruct<T>(arrayGuts.data, newArray, index);
        arrayMoveConstruct<T>(arrayGuts.data + index, newArray + index + count, arrayGuts.size - index);
        array = Array<T>{newArray, arrayGuts.size, Allocator::deleter};

    } else {
        capacity = Allocator::capacity(arrayGuts.data);
        if(arrayGuts.size + count > capacity) {
            capacity = Allocator::grow(arrayGuts.data, desiredCapacity);
            Allocator::reallocate(arrayGuts.data, arrayGuts.size, capacity);
        } else {
            #ifdef _CORRADE_CONTAINERS_SANITIZER_ENABLED
            oldMid = arrayGuts.data + arrayGuts.size;
            #endif
        }

        needsShiftForward = true;
    }

    T* const it = arrayGuts.data + index;
    #ifdef _CORRADE_CONTAINERS_SANITIZER_ENABLED
    __sanitizer_annotate_contiguous_container(
        Allocator::base(arrayGuts.data),
        arrayGuts.data + capacity,
        oldMid ? oldMid : arrayGuts.data + capacity,
        arrayGuts.data + arrayGuts.size + count);
    #endif

    if(needsShiftForward)
        arrayShiftForward(arrayGuts.data + index, arrayGuts.data + index + count, arrayGuts.size - index);

    arrayGuts.size += count;
    return it;
}

}

template<class T, class Allocator> inline T& arrayInsert(Array<T>& array, std::size_t index, const typename std::common_type<T>::type& value) {
    T* const it = Implementation::arrayGrowAtBy<T, Allocator>(array, index, 1);
    #if defined(CORRADE_TARGET_GCC) && !defined(CORRADE_TARGET_CLANG) &&  __GNUC__ < 5
    Implementation::construct(*it, value);
    #else
    new(it) T{value};
    #endif
    return *it;
}

template<class T, class Allocator> inline ArrayView<T> arrayInsert(Array<T>& array, std::size_t index, const typename std::common_type<ArrayView<const T>>::type values) {
    const std::size_t valueCount = values.size();

    T* const it = Implementation::arrayGrowAtBy<T, Allocator>(array, index, valueCount);
    Implementation::arrayCopyConstruct<T>(values.data(), it, valueCount);
    return {it, valueCount};
}

template<class T, class ...Args> inline T& arrayInsert(Array<T>& array, std::size_t index, Corrade::InPlaceInitT, Args&&... args) {
    return arrayInsert<T, ArrayAllocator<T>>(array, index, Corrade::InPlaceInit, Utility::forward<Args>(args)...);
}

template<class T, class Allocator, class ...Args> T& arrayInsert(Array<T>& array, std::size_t index, Corrade::InPlaceInitT, Args&&... args) {
    T* const it = Implementation::arrayGrowAtBy<T, Allocator>(array, index, 1);
    Implementation::construct(*it, Utility::forward<Args>(args)...);
    return *it;
}

template<class T, class Allocator> ArrayView<T> arrayInsert(Array<T>& array, const std::size_t index, Corrade::NoInitT, const std::size_t count) {
    T* const it = Implementation::arrayGrowAtBy<T, Allocator>(array, index, count);
    return {it, count};
}

namespace Implementation {

template<class T, typename std::enable_if<
    #ifdef CORRADE_NO_STD_IS_TRIVIALLY_TRAITS
    __has_trivial_copy(T) && __has_trivial_destructor(T)
    #else
    std::is_trivially_copyable<T>::value
    #endif
, int>::type = 0> inline void arrayShiftBackward(T* const src, T* const dst, const std::size_t moveCount, std::size_t) {
    if(moveCount) std::memmove(dst, src, moveCount*sizeof(T));
}

template<class T, typename std::enable_if<
    #ifdef CORRADE_NO_STD_IS_TRIVIALLY_TRAITS
    !__has_trivial_copy(T) || !__has_trivial_destructor(T)
    #else
    !std::is_trivially_copyable<T>::value
    #endif
, int>::type = 0> inline void arrayShiftBackward(T* const src, T* const dst, const std::size_t moveCount, std::size_t destructCount) {
    static_assert(std::is_nothrow_move_constructible<T>::value && std::is_nothrow_move_assignable<T>::value,
        "nothrow move-constructible and move-assignable type is required");

    for(T *end = src + moveCount, *assignSrc = src, *assignDst = dst; assignSrc != end; ++assignSrc, ++assignDst)
        *assignDst = Utility::move(*assignSrc);

    for(T *end = src + moveCount, *destructSrc = end - destructCount; destructSrc != end; ++destructSrc)
        destructSrc->~T();
}

}

template<class T, class Allocator> void arrayRemove(Array<T>& array, const std::size_t index, const std::size_t count) {
    auto& arrayGuts = reinterpret_cast<Implementation::ArrayGuts<T>&>(array);
    CORRADE_DEBUG_ASSERT(index + count <= arrayGuts.size, "Containers::arrayRemove(): can't remove" << count << "elements at index" << index << "from an array of size" << arrayGuts.size, );

    if(!count) return;

    if(arrayGuts.deleter != Allocator::deleter) {
        T* const newArray = Allocator::allocate(arrayGuts.size - count);
        Implementation::arrayMoveConstruct<T>(arrayGuts.data, newArray, index);
        Implementation::arrayMoveConstruct<T>(arrayGuts.data + index + count, newArray + index, arrayGuts.size - index - count);
        array = Array<T>{newArray, arrayGuts.size - count, Allocator::deleter};

        #ifdef _CORRADE_CONTAINERS_SANITIZER_ENABLED
        __sanitizer_annotate_contiguous_container(
            Allocator::base(arrayGuts.data),
            arrayGuts.data + arrayGuts.size,
            arrayGuts.data + arrayGuts.size,
            arrayGuts.data + arrayGuts.size);
        #endif

    } else {
        Implementation::arrayShiftBackward(arrayGuts.data + index + count, arrayGuts.data + index, arrayGuts.size - index - count, count);
        #ifdef _CORRADE_CONTAINERS_SANITIZER_ENABLED
        __sanitizer_annotate_contiguous_container(
            Allocator::base(arrayGuts.data),
            arrayGuts.data + Allocator::capacity(arrayGuts.data),
            arrayGuts.data + arrayGuts.size,
            arrayGuts.data + arrayGuts.size - count);
        #endif
        arrayGuts.size -= count;
    }
}

template<class T, class Allocator> void arrayRemoveUnordered(Array<T>& array, const std::size_t index, const std::size_t count) {
    auto& arrayGuts = reinterpret_cast<Implementation::ArrayGuts<T>&>(array);
    CORRADE_DEBUG_ASSERT(index + count <= arrayGuts.size, "Containers::arrayRemoveUnordered(): can't remove" << count << "elements at index" << index << "from an array of size" << arrayGuts.size, );

    if(!count) return;

    if(arrayGuts.deleter != Allocator::deleter) {
        T* const newArray = Allocator::allocate(arrayGuts.size - count);
        Implementation::arrayMoveConstruct<T>(arrayGuts.data, newArray, index);
        Implementation::arrayMoveConstruct<T>(arrayGuts.data + index + count, newArray + index, arrayGuts.size - index - count);
        array = Array<T>{newArray, arrayGuts.size - count, Allocator::deleter};

        #ifdef _CORRADE_CONTAINERS_SANITIZER_ENABLED
        __sanitizer_annotate_contiguous_container(
            Allocator::base(arrayGuts.data),
            arrayGuts.data + arrayGuts.size,
            arrayGuts.data + arrayGuts.size,
            arrayGuts.data + arrayGuts.size);
        #endif

    } else {
        const std::size_t moveCount = Utility::min(count, arrayGuts.size - count - index);
        Implementation::arrayShiftBackward(arrayGuts.data + arrayGuts.size - moveCount, arrayGuts.data + index, moveCount, count);
        #ifdef _CORRADE_CONTAINERS_SANITIZER_ENABLED
        __sanitizer_annotate_contiguous_container(
            Allocator::base(arrayGuts.data),
            arrayGuts.data + Allocator::capacity(arrayGuts.data),
            arrayGuts.data + arrayGuts.size,
            arrayGuts.data + arrayGuts.size - count);
        #endif
        arrayGuts.size -= count;
    }
}

template<class T, class Allocator> void arrayRemoveSuffix(Array<T>& array, const std::size_t count) {
    auto& arrayGuts = reinterpret_cast<Implementation::ArrayGuts<T>&>(array);
    CORRADE_DEBUG_ASSERT(count <= arrayGuts.size, "Containers::arrayRemoveSuffix(): can't remove" << count << "elements from an array of size" << arrayGuts.size, );

    if(!count) return;

    if(arrayGuts.deleter != Allocator::deleter) {
        T* const newArray = Allocator::allocate(arrayGuts.size - count);
        Implementation::arrayMoveConstruct<T>(arrayGuts.data, newArray, arrayGuts.size - count);
        array = Array<T>{newArray, arrayGuts.size - count, Allocator::deleter};

        #ifdef _CORRADE_CONTAINERS_SANITIZER_ENABLED
        __sanitizer_annotate_contiguous_container(
            Allocator::base(arrayGuts.data),
            arrayGuts.data + arrayGuts.size,
            arrayGuts.data + arrayGuts.size,
            arrayGuts.data + arrayGuts.size);
        #endif

    } else {
        Implementation::arrayDestruct<T>(arrayGuts.data + arrayGuts.size - count, arrayGuts.data + arrayGuts.size);
        #ifdef _CORRADE_CONTAINERS_SANITIZER_ENABLED
        __sanitizer_annotate_contiguous_container(
            Allocator::base(arrayGuts.data),
            arrayGuts.data + Allocator::capacity(arrayGuts.data),
            arrayGuts.data + arrayGuts.size,
            arrayGuts.data + arrayGuts.size - count);
        #endif
        arrayGuts.size -= count;
    }
}

template<class T, class Allocator> void arrayShrink(Array<T>& array, Corrade::NoInitT) {
    auto& arrayGuts = reinterpret_cast<Implementation::ArrayGuts<T>&>(array);

    if(arrayGuts.deleter != Allocator::deleter)
        return;

    Array<T> newArray{Corrade::NoInit, arrayGuts.size};
    Implementation::arrayMoveConstruct<T>(arrayGuts.data, newArray, arrayGuts.size);
    array = Utility::move(newArray);

}

template<class T, class Allocator> void arrayShrink(Array<T>& array, Corrade::DefaultInitT) {
    auto& arrayGuts = reinterpret_cast<Implementation::ArrayGuts<T>&>(array);

    if(arrayGuts.deleter != Allocator::deleter)
        return;

    Array<T> newArray{Corrade::DefaultInit, arrayGuts.size};
    Implementation::arrayMoveAssign<T>(arrayGuts.data, newArray, arrayGuts.size);
    array = Utility::move(newArray);

}

}}

#ifdef _CORRADE_CONTAINERS_SANITIZER_ENABLED
#undef _CORRADE_CONTAINERS_SANITIZER_ENABLED
#endif

#endif
