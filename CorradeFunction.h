/*
    Corrade::Containers::Function
        — a lightweight alternative to std::function

    https://doc.magnum.graphics/corrade/classCorrade_1_1Containers_1_1Function_3_01R_07Args_8_8_8_08_4.html

    This is a single-header library generated from the Corrade project. With
    the goal being easy integration, it's deliberately free of all comments
    to keep the file size small. More info, detailed changelogs and docs here:

    -   Project homepage — https://magnum.graphics/corrade/
    -   Documentation — https://doc.magnum.graphics/corrade/
    -   GitHub project page — https://github.com/mosra/corrade
    -   GitHub Singles repository — https://github.com/mosra/magnum-singles

    v2020.06-1631-g9001f (2024-05-17)
    -   Initial release

    Generated from Corrade v2020.06-1631-g9001f (2024-05-17), 511 / 1919 LoC
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
#include <new>
#include <type_traits>
#if !defined(CORRADE_CONSTEXPR_ASSERT) && !defined(NDEBUG)
#include <cassert>
#endif

#ifdef __GNUC__
#define CORRADE_TARGET_GCC
#endif
#ifdef __clang__
#define CORRADE_TARGET_CLANG
#endif
#ifdef __MINGW32__
#define CORRADE_TARGET_MINGW
#endif
#if !defined(__x86_64) && !defined(_M_X64) && !defined(__aarch64__) && !defined(_M_ARM64) && !defined(__powerpc64__) && !defined(__wasm64__)
#define CORRADE_TARGET_32BIT
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

#if defined(CORRADE_TARGET_LIBSTDCXX) && __GNUC__ < 5 && _GLIBCXX_RELEASE < 7
#define CORRADE_NO_STD_IS_TRIVIALLY_TRAITS
#endif

#if defined(_MSC_VER) && _MSC_VER < 1910
#define CORRADE_MSVC2015_COMPATIBILITY
#endif
#if defined(_MSC_VER) && _MSC_VER < 1920
#define CORRADE_MSVC2017_COMPATIBILITY
#endif

#ifdef _WIN32
#define CORRADE_TARGET_WINDOWS
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
#ifndef Corrade_Containers_Function_h
#define Corrade_Containers_Function_h

namespace Corrade { namespace Containers {

struct NoAllocateInitT {
    struct Init {};
    constexpr explicit NoAllocateInitT(Init) {}
};

constexpr NoAllocateInitT NoAllocateInit{NoAllocateInitT::Init{}};

namespace Implementation {

enum: std::size_t { FunctionPointerSize =
    #if !defined(CORRADE_TARGET_WINDOWS) || defined(CORRADE_TARGET_MINGW)
    2*sizeof(void*)/sizeof(std::size_t)
    #else
    #ifdef CORRADE_TARGET_32BIT
    12/sizeof(std::size_t)
    #else
    16/sizeof(std::size_t)
    #endif
    #endif
};

}

class FunctionData {
    public:
        /*implicit*/ FunctionData(std::nullptr_t = nullptr) noexcept: _storage{}, _call{nullptr} {}

        FunctionData(const FunctionData&) = delete;

        FunctionData(FunctionData&&) noexcept;

        ~FunctionData();

        FunctionData& operator=(const FunctionData&) = delete;

        FunctionData& operator=(FunctionData&&) noexcept;

        explicit operator bool() const {
            return _call || _storage.functor.call;
        }

        bool isAllocated() const { return !_call && _storage.functor.call; }

    private:
        template<class> friend class Function;

        union Storage {
            char data[Implementation::FunctionPointerSize*sizeof(std::size_t) + sizeof(void*)];

            void(*function)();

            struct {
                char data[Implementation::FunctionPointerSize*sizeof(std::size_t)];
                void* instance;
            } member;

            struct {
                void* data;
                void(*free)(Storage&);
                void(*call)();
            } functor;
        };

        constexpr explicit FunctionData(const Storage& storage, void(*call)()):
            #if defined(CORRADE_TARGET_GCC) && !defined(CORRADE_TARGET_CLANG) && __GNUC__ < 5
            _storage(storage),
            #else
            _storage{storage},
            #endif
            _call{call} {}

        Storage _storage;
        void(*_call)();
};

