/*
    Corrade::Containers::Reference
        — a lightweight alternative to std::reference_wrapper

    https://doc.magnum.graphics/corrade/classCorrade_1_1Containers_1_1Reference.html

    This is a single-header library generated from the Corrade project. With
    the goal being easy integration, it's deliberately free of all comments
    to keep the file size small. More info, detailed changelogs and docs here:

    -   Project homepage — https://magnum.graphics/corrade/
    -   Documentation — https://doc.magnum.graphics/corrade/
    -   GitHub project page — https://github.com/mosra/corrade
    -   GitHub Singles repository — https://github.com/mosra/magnum-singles

    v2018.10-232-ge927d7f3 (2019-01-28)
    -   Stricter matching for external representation conversion
    -   Fixed STL compatibility to not recurse infinitely
    v2018.10-183-g4eb1adc0 (2019-01-23)
    -   Initial release

    Generated from Corrade v2018.10-232-ge927d7f3 (2019-01-28), 115 / 1639 LoC
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

#include <type_traits>

#ifndef Corrade_Containers_Reference_h
#define Corrade_Containers_Reference_h

namespace Corrade { namespace Containers {

namespace Implementation {
    template<class, class> struct ReferenceConverter;
}

template<class T> class Reference {
    public:
        constexpr /*implicit*/ Reference(T& reference) noexcept: _reference{&reference} {}

        template<class U, class = decltype(Implementation::ReferenceConverter<T, U>::from(std::declval<U>()))> constexpr /*implicit*/ Reference(U other) noexcept: Reference{Implementation::ReferenceConverter<T, U>::from(other)} {}

        Reference(T&&) = delete;

        template<class U, class = typename std::enable_if<std::is_base_of<T, U>::value>::type> constexpr /*implicit*/ Reference(Reference<U> other) noexcept: _reference{&*other} {}

        template<class U, class = decltype(Implementation::ReferenceConverter<T, U>::to(std::declval<Reference<T>>()))> constexpr /*implicit*/ operator U() const {
            return Implementation::ReferenceConverter<T, U>::to(*this);
        }

        constexpr /*implicit*/ operator T&() const { return *_reference; }
        constexpr /*implicit*/ operator Reference<const T>() const { return *_reference; }

        constexpr T& get() const { return *_reference; }

        constexpr T* operator->() const {
            return _reference;
        }

        constexpr T& operator*() const {
            return *_reference;
        }

    private:
        T* _reference;
};

}}

#endif
#ifdef CORRADE_POINTER_STL_COMPATIBILITY
#include <functional>
#ifndef Corrade_Containers_ReferenceStl_h
#define Corrade_Containers_ReferenceStl_h

namespace Corrade { namespace Containers { namespace Implementation {

template<class T> struct ReferenceConverter<T, std::reference_wrapper<T>> {
    static Reference<T> from(std::reference_wrapper<T> other) {
        return other.get();
    }

    static std::reference_wrapper<T> to(Reference<T> other) {
        return other.get();
    }
};

}}}

#endif
#endif
