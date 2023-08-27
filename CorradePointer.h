/*
    Corrade::Containers::Pointer
        — a lightweight alternative to std::unique_ptr

    https://doc.magnum.graphics/corrade/classCorrade_1_1Containers_1_1Pointer.html

    This is a single-header library generated from the Corrade project. With
    the goal being easy integration, it's deliberately free of all comments
    to keep the file size small. More info, detailed changelogs and docs here:

    -   Project homepage — https://magnum.graphics/corrade/
    -   Documentation — https://doc.magnum.graphics/corrade/
    -   GitHub project page — https://github.com/mosra/corrade
    -   GitHub Singles repository — https://github.com/mosra/magnum-singles

    v2020.06-1454-gfc3b7 (2023-08-27)
    -   The InPlaceInit tag is moved from Containers to the root namespace
    -   The underlying type is exposed in a new Pointer::Type typedef
    -   Added Pointer::emplace<U>() for emplacing with a derived type
    -   Further workarounds for various compiler-specific issues and standard
        defects when using {}-initialization for aggregate types
    -   Removed dependency on <utility>, resulting in about ~600 preprocessed
        lines less
    v2020.06-0-g61d1b58c (2020-06-27)
    -   Working around various compiler-specific issues and standard defects
        when using {}-initialization for aggregate types
    v2019.01-107-g80d9f347 (2019-03-23)
    -   Including <cassert> only when needed
    v2018.10-232-ge927d7f3 (2019-01-28)
    -   Stricter matching for external representation conversion
    -   Ability to convert from external representation also using pointer()
    v2018.10-183-g4eb1adc0 (2019-01-23)
    -   Initial release

    Generated from Corrade v2020.06-1454-gfc3b7 (2023-08-27), 338 / 1771 LoC
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
#if !defined(CORRADE_ASSERT) && !defined(NDEBUG)
#include <cassert>
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
#ifndef CORRADE_ASSERT
#ifdef NDEBUG
#define CORRADE_ASSERT(condition, message, returnValue) do {} while(false)
#else
#define CORRADE_ASSERT(condition, message, returnValue) assert(condition)
#endif
#endif
#ifndef CORRADE_DEBUG_ASSERT
#define CORRADE_DEBUG_ASSERT(condition, message, returnValue)               \
    CORRADE_ASSERT(condition, message, returnValue)
#endif
#ifndef Corrade_Containers_Pointer_h
#define Corrade_Containers_Pointer_h

namespace Corrade { namespace Containers {

namespace Implementation {
    template<class, class> struct PointerConverter;

    template<class T, class First, class ...Next> inline T* allocate(First&& first, Next&& ...next) {
        return new T{Utility::forward<First>(first), Utility::forward<Next>(next)...};
    }
    template<class T> inline T* allocate() {
        return new T();
    }
    #if defined(CORRADE_TARGET_GCC) && !defined(CORRADE_TARGET_CLANG) && __GNUC__ < 5
    template<class T> inline T* allocate(const T& b) {
        return new T(b);
    }
    template<class T> inline T* allocate(T&& b) {
        return new T(Utility::move(b));
    }
    #endif
}

template<class T> class Pointer {
    static_assert(!std::is_array<T>::value, "use Containers::Array for arrays instead");

    public:
        typedef T Type;

        template<class U, class = typename std::enable_if<std::is_same<std::nullptr_t, U>::value>::type> /*implicit*/ Pointer(U) noexcept: _pointer{} {}

        /*implicit*/ Pointer() noexcept: _pointer{} {}

        explicit Pointer(T* pointer) noexcept: _pointer{pointer} {}

        template<class ...Args> explicit Pointer(Corrade::InPlaceInitT, Args&&... args): _pointer{
            Implementation::allocate<T>(Utility::forward<Args>(args)...)
        } {}

        template<class U, class = typename std::enable_if<std::is_base_of<T, U>::value>::type> /*implicit*/ Pointer(Pointer<U>&& other) noexcept: _pointer{other.release()} {}

        template<class U, class = decltype(Implementation::PointerConverter<T, U>::from(std::declval<U&&>()))> /*implicit*/ Pointer(U&& other) noexcept: Pointer{Implementation::PointerConverter<T, U>::from(Utility::move(other))} {}

        Pointer(const Pointer<T>&) = delete;

        Pointer(Pointer<T>&& other) noexcept: _pointer{other._pointer} {
            other._pointer = nullptr;
        }

        Pointer<T>& operator=(const Pointer<T>&) = delete;

        Pointer<T>& operator=(Pointer<T>&& other) noexcept {
            Utility::swap(_pointer, other._pointer);
            return *this;
        }

        template<class U, class = decltype(Implementation::PointerConverter<T, U>::to(std::declval<Pointer<T>&&>()))> /*implicit*/ operator U() && {
            return Implementation::PointerConverter<T, U>::to(Utility::move(*this));
        }

        bool operator==(std::nullptr_t) const { return !_pointer; }

        bool operator!=(std::nullptr_t) const { return _pointer; }

        ~Pointer() { delete _pointer; }

        explicit operator bool() const { return _pointer; }

        T* get() { return _pointer; }
        const T* get() const { return _pointer; }

        T* operator->() {
            CORRADE_DEBUG_ASSERT(_pointer, "Containers::Pointer: the pointer is null", nullptr);
            return _pointer;
        }

        const T* operator->() const {
            CORRADE_DEBUG_ASSERT(_pointer, "Containers::Pointer: the pointer is null", nullptr);
            return _pointer;
        }

        T& operator*() {
            CORRADE_DEBUG_ASSERT(_pointer, "Containers::Pointer: the pointer is null", *_pointer);
            return *_pointer;
        }

        const T& operator*() const {
            CORRADE_DEBUG_ASSERT(_pointer, "Containers::Pointer: the pointer is null", *_pointer);
            return *_pointer;
        }

        void reset(T* pointer = nullptr) {
            delete _pointer;
            _pointer = pointer;
        }

        template<class ...Args> T& emplace(Args&&... args) {
            delete _pointer;
            _pointer = Implementation::allocate<T>(Utility::forward<Args>(args)...);
            return *_pointer;
        }

        template<class U, class ...Args> U& emplace(Args&&... args) {
            delete _pointer;
            U* const derived = Implementation::allocate<U>(Utility::forward<Args>(args)...);
            _pointer = derived;
            return *derived;
        }

        T* release() {
            T* const out = _pointer;
            _pointer = nullptr;
            return out;
        }

    private:
        T* _pointer;
};

