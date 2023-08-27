/*
    Corrade::Containers::ScopeGuard
        — a lightweight alternative to std::unique_ptr with a custom deleter

    https://doc.magnum.graphics/corrade/classCorrade_1_1Containers_1_1ScopeGuard.html

    This is a single-header library generated from the Corrade project. With
    the goal being easy integration, it's deliberately free of all comments
    to keep the file size small. More info, detailed changelogs and docs here:

    -   Project homepage — https://magnum.graphics/corrade/
    -   Documentation — https://doc.magnum.graphics/corrade/
    -   GitHub project page — https://github.com/mosra/corrade
    -   GitHub Singles repository — https://github.com/mosra/magnum-singles

    v2020.06-1454-gfc3b7 (2023-08-27)
    -   Ability to construct a NoCreate'd ScopeGuard and then move a complete
        instance over it
    v2019.01-41-g39c08d7c (2019-02-18)
    -   Ability to create a handle-less ScopeGuard
    v2018.10-232-ge927d7f3 (2019-01-28)
    -   Initial release

    Generated from Corrade v2020.06-1454-gfc3b7 (2023-08-27), 233 / 1702 LoC
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

#if defined(_MSC_VER) && _MSC_VER < 1910
#define CORRADE_MSVC2015_COMPATIBILITY
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
#ifndef Corrade_Containers_ScopeGuard_h
#define Corrade_Containers_ScopeGuard_h

namespace Corrade { namespace Containers {

class ScopeGuard {
    public:
        template<class T, class Deleter> explicit ScopeGuard(T handle, Deleter deleter);

        template<class Deleter> explicit ScopeGuard(Deleter deleter);

        #ifdef CORRADE_MSVC2015_COMPATIBILITY
        template<class T, class U> explicit ScopeGuard(T handle, U(*deleter)(T));
        template<class U> explicit ScopeGuard(U(*deleter)());
        #endif

        explicit ScopeGuard(Corrade::NoCreateT) noexcept: _deleterWrapper{}, _deleter{}, _handle{} {}

        ScopeGuard(const ScopeGuard&) = delete;

        ScopeGuard(ScopeGuard&& other) noexcept;

        ScopeGuard& operator=(const ScopeGuard&) = delete;

        ScopeGuard& operator=(ScopeGuard&&) noexcept;

        void release() { _deleterWrapper = nullptr; }

        ~ScopeGuard() {
            if(_deleterWrapper) _deleterWrapper(&_deleter, &_handle);
        }

    private:
        void(*_deleterWrapper)(void(**)(), void**);
        void(*_deleter)();
        void* _handle;
};

template<class T, class Deleter> ScopeGuard::ScopeGuard(T handle, Deleter deleter): _deleter{
    #ifndef CORRADE_MSVC2015_COMPATIBILITY
    reinterpret_cast<void(*)()>(+deleter)
    #else
    reinterpret_cast<void(*)()>(static_cast<void(*)(T)>(deleter))
    #endif
}, _handle{reinterpret_cast<void*>(handle)} {
    static_assert(sizeof(T) <= sizeof(void*), "handle too big to store");
    _deleterWrapper = [](void(**deleter)(), void** handle) {
        (*reinterpret_cast<Deleter*>(deleter))(*reinterpret_cast<T*>(handle));
    };
}

template<class Deleter> ScopeGuard::ScopeGuard(Deleter deleter): _deleter{
    #ifndef CORRADE_MSVC2015_COMPATIBILITY
    reinterpret_cast<void(*)()>(+deleter)
    #else
    reinterpret_cast<void(*)()>(static_cast<void(*)()>(deleter))
    #endif
}, _handle{nullptr} {
    _deleterWrapper = [](void(**deleter)(), void**) {
        (*reinterpret_cast<Deleter*>(deleter))();
    };
}

inline ScopeGuard::ScopeGuard(ScopeGuard&& other) noexcept: _deleterWrapper{other._deleterWrapper}, _deleter{other._deleter}, _handle{other._handle} {
    other._deleterWrapper = nullptr;
}

inline ScopeGuard& ScopeGuard::operator=(ScopeGuard&& other) noexcept {
    using Utility::swap;
    swap(other._deleterWrapper, _deleterWrapper);
    swap(other._deleter, _deleter);
    swap(other._handle, _handle);
    return *this;
}

#ifdef CORRADE_MSVC2015_COMPATIBILITY
template<class T, class U> ScopeGuard::ScopeGuard(T handle, U(*deleter)(T)): _deleter{reinterpret_cast<void(*)()>(deleter)}, _handle{reinterpret_cast<void*>(handle)} {
    static_assert(sizeof(T) <= sizeof(void*), "handle too big to store");
    _deleterWrapper = [](void(**deleter)(), void** handle) {
        (*reinterpret_cast<U(**)(T)>(deleter))(*reinterpret_cast<T*>(handle));
    };
}

template<class U> ScopeGuard::ScopeGuard(U(*deleter)()): _deleter{reinterpret_cast<void(*)()>(deleter)}, _handle{nullptr} {
    _deleterWrapper = [](void(**deleter)(), void**) {
        (*reinterpret_cast<U(**)()>(deleter))();
    };
}
#endif

}}

#endif
