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
    template<class> struct ReferenceConverter;
}

template<class T> class Reference {
    public:
        constexpr /*implicit*/ Reference(T& reference) noexcept: _reference{&reference} {}

        template<class U, class = decltype(Implementation::ReferenceConverter<U>::from(std::declval<U>()))> constexpr /*implicit*/ Reference(U other) noexcept: Reference{Implementation::ReferenceConverter<U>::from(other)} {}

        Reference(T&&) = delete;

        template<class U, class = typename std::enable_if<std::is_base_of<T, U>::value>::type> constexpr /*implicit*/ Reference(Reference<U> other) noexcept: _reference{&*other} {}

        template<class U, class = decltype(Implementation::ReferenceConverter<U>::to(std::declval<Reference<T>>()))> constexpr /*implicit*/ operator U() const {
            return Implementation::ReferenceConverter<U>::to(*this);
        }

        constexpr /*implicit*/ operator T&() const { return *_reference; }
        constexpr /*implicit*/ operator Reference<const T>() const { return *_reference; } /**< @overload */

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
