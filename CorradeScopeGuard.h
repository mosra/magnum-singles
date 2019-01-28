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

    v2018.10-232-ge927d7f3 (2019-01-28)
    -   Initial release

    Generated from Corrade v2018.10-232-ge927d7f3 (2019-01-28), 108 / 26 LoC
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

#if defined(_MSC_VER) && _MSC_VER <= 1900
#define CORRADE_MSVC2015_COMPATIBILITY
#endif

#ifndef Corrade_Containers_ScopeGuard_h
#define Corrade_Containers_ScopeGuard_h

namespace Corrade { namespace Containers {

class ScopeGuard {
    public:
        template<class T, class Deleter> explicit ScopeGuard(T handle, Deleter deleter);

        #ifdef CORRADE_MSVC2015_COMPATIBILITY
        template<class T, class U> explicit ScopeGuard(T handle, U(*deleter)(T));
        #endif

        ScopeGuard(const ScopeGuard&) = delete;

        ScopeGuard(ScopeGuard&&) = delete;

        ScopeGuard& operator=(const ScopeGuard&) = delete;

        ScopeGuard& operator=(ScopeGuard&&) = delete;

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

#ifdef CORRADE_MSVC2015_COMPATIBILITY
template<class T, class U> ScopeGuard::ScopeGuard(T handle, U(*deleter)(T)): _deleter{reinterpret_cast<void(*)()>(deleter)}, _handle{reinterpret_cast<void*>(handle)} {
    static_assert(sizeof(T) <= sizeof(void*), "handle too big to store");
    _deleterWrapper = [](void(**deleter)(), void** handle) {
        (*reinterpret_cast<U(**)(T)>(deleter))(*reinterpret_cast<T*>(handle));
    };
}
#endif

}}

#endif
