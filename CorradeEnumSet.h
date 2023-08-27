/*
    Corrade::Containers::EnumSet
        — a type-safe set of bits

    https://doc.magnum.graphics/corrade/classCorrade_1_1Containers_1_1EnumSet.html

    This is a single-header library generated from the Corrade project. With
    the goal being easy integration, it's deliberately free of all comments
    to keep the file size small. More info, detailed changelogs and docs here:

    -   Project homepage — https://magnum.graphics/corrade/
    -   Documentation — https://doc.magnum.graphics/corrade/
    -   GitHub project page — https://github.com/mosra/corrade
    -   GitHub Singles repository — https://github.com/mosra/magnum-singles

    v2020.06-1454-gfc3b7 (2023-08-27)
    -   It's now possible to construct the EnumSet directly from the underlying
        enum's type instead of having to cast to the enum type first
    -   Removed unnecessary function calls for improved debug performace
    v2020.06-1075-gdd71 (2022-10-13)
    -   Initial release

    Generated from Corrade v2020.06-1454-gfc3b7 (2023-08-27), 260 / 1705 LoC
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

#ifndef CorradeEnumSet_h
#define CorradeEnumSet_h
namespace Corrade { namespace Containers {

template<class T, typename std::underlying_type<T>::type fullValue = typename std::underlying_type<T>::type(~0)> class EnumSet;

}}
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
#ifndef Corrade_Containers_EnumSet_h
#define Corrade_Containers_EnumSet_h

namespace Corrade { namespace Containers {

template<class T, typename std::underlying_type<T>::type fullValue>
class EnumSet {
    static_assert(std::is_enum<T>::value, "EnumSet type must be a strongly typed enum");

    public:
        typedef T Type;

        typedef typename std::underlying_type<T>::type UnderlyingType;

        enum: UnderlyingType {
            FullValue = fullValue
        };

        constexpr /*implicit*/ EnumSet() noexcept: _value{} {}

        constexpr /*implicit*/ EnumSet(T value) noexcept:
            _value(static_cast<UnderlyingType>(value)) {}

        constexpr explicit EnumSet(UnderlyingType value) noexcept: _value{value} {}

        explicit EnumSet(Corrade::NoInitT) {}

        constexpr bool operator==(EnumSet<T, fullValue> other) const {
            return _value == other._value;
        }

        constexpr bool operator!=(EnumSet<T, fullValue> other) const {
            return _value != other._value;
        }

        constexpr bool operator>=(EnumSet<T, fullValue> other) const {
            return (_value & other._value) == other._value;
        }

        constexpr bool operator<=(EnumSet<T, fullValue> other) const {
            return (_value & other._value) == _value;
        }

        constexpr EnumSet<T, fullValue> operator|(EnumSet<T, fullValue> other) const {
            return EnumSet<T, fullValue>(_value | other._value);
        }

        EnumSet<T, fullValue>& operator|=(EnumSet<T, fullValue> other) {
            _value |= other._value;
            return *this;
        }

        constexpr EnumSet<T, fullValue> operator&(EnumSet<T, fullValue> other) const {
            return EnumSet<T, fullValue>(_value & other._value);
        }

        EnumSet<T, fullValue>& operator&=(EnumSet<T, fullValue> other) {
            _value &= other._value;
            return *this;
        }

        constexpr EnumSet<T, fullValue> operator^(EnumSet<T, fullValue> other) const {
            return EnumSet<T, fullValue>(_value ^ other._value);
        }

        EnumSet<T, fullValue>& operator^=(EnumSet<T, fullValue> other) {
            _value ^= other._value;
            return *this;
        }

        constexpr EnumSet<T, fullValue> operator~() const {
            return EnumSet<T, fullValue>(fullValue & ~_value);
        }

        constexpr explicit operator bool() const {
            return _value != 0;
        }

        constexpr explicit operator UnderlyingType() const {
            return _value;
        }

    private:
        UnderlyingType _value;
};

template<class T, class = typename std::enable_if<std::is_enum<T>::value>::type> constexpr typename std::underlying_type<T>::type enumCastUnderlyingType(T value) {
    return typename std::underlying_type<T>::type(value);
}

template<class T, typename std::underlying_type<T>::type fullValue> constexpr typename std::underlying_type<T>::type enumCastUnderlyingType(EnumSet<T, fullValue> value) {
    return typename std::underlying_type<T>::type(value);
}

#define CORRADE_ENUMSET_OPERATORS(class)                                    \
    constexpr bool operator==(class::Type a, class b) {                     \
        return class(a) == b;                                               \
    }                                                                       \
    constexpr bool operator!=(class::Type a, class b) {                     \
        return class(a) != b;                                               \
    }                                                                       \
    constexpr bool operator>=(class::Type a, class b) {                     \
        return class(a) >= b;                                               \
    }                                                                       \
    constexpr bool operator<=(class::Type a, class b) {                     \
        return class(a) <= b;                                               \
    }                                                                       \
    constexpr class operator|(class::Type a, class b) {                     \
        return b | a;                                                       \
    }                                                                       \
    constexpr class operator&(class::Type a, class b) {                     \
        return b & a;                                                       \
    }                                                                       \
    constexpr class operator^(class::Type a, class b) {                     \
        return b ^ a;                                                       \
    }                                                                       \
    constexpr class operator~(class::Type a) {                              \
        return ~class(a);                                                   \
    }

#define CORRADE_ENUMSET_FRIEND_OPERATORS(class)                             \
    friend constexpr bool operator==(typename class::Type a, class b) {     \
        return class(a) == b;                                               \
    }                                                                       \
    friend constexpr bool operator!=(typename class::Type a, class b) {     \
        return class(a) != b;                                               \
    }                                                                       \
    friend constexpr bool operator>=(typename class::Type a, class b) {     \
        return class(a) >= b;                                               \
    }                                                                       \
    friend constexpr bool operator<=(typename class::Type a, class b) {     \
        return class(a) <= b;                                               \
    }                                                                       \
    friend constexpr class operator|(typename class::Type a, class b) {     \
        return b | a;                                                       \
    }                                                                       \
    friend constexpr class operator&(typename class::Type a, class b) {     \
        return b & a;                                                       \
    }                                                                       \
    friend constexpr class operator^(typename class::Type a, class b) {     \
        return b ^ a;                                                       \
    }                                                                       \
    friend constexpr class operator~(typename class::Type a) {              \
        return ~class(a);                                                   \
    }

}}

#endif