namespace Implementation {

template<class T, class Signature, class = void> struct IsFunctor;
template<class T, class R, class ...Args, class U> struct IsFunctor<T, R(Args...), U> {
    enum: bool { value = false };
};
#if !defined(CORRADE_MSVC2017_COMPATIBILITY) || defined(CORRADE_MSVC2015_COMPATIBILITY)
template<class Class, class R, class ...Args> void functorSignature(R(Class::*)(Args...)) {}
template<class Class, class R, class ...Args> void functorSignature(R(Class::*)(Args...) &) {}
template<class Class, class R, class ...Args> void functorSignature(R(Class::*)(Args...) const) {}
template<class Class, class R, class ...Args> void functorSignature(R(Class::*)(Args...) const &) {}
template<class T, class R, class ...Args> struct IsFunctor<T, R(Args...), decltype(functorSignature<T, R, Args...>(&T::operator()))> {
    enum: bool { value = !std::is_convertible<T, R(*)(Args...)>::value };
};
#else
template<class> struct FunctorSignature;
template<class C, class R,  class ...Args> struct FunctorSignature<R(C::*)(Args...)> {
    static void match(R(C::*)(Args...)) {}
    static void match(R(C::*)(Args...) &) {}
    static void match(R(C::*)(Args...) const) {}
    static void match(R(C::*)(Args...) const &) {}
};
template<class T, class R, class ...Args> struct IsFunctor<T, R(Args...), decltype(FunctorSignature<R(T::*)(Args...)>::match(&T::operator()))> {
    enum: bool { value = !std::is_convertible<T, R(*)(Args...)>::value };
};
#endif

}

template<class> class Function;

template<class R, class ...Args> class Function<R(Args...)>: public FunctionData {
    public:
        typedef R(Type)(Args...);

        /*implicit*/ Function(std::nullptr_t = nullptr) noexcept: FunctionData{nullptr} {}

        /*implicit*/ Function(R(*f)(Args...)) noexcept;

        template<class Instance, class Class> /*implicit*/ Function(Instance& instance, R(Class::*f)(Args...)) noexcept;
        template<class Instance, class Class> /*implicit*/ Function(Instance& instance, R(Class::*f)(Args...) &) noexcept;
        template<class Instance, class Class> /*implicit*/ Function(Instance& instance, R(Class::*f)(Args...) const) noexcept;
        template<class Instance, class Class> /*implicit*/ Function(Instance& instance, R(Class::*f)(Args...) const &) noexcept;

        template<class Instance> /*implicit*/ Function(Instance&, std::nullptr_t) noexcept: Function{nullptr} {}

        template<class F
            , typename std::enable_if<std::is_convertible<typename std::decay<F>::type, R(*)(Args...)>::value || Implementation::IsFunctor<typename std::decay<F>::type, R(Args...)>::value, int>::type = 0
        > /*implicit*/ Function(F&& f) noexcept(sizeof(typename std::decay<F>::type) <= sizeof(Storage) &&
            #ifndef CORRADE_NO_STD_IS_TRIVIALLY_TRAITS
            std::is_trivially_copyable<typename std::decay<F>::type>::value
            #else
            __has_trivial_copy(typename std::decay<F>::type) && __has_trivial_destructor(typename std::decay<F>::type)
            #endif
        ): Function{nullptr, Utility::forward<F>(f)} {}

        template<class F
            , typename std::enable_if<Implementation::IsFunctor<typename std::decay<F>::type, R(Args...)>::value, int>::type = 0
        > explicit Function(NoAllocateInitT, F&& f) noexcept;

        R operator()(Args... args);

    private:
        template<class F, typename std::enable_if<std::is_convertible<typename std::decay<F>::type, R(*)(Args...)>::value, int>::type = 0> explicit Function(std::nullptr_t, F&& f) noexcept: Function{static_cast<R(*)(Args...)>(f)} {}

        template<class F, typename std::enable_if<!std::is_convertible<typename std::decay<F>::type, R(*)(Args...)>::value && sizeof(typename std::decay<F>::type) <= sizeof(Storage) &&
            #ifndef CORRADE_NO_STD_IS_TRIVIALLY_TRAITS
            std::is_trivially_copyable<typename std::decay<F>::type>::value
            #else
            __has_trivial_copy(typename std::decay<F>::type) && __has_trivial_destructor(typename std::decay<F>::type)
            #endif
        , int>::type = 0> explicit Function(std::nullptr_t, F&& f) noexcept:
            Function{NoAllocateInit, f} {}

        template<class F, typename std::enable_if<
            #ifdef CORRADE_MSVC2017_COMPATIBILITY
            !std::is_convertible<typename std::decay<F>::type, R(*)(Args...)>::value &&
            #endif
            (sizeof(typename std::decay<F>::type) > sizeof(Storage) ||
            #ifndef CORRADE_NO_STD_IS_TRIVIALLY_TRAITS
            !std::is_trivially_copyable<typename std::decay<F>::type>::value
            #else
            !__has_trivial_copy(typename std::decay<F>::type) || !__has_trivial_destructor(typename std::decay<F>::type)
            #endif
        ), int>::type = 0> explicit Function(std::nullptr_t, F&& f);
};

