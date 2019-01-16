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
#include <new>
#include <type_traits>
#include <utility>

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
#ifndef CORRADE_ASSERT
#ifdef NDEBUG
#define CORRADE_ASSERT(condition, message, returnValue) do {} while(0)
#else
#define CORRADE_ASSERT(condition, message, returnValue) assert(condition)
#endif
#endif
#ifndef Corrade_Containers_Optional_h
#define Corrade_Containers_Optional_h

namespace Corrade { namespace Containers {

struct NullOptT {
    struct Init{};
    constexpr explicit NullOptT(Init) {}
};

constexpr NullOptT NullOpt{NullOptT::Init{}};

template<class T> class Optional {
    public:
        /*implicit*/ Optional(NullOptT = NullOpt) noexcept: _set{false} {}

        /*implicit*/ Optional(const T& value) noexcept(std::is_nothrow_copy_assignable<T>::value): _set{true} {
            new(&_value.v) T{value};
        }

        /*implicit*/ Optional(T&& value) noexcept(std::is_nothrow_move_assignable<T>::value): _set{true} {
            new(&_value.v) T{std::move(value)};
        }

        template<class ...Args> /*implicit*/ Optional(InPlaceInitT, Args&&... args) noexcept(std::is_nothrow_constructible<T, Args&&...>::value): _set{true} {
            new(&_value.v) T{std::forward<Args>(args)...};
        }

        Optional(const Optional<T>& other) noexcept(std::is_nothrow_copy_constructible<T>::value);

        Optional(Optional<T>&& other) noexcept(std::is_nothrow_move_constructible<T>::value);

        Optional<T>& operator=(const Optional<T>& other) noexcept(std::is_nothrow_copy_assignable<T>::value);

        Optional<T>& operator=(Optional<T>&& other) noexcept(std::is_nothrow_move_assignable<T>::value);

        Optional<T>& operator=(NullOptT) noexcept;

        ~Optional() { if(_set) _value.v.~T(); }

        explicit operator bool() const { return _set; }

        bool operator==(const Optional<T>& other) const {
            return (!_set && !other._set) || (_set && other._set && _value.v == other._value.v);
        }

        bool operator!=(const Optional<T>& other) const { return !operator==(other); }

        bool operator==(NullOptT) const { return !_set; }

        bool operator!=(NullOptT) const { return _set; }

        bool operator==(const T& other) const {
            return _set ? _value.v == other : false;
        }

        bool operator!=(const T& other) const { return !operator==(other); }

        T* operator->() {
            CORRADE_ASSERT(_set, "Containers::Optional: the optional is empty", &_value.v);
            return &_value.v;
        }

        const T* operator->() const {
            CORRADE_ASSERT(_set, "Containers::Optional: the optional is empty", &_value.v);
            return &_value.v;
        }

        T& operator*() {
            CORRADE_ASSERT(_set, "Containers::Optional: the optional is empty", _value.v);
            return _value.v;
        }

        const T& operator*() const {
            CORRADE_ASSERT(_set, "Containers::Optional: the optional is empty", _value.v);
            return _value.v;
        }

        template<class ...Args> T& emplace(Args&&... args);

    private:
        union Storage {
            constexpr Storage() noexcept: _{} {}
            ~Storage() {}

            T v;
            char _;
        } _value;
        bool _set;
};

template<class T> bool operator==(NullOptT, const Optional<T>& b) { return b == NullOpt; }

template<class T> bool operator!=(NullOptT, const Optional<T>& b) { return b != NullOpt; }

template<class T> bool operator==(const T& a, const Optional<T>& b) { return b == a; }

template<class T> bool operator!=(const T& a, const Optional<T>& b) { return b != a; }

template<class T> inline Optional<typename std::decay<T>::type> optional(T&& value) {
    return Optional<typename std::decay<T>::type>{std::forward<T>(value)};
}

template<class T, class ...Args> inline Optional<T> optional(Args&&... args) {
    return Optional<T>{InPlaceInit, std::forward<Args>(args)...};
}

template<class T> Optional<T>::Optional(const Optional<T>& other) noexcept(std::is_nothrow_copy_constructible<T>::value): _set(other._set) {
    if(_set) new(&_value.v) T{other._value.v};
}

template<class T> Optional<T>::Optional(Optional<T>&& other) noexcept(std::is_nothrow_move_constructible<T>::value): _set(other._set) {
    if(_set) new(&_value.v) T{std::move(other._value.v)};
}

template<class T> Optional<T>& Optional<T>::operator=(const Optional<T>& other) noexcept(std::is_nothrow_copy_assignable<T>::value) {
    if(_set) _value.v.~T();
    if((_set = other._set)) new(&_value.v) T{other._value.v};
    return *this;
}

template<class T> Optional<T>& Optional<T>::operator=(Optional<T>&& other) noexcept(std::is_nothrow_move_assignable<T>::value) {
    if(_set && other._set) {
        using std::swap;
        swap(other._value.v, _value.v);
    } else {
        if(_set) _value.v.~T();
        if((_set = other._set)) new(&_value.v) T{std::move(other._value.v)};
    }
    return *this;
}

template<class T> Optional<T>& Optional<T>::operator=(NullOptT) noexcept {
    if(_set) _value.v.~T();
    _set = false;
    return *this;
}

template<class T> template<class ...Args> T& Optional<T>::emplace(Args&&... args) {
    if(_set) _value.v.~T();
    _set = true;
    new(&_value.v) T{std::forward<Args>(args)...};
    return _value.v;
}

}}

#endif