template<class T> bool operator==(std::nullptr_t, const Pointer<T>& b) { return b == nullptr; }

template<class T> bool operator!=(std::nullptr_t, const Pointer<T>& b) { return b != nullptr; }

template<class T> inline Pointer<T> pointer(T* pointer) {
    static_assert(!std::is_constructible<T, T*>::value, "the type is constructible from its own pointer, which is ambiguous -- explicitly use the constructor instead");
    return Pointer<T>{pointer};
}

namespace Implementation {
    template<class> struct DeducedPointerConverter;
}

template<class T> inline auto pointer(T&& other) -> decltype(Implementation::DeducedPointerConverter<T>::from(Utility::move(other))) {
    return Implementation::DeducedPointerConverter<T>::from(Utility::move(other));
}

template<class U, class T> Pointer<U> pointerCast(Pointer<T>&& pointer) {
    return Pointer<U>{static_cast<U*>(pointer.release())};
}

namespace Implementation {
    template<class T, class ...Args> struct IsFirstAPointer: std::false_type {};
    template<class T> struct IsFirstAPointer<T, T*>: std::true_type {};
}

template<class T, class ...Args> inline Pointer<T> pointer(Args&&... args) {
    static_assert(!Implementation::IsFirstAPointer<T, Args...>::value || !std::is_constructible<T, T*>::value, "attempt to construct a type from its own pointer, which is ambiguous --  explicitly use the constructor instead");
    return Pointer<T>{Corrade::InPlaceInit, Utility::forward<Args>(args)...};
}

}}

#endif
#ifdef CORRADE_POINTER_STL_COMPATIBILITY
#include <memory>
#ifndef Corrade_Containers_PointerStl_h
#define Corrade_Containers_PointerStl_h

namespace Corrade { namespace Containers { namespace Implementation {

template<class T> struct PointerConverter<T, std::unique_ptr<T>> {
    static Pointer<T> from(std::unique_ptr<T>&& other) {
        return Pointer<T>{other.release()};
    }

    static std::unique_ptr<T> to(Pointer<T>&& other) {
        return std::unique_ptr<T>{other.release()};
    }
};

template<class T> struct DeducedPointerConverter<std::unique_ptr<T>>: PointerConverter<T, std::unique_ptr<T>> {};

}}}

#endif
#endif