inline FunctionData::FunctionData(FunctionData&& other) noexcept:
    #if defined(CORRADE_TARGET_GCC) && !defined(CORRADE_TARGET_CLANG) && __GNUC__ < 5
    _storage(other._storage),
    #else
    _storage{other._storage},
    #endif
    _call{other._call}
{
    if(!_call && _storage.functor.call)
        other._storage.functor.call = nullptr;
}

inline FunctionData:: ~FunctionData() {
    if(!_call && _storage.functor.call)
        _storage.functor.free(_storage);
}

inline FunctionData& FunctionData::operator=(FunctionData&& other) noexcept {
    using Utility::swap;
    swap(other._storage, _storage);
    swap(other._call, _call);
    return *this;
}

template<class R, class ...Args> inline R Function<R(Args...)>::operator()(Args... args) {
    void(*const call)() = _call ? _call : _storage.functor.call;
    return CORRADE_CONSTEXPR_DEBUG_ASSERT(call, "Containers::Function: the function is null"),
        reinterpret_cast<R(*)(Storage&, Args...)>(call)(_storage, Utility::forward<Args>(args)...);
}

template<class R, class ...Args> Function<R(Args...)>::Function(R(*f)(Args...)) noexcept {
    _storage.function = reinterpret_cast<void(*)()>(f);
    _call = !f ? nullptr : reinterpret_cast<void(*)()>(
        #ifndef CORRADE_MSVC2015_COMPATIBILITY
        +
        #else
        static_cast<R(*)(Storage&, Args...)>
        #endif
    ([](Storage& storage, Args... args) -> R {
        return reinterpret_cast<R(*)(Args...)>(storage.function)(Utility::forward<Args>(args)...);
    }));
}

template<class R, class ...Args> template<class F, typename std::enable_if<Implementation::IsFunctor<typename std::decay<F>::type, R(Args...)>::value, int>::type> Function<R(Args...)>::Function(NoAllocateInitT, F&& f) noexcept {
    static_assert(sizeof(typename std::decay<F>::type) <= sizeof(FunctionData::Storage) &&
        #ifndef CORRADE_NO_STD_IS_TRIVIALLY_TRAITS
        std::is_trivially_copyable<typename std::decay<F>::type>::value
        #else
        __has_trivial_copy(typename std::decay<F>::type) && __has_trivial_destructor(typename std::decay<F>::type)
        #endif
    , "functor too large or non-trivial to be stored without allocation");

    #if defined(CORRADE_TARGET_GCC) && !defined(CORRADE_TARGET_CLANG) && __GNUC__ < 5
    new(&_storage.data) typename std::decay<F>::type(f);
    #else
    new(&_storage.data) typename std::decay<F>::type{Utility::move(f)};
    #endif
    _call = reinterpret_cast<void(*)()>(
        #ifndef CORRADE_MSVC2015_COMPATIBILITY
        +
        #else
        static_cast<R(*)(Storage&, Args...)>
        #endif
    ([](Storage& storage, Args... args) -> R {
        return reinterpret_cast<F&>(storage.data)(Utility::forward<Args>(args)...);
    }));
}

template<class R, class ...Args> template<class F, typename std::enable_if<
    #ifdef CORRADE_MSVC2017_COMPATIBILITY
    !std::is_convertible<typename std::decay<F>::type, R(*)(Args...)>::value &&
    #endif
    (sizeof(typename std::decay<F>::type) > sizeof(FunctionData::Storage) ||
    #ifndef CORRADE_NO_STD_IS_TRIVIALLY_TRAITS
    !std::is_trivially_copyable<typename std::decay<F>::type>::value
    #else
    !__has_trivial_copy(typename std::decay<F>::type) || !__has_trivial_destructor(typename std::decay<F>::type)
    #endif
), int>::type> Function<R(Args...)>::Function(std::nullptr_t, F&& f) {
    #if defined(CORRADE_TARGET_GCC) && !defined(CORRADE_TARGET_CLANG) && __GNUC__ < 5
    reinterpret_cast<typename std::decay<F>::type*&>(_storage.functor.data) = new typename std::decay<F>::type(Utility::forward<F>(f));
    #else
    reinterpret_cast<typename std::decay<F>::type*&>(_storage.functor.data) = new typename std::decay<F>::type{Utility::forward<F>(f)};
    #endif
    _storage.functor.free = [](Storage& storage) {
        delete reinterpret_cast<typename std::decay<F>::type*>(storage.functor.data);
    };
    _storage.functor.call = reinterpret_cast<void(*)()>(
        #ifndef CORRADE_MSVC2015_COMPATIBILITY
        +
        #else
        static_cast<R(*)(Storage&, Args...)>
        #endif
    ([](Storage& storage, Args... args) -> R {
        return (*reinterpret_cast<typename std::decay<F>::type*>(storage.functor.data))(Utility::forward<Args>(args)...);
    }));
    _call = nullptr;
}

template<class R, class ...Args> template<class Instance, class Class> Function<R(Args...)>::Function(Instance& instance, R(Class::*f)(Args...)) noexcept {
    static_assert(sizeof(f) <= sizeof(_storage.member.data),
        "size of member function pointer is incorrectly assumed to be smaller");
    if(!f) return;

    _storage.member.instance = &instance;
    reinterpret_cast<R(Class::*&)(Args...)>(_storage.member.data) = f;
    _call = reinterpret_cast<void(*)()>(
        #ifndef CORRADE_MSVC2015_COMPATIBILITY
        +
        #else
        static_cast<R(*)(Storage&, Args...)>
        #endif
    ([](Storage& storage, Args... args) -> R {
        return (static_cast<Instance*>(storage.member.instance)->*reinterpret_cast<R(Class::*&)(Args...)>(storage.member.data))(Utility::forward<Args>(args)...);
    }));
}
template<class R, class ...Args> template<class Instance, class Class> Function<R(Args...)>::Function(Instance& instance, R(Class::*f)(Args...) &) noexcept {
    static_assert(sizeof(f) <= sizeof(_storage.member.data),
        "size of member function pointer is incorrectly assumed to be smaller");
    if(!f) return;

    _storage.member.instance = &instance;
    reinterpret_cast<R(Class::*&)(Args...) &>(_storage.member.data) = f;
    _call = reinterpret_cast<void(*)()>(
        #ifndef CORRADE_MSVC2015_COMPATIBILITY
        +
        #else
        static_cast<R(*)(Storage&, Args...)>
        #endif
    ([](Storage& storage, Args... args) -> R {
        return (static_cast<Instance*>(storage.member.instance)->*reinterpret_cast<R(Class::*&)(Args...) &>(storage.member.data))(Utility::forward<Args>(args)...);
    }));
}
template<class R, class ...Args> template<class Instance, class Class> Function<R(Args...)>::Function(Instance& instance, R(Class::*f)(Args...) const) noexcept {
    static_assert(sizeof(f) <= sizeof(_storage.member.data),
        "size of member function pointer is incorrectly assumed to be smaller");
    if(!f) return;

    _storage.member.instance = &instance;
    reinterpret_cast<R(Class::*&)(Args...) const>(_storage.member.data) = f;
    _call = reinterpret_cast<void(*)()>(
        #ifndef CORRADE_MSVC2015_COMPATIBILITY
        +
        #else
        static_cast<R(*)(Storage&, Args...)>
        #endif
    ([](Storage& storage, Args... args) -> R {
        return (static_cast<Instance*>(storage.member.instance)->*reinterpret_cast<R(Class::*&)(Args...) const>(storage.member.data))(Utility::forward<Args>(args)...);
    }));
}
template<class R, class ...Args> template<class Instance, class Class> Function<R(Args...)>::Function(Instance& instance, R(Class::*f)(Args...) const &) noexcept {
    static_assert(sizeof(f) <= sizeof(_storage.member.data),
        "size of member function pointer is incorrectly assumed to be smaller");
    if(!f) return;

    _storage.member.instance = &instance;
    reinterpret_cast<R(Class::*&)(Args...) const &>(_storage.member.data) = f;
    _call = reinterpret_cast<void(*)()>(
        #ifndef CORRADE_MSVC2015_COMPATIBILITY
        +
        #else
        static_cast<R(*)(Storage&, Args...)>
        #endif
    ([](Storage& storage, Args... args) -> R {
        return (static_cast<Instance*>(storage.member.instance)->*reinterpret_cast<R(Class::*&)(Args...) const &>(storage.member.data))(Utility::forward<Args>(args)...);
    }));
}

}}

#endif
