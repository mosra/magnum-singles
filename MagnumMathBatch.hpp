/*
    Magnum::Math
        — a graphics-focused vector math library, batch APIs

    https://doc.magnum.graphics/magnum/namespaceMagnum_1_1Math.html

    Depends on ContainersStridedArrayView.h and MagnumMath.hpp.

    This is a single-header library generated from the Magnum project. With the
    goal being easy integration, it's deliberately free of all comments to keep
    the file size small. More info, changelogs and full docs here:

    -   Project homepage — https://magnum.graphics/magnum/
    -   Documentation — https://doc.magnum.graphics/
    -   GitHub project page — https://github.com/mosra/magnum
    -   GitHub Singles repository — https://github.com/mosra/magnum-singles

    The library has a separate non-inline implementation part, enable it *just
    once* like this:

        #define MAGNUM_MATH_BATCH_IMPLEMENTATION
        #include <MagnumMathBatch.hpp>

    If you need the deinlined symbols to be exported from a shared library,
    `#define MAGNUM_EXPORT` as appropriate.

    v2020.06-3290-g454e9 (2025-04-11)
    -   Include guard for the implementation part to prevent double definitions
    -   Cleanup and unification of SFINAE code, it's now done in template args
        as that's simpler for the compiler
    v2020.06-3128-g47b22 (2025-01-07)
    -   Initial release

    Generated from Corrade v2020.06-1890-g77f9f (2025-04-11) and
        Magnum v2020.06-3290-g454e9 (2025-04-11), 1323 / 11405 LoC
*/

/*
    This file is part of Magnum.

    Copyright © 2007, 2008, 2009, 2010, 2011, 2012, 2013, 2014, 2015, 2016,
                2017, 2018, 2019, 2020, 2021, 2022, 2023, 2024, 2025
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

#include <initializer_list>
#include <type_traits>

#include "CorradeStridedArrayView.h"
#include "MagnumMath.hpp"

#ifndef Magnum_Math_FunctionsBatch_h
#define Magnum_Math_FunctionsBatch_h

namespace Magnum { namespace Math {

namespace Implementation {

template<class T, class View = decltype(Containers::Implementation::ErasedArrayViewConverter<typename std::remove_reference<T&&>::type>::from(std::declval<T&&>()))> static auto stridedArrayViewTypeFor(T&&) -> typename std::remove_const<typename View::Type>::type;
template<class T> static typename std::remove_const<T>::type stridedArrayViewTypeFor(const Containers::ArrayView<T>&);
template<class T> static typename std::remove_const<T>::type stridedArrayViewTypeFor(const Containers::StridedArrayView1D<T>&);

}

template<class T> auto isInf(const Containers::StridedArrayView1D<const T>& range) -> decltype(isInf(std::declval<T>())) {
    if(range.isEmpty()) return {};

    auto out = isInf(range[0]);
    for(std::size_t i = 1; i != range.size(); ++i) {
        if(out) break;
        out = out || isInf(range[i]);
    }

    return out;
}

template<class Iterable, class T = decltype(Implementation::stridedArrayViewTypeFor(std::declval<Iterable&&>()))> inline auto isInf(Iterable&& range) -> decltype(isInf(std::declval<T>())) {
    return isInf<T>(Containers::StridedArrayView1D<const T>{range});
}

template<class T> inline auto isInf(std::initializer_list<T> list) -> decltype(isInf(std::declval<T>())) {
    return isInf<T>(Containers::stridedArrayView(list));
}

template<class T, std::size_t size> inline auto isInf(const T(&array)[size]) -> decltype(isInf(std::declval<T>())) {
    return isInf<T>(Containers::StridedArrayView1D<const T>{array});
}

template<class T> inline auto isNan(const Containers::StridedArrayView1D<const T>& range) -> decltype(isNan(std::declval<T>())) {
    if(range.isEmpty()) return {};

    auto out = isNan(range[0]);
    for(std::size_t i = 1; i != range.size(); ++i) {
        if(out) break;
        out = out || isNan(range[i]);
    }

    return out;
}

template<class Iterable, class T = decltype(Implementation::stridedArrayViewTypeFor(std::declval<Iterable&&>()))> inline auto isNan(Iterable&& range) -> decltype(isNan(std::declval<T>())) {
    return isNan<T>(Containers::StridedArrayView1D<const T>{range});
}

template<class T> inline auto isNan(std::initializer_list<T> list) -> decltype(isNan(std::declval<T>())) {
    return isNan<T>(Containers::stridedArrayView(list));
}

template<class T, std::size_t size> inline auto isNan(const T(&array)[size]) -> decltype(isNan(std::declval<T>())) {
    return isNan<T>(Containers::StridedArrayView1D<const T>{array});
}

namespace Implementation {
    template<class T, bool any> constexpr Containers::Pair<std::size_t, T> firstNonNan(Containers::StridedArrayView1D<const T> range, std::false_type, std::integral_constant<bool, any>) {
        return {0, range.front()};
    }
    template<class T> inline Containers::Pair<std::size_t, T> firstNonNan(Containers::StridedArrayView1D<const T> range, std::true_type, std::false_type) {
        for(std::size_t i = 0; i != range.size(); ++i)
            if(!isNan(range[i])) return {i, range[i]};
        return {range.size() - 1, range.back()};
    }
    template<class T> inline Containers::Pair<std::size_t, T> firstNonNan(Containers::StridedArrayView1D<const T> range, std::true_type, std::true_type) {
        T out = range[0];
        std::size_t firstValid = 0;
        for(std::size_t i = 1; i != range.size(); ++i) {
            BitVector<T::Size> nans = isNan(out);
            if(nans.none()) break;
            if(nans.all() && firstValid + 1 == i) ++firstValid;
            out = Math::lerp(out, range[i], isNan(out));
        }
        return {firstValid, out};
    }
}

template<class T> inline T min(const Containers::StridedArrayView1D<const T>& range) {
    if(range.isEmpty()) return {};

    Containers::Pair<std::size_t, T> iOut = Implementation::firstNonNan(range, IsFloatingPoint<T>{}, IsVector<T>{});
    for(++iOut.first(); iOut.first() != range.size(); ++iOut.first())
        iOut.second() = Math::min(iOut.second(), range[iOut.first()]);

    return iOut.second();
}

template<class Iterable, class T = decltype(Implementation::stridedArrayViewTypeFor(std::declval<Iterable&&>()))> inline T min(Iterable&& range) {
    return min<T>(Containers::StridedArrayView1D<const T>{range});
}

template<class T> inline T min(std::initializer_list<T> list) {
    return min<T>(Containers::stridedArrayView(list));
}

template<class T, std::size_t size> inline T min(const T(&array)[size]) {
    return min<T>(Containers::StridedArrayView1D<const T>{array});
}

template<class T> inline T max(const Containers::StridedArrayView1D<const T>& range) {
    if(range.isEmpty()) return {};

    Containers::Pair<std::size_t, T> iOut = Implementation::firstNonNan(range, IsFloatingPoint<T>{}, IsVector<T>{});
    for(++iOut.first(); iOut.first() != range.size(); ++iOut.first())
        iOut.second() = Math::max(iOut.second(), range[iOut.first()]);

    return iOut.second();
}

template<class Iterable, class T = decltype(Implementation::stridedArrayViewTypeFor(std::declval<Iterable&&>()))> inline T max(Iterable&& range) {
    return max<T>(Containers::StridedArrayView1D<const T>{range});
}

template<class T> inline T max(std::initializer_list<T> list) {
    return max<T>(Containers::stridedArrayView(list));
}

template<class T, std::size_t size> inline T max(const T(&array)[size]) {
    return max<T>(Containers::StridedArrayView1D<const T>{array});
}

namespace Implementation {
    template<class T, typename std::enable_if<IsScalar<T>::value, int>::type = 0> inline void minmax(T& min, T& max, T value) {
        if(value < min)
            min = value;
        else if(value > max)
            max = value;
    }
    template<std::size_t size, class T> inline void minmax(Vector<size, T>& min, Vector<size, T>& max, const Vector<size, T>& value) {
        for(std::size_t i = 0; i != size; ++i)
            minmax(min[i], max[i], value[i]);
    }
}

template<class T> inline Containers::Pair<T, T> minmax(const Containers::StridedArrayView1D<const T>& range) {
    if(range.isEmpty()) return {};

    Containers::Pair<std::size_t, T> iOut = Implementation::firstNonNan(range, IsFloatingPoint<T>{}, IsVector<T>{});
    T min{iOut.second()}, max{iOut.second()};
    for(++iOut.first(); iOut.first() != range.size(); ++iOut.first())
        Implementation::minmax(min, max, range[iOut.first()]);

    return {min, max};
}

template<class Iterable, class T = decltype(Implementation::stridedArrayViewTypeFor(std::declval<Iterable&&>()))> inline Containers::Pair<T, T> minmax(Iterable&& range) {
    return minmax<T>(Containers::StridedArrayView1D<const T>{range});
}

template<class T> inline Containers::Pair<T, T> minmax(std::initializer_list<T> list) {
    return minmax<T>(Containers::stridedArrayView(list));
}

template<class T, std::size_t size> inline Containers::Pair<T, T> minmax(const T(&array)[size]) {
    return minmax<T>(Containers::StridedArrayView1D<const T>{array});
}

}}

#endif
#ifndef Magnum_Math_PackingBatch_h
#define Magnum_Math_PackingBatch_h

namespace Magnum { namespace Math {

MAGNUM_EXPORT void unpackInto(const Containers::StridedArrayView2D<const UnsignedByte>& src, const Containers::StridedArrayView2D<Float>& dst);

MAGNUM_EXPORT void unpackInto(const Containers::StridedArrayView2D<const UnsignedShort>& src, const Containers::StridedArrayView2D<Float>& dst);

MAGNUM_EXPORT void unpackInto(const Containers::StridedArrayView2D<const Byte>& src, const Containers::StridedArrayView2D<Float>& dst);

MAGNUM_EXPORT void unpackInto(const Containers::StridedArrayView2D<const Short>& src, const Containers::StridedArrayView2D<Float>& dst);

MAGNUM_EXPORT void packInto(const Containers::StridedArrayView2D<const Float>& src, const Containers::StridedArrayView2D<UnsignedByte>& dst);

MAGNUM_EXPORT void packInto(const Containers::StridedArrayView2D<const Float>& src, const Containers::StridedArrayView2D<Byte>& dst);

MAGNUM_EXPORT void packInto(const Containers::StridedArrayView2D<const Float>& src, const Containers::StridedArrayView2D<UnsignedShort>& dst);

MAGNUM_EXPORT void packInto(const Containers::StridedArrayView2D<const Float>& src, const Containers::StridedArrayView2D<Short>& dst);

MAGNUM_EXPORT void packHalfInto(const Containers::StridedArrayView2D<const Float>& src, const Containers::StridedArrayView2D<UnsignedShort>& dst);

MAGNUM_EXPORT void unpackHalfInto(const Containers::StridedArrayView2D<const UnsignedShort>& src, const Containers::StridedArrayView2D<Float>& dst);

MAGNUM_EXPORT void castInto(const Containers::StridedArrayView2D<const UnsignedByte>& src, const Containers::StridedArrayView2D<Float>& dst);

MAGNUM_EXPORT void castInto(const Containers::StridedArrayView2D<const Byte>& src, const Containers::StridedArrayView2D<Float>& dst);

MAGNUM_EXPORT void castInto(const Containers::StridedArrayView2D<const UnsignedShort>& src, const Containers::StridedArrayView2D<Float>& dst);

MAGNUM_EXPORT void castInto(const Containers::StridedArrayView2D<const Short>& src, const Containers::StridedArrayView2D<Float>& dst);

MAGNUM_EXPORT void castInto(const Containers::StridedArrayView2D<const UnsignedInt>& src, const Containers::StridedArrayView2D<Float>& dst);

MAGNUM_EXPORT void castInto(const Containers::StridedArrayView2D<const Int>& src, const Containers::StridedArrayView2D<Float>& dst);

MAGNUM_EXPORT void castInto(const Containers::StridedArrayView2D<const UnsignedByte>& src, const Containers::StridedArrayView2D<Double>& dst);

MAGNUM_EXPORT void castInto(const Containers::StridedArrayView2D<const Byte>& src, const Containers::StridedArrayView2D<Double>& dst);

MAGNUM_EXPORT void castInto(const Containers::StridedArrayView2D<const UnsignedShort>& src, const Containers::StridedArrayView2D<Double>& dst);

MAGNUM_EXPORT void castInto(const Containers::StridedArrayView2D<const Short>& src, const Containers::StridedArrayView2D<Double>& dst);

MAGNUM_EXPORT void castInto(const Containers::StridedArrayView2D<const UnsignedInt>& src, const Containers::StridedArrayView2D<Double>& dst);

MAGNUM_EXPORT void castInto(const Containers::StridedArrayView2D<const Int>& src, const Containers::StridedArrayView2D<Double>& dst);

MAGNUM_EXPORT void castInto(const Containers::StridedArrayView2D<const Float>& src, const Containers::StridedArrayView2D<UnsignedByte>& dst);

MAGNUM_EXPORT void castInto(const Containers::StridedArrayView2D<const Float>& src, const Containers::StridedArrayView2D<Byte>& dst);

MAGNUM_EXPORT void castInto(const Containers::StridedArrayView2D<const Float>& src, const Containers::StridedArrayView2D<UnsignedShort>& dst);

MAGNUM_EXPORT void castInto(const Containers::StridedArrayView2D<const Float>& src, const Containers::StridedArrayView2D<Short>& dst);

MAGNUM_EXPORT void castInto(const Containers::StridedArrayView2D<const Float>& src, const Containers::StridedArrayView2D<UnsignedInt>& dst);

MAGNUM_EXPORT void castInto(const Containers::StridedArrayView2D<const Float>& src, const Containers::StridedArrayView2D<Int>& dst);

MAGNUM_EXPORT void castInto(const Containers::StridedArrayView2D<const Double>& src, const Containers::StridedArrayView2D<UnsignedByte>& dst);

MAGNUM_EXPORT void castInto(const Containers::StridedArrayView2D<const Double>& src, const Containers::StridedArrayView2D<Byte>& dst);

MAGNUM_EXPORT void castInto(const Containers::StridedArrayView2D<const Double>& src, const Containers::StridedArrayView2D<UnsignedShort>& dst);

MAGNUM_EXPORT void castInto(const Containers::StridedArrayView2D<const Double>& src, const Containers::StridedArrayView2D<Short>& dst);

MAGNUM_EXPORT void castInto(const Containers::StridedArrayView2D<const Double>& src, const Containers::StridedArrayView2D<UnsignedInt>& dst);

MAGNUM_EXPORT void castInto(const Containers::StridedArrayView2D<const Double>& src, const Containers::StridedArrayView2D<Int>& dst);

MAGNUM_EXPORT void castInto(const Containers::StridedArrayView2D<const UnsignedByte>& src, const Containers::StridedArrayView2D<UnsignedShort>& dst);

MAGNUM_EXPORT void castInto(const Containers::StridedArrayView2D<const Byte>& src, const Containers::StridedArrayView2D<Short>& dst);

MAGNUM_EXPORT void castInto(const Containers::StridedArrayView2D<const UnsignedByte>& src, const Containers::StridedArrayView2D<UnsignedInt>& dst);

MAGNUM_EXPORT void castInto(const Containers::StridedArrayView2D<const Byte>& src, const Containers::StridedArrayView2D<Int>& dst);

MAGNUM_EXPORT void castInto(const Containers::StridedArrayView2D<const UnsignedShort>& src, const Containers::StridedArrayView2D<UnsignedInt>& dst);

MAGNUM_EXPORT void castInto(const Containers::StridedArrayView2D<const Short>& src, const Containers::StridedArrayView2D<Int>& dst);

MAGNUM_EXPORT void castInto(const Containers::StridedArrayView2D<const UnsignedByte>& src, const Containers::StridedArrayView2D<UnsignedLong>& dst);

MAGNUM_EXPORT void castInto(const Containers::StridedArrayView2D<const Byte>& src, const Containers::StridedArrayView2D<Long>& dst);

MAGNUM_EXPORT void castInto(const Containers::StridedArrayView2D<const UnsignedShort>& src, const Containers::StridedArrayView2D<UnsignedLong>& dst);

MAGNUM_EXPORT void castInto(const Containers::StridedArrayView2D<const Short>& src, const Containers::StridedArrayView2D<Long>& dst);

MAGNUM_EXPORT void castInto(const Containers::StridedArrayView2D<const UnsignedInt>& src, const Containers::StridedArrayView2D<UnsignedLong>& dst);

MAGNUM_EXPORT void castInto(const Containers::StridedArrayView2D<const Int>& src, const Containers::StridedArrayView2D<Long>& dst);

MAGNUM_EXPORT void castInto(const Containers::StridedArrayView2D<const UnsignedLong>& src, const Containers::StridedArrayView2D<UnsignedInt>& dst);

MAGNUM_EXPORT void castInto(const Containers::StridedArrayView2D<const Long>& src, const Containers::StridedArrayView2D<Int>& dst);

MAGNUM_EXPORT void castInto(const Containers::StridedArrayView2D<const UnsignedLong>& src, const Containers::StridedArrayView2D<UnsignedShort>& dst);

MAGNUM_EXPORT void castInto(const Containers::StridedArrayView2D<const Long>& src, const Containers::StridedArrayView2D<Short>& dst);

MAGNUM_EXPORT void castInto(const Containers::StridedArrayView2D<const UnsignedLong>& src, const Containers::StridedArrayView2D<UnsignedByte>& dst);

MAGNUM_EXPORT void castInto(const Containers::StridedArrayView2D<const Long>& src, const Containers::StridedArrayView2D<Byte>& dst);

MAGNUM_EXPORT void castInto(const Containers::StridedArrayView2D<const UnsignedInt>& src, const Containers::StridedArrayView2D<UnsignedShort>& dst);

MAGNUM_EXPORT void castInto(const Containers::StridedArrayView2D<const Int>& src, const Containers::StridedArrayView2D<Short>& dst);

MAGNUM_EXPORT void castInto(const Containers::StridedArrayView2D<const UnsignedInt>& src, const Containers::StridedArrayView2D<UnsignedByte>& dst);

MAGNUM_EXPORT void castInto(const Containers::StridedArrayView2D<const Int>& src, const Containers::StridedArrayView2D<Byte>& dst);

MAGNUM_EXPORT void castInto(const Containers::StridedArrayView2D<const UnsignedShort>& src, const Containers::StridedArrayView2D<UnsignedByte>& dst);

MAGNUM_EXPORT void castInto(const Containers::StridedArrayView2D<const Short>& src, const Containers::StridedArrayView2D<Byte>& dst);

MAGNUM_EXPORT void castInto(const Containers::StridedArrayView2D<const Float>& src, const Containers::StridedArrayView2D<Double>& dst);

MAGNUM_EXPORT void castInto(const Containers::StridedArrayView2D<const Double>& src, const Containers::StridedArrayView2D<Float>& dst);

MAGNUM_EXPORT void castInto(const Containers::StridedArrayView2D<const UnsignedByte>& src, const Containers::StridedArrayView2D<UnsignedByte>& dst);

MAGNUM_EXPORT void castInto(const Containers::StridedArrayView2D<const Byte>& src, const Containers::StridedArrayView2D<Byte>& dst);

MAGNUM_EXPORT void castInto(const Containers::StridedArrayView2D<const UnsignedShort>& src, const Containers::StridedArrayView2D<UnsignedShort>& dst);

MAGNUM_EXPORT void castInto(const Containers::StridedArrayView2D<const Short>& src, const Containers::StridedArrayView2D<Short>& dst);

MAGNUM_EXPORT void castInto(const Containers::StridedArrayView2D<const UnsignedInt>& src, const Containers::StridedArrayView2D<UnsignedInt>& dst);

MAGNUM_EXPORT void castInto(const Containers::StridedArrayView2D<const Int>& src, const Containers::StridedArrayView2D<Int>& dst);

MAGNUM_EXPORT void castInto(const Containers::StridedArrayView2D<const UnsignedLong>& src, const Containers::StridedArrayView2D<UnsignedLong>& dst);

MAGNUM_EXPORT void castInto(const Containers::StridedArrayView2D<const Long>& src, const Containers::StridedArrayView2D<Long>& dst);

MAGNUM_EXPORT void castInto(const Containers::StridedArrayView2D<const Float>& src, const Containers::StridedArrayView2D<Float>& dst);

MAGNUM_EXPORT void castInto(const Containers::StridedArrayView2D<const Double>& src, const Containers::StridedArrayView2D<Double>& dst);

}}

#endif
#if defined(MAGNUM_MATH_BATCH_IMPLEMENTATION) && !defined(MagnumMathBatch_hpp_implementation)
#define MagnumMathBatch_hpp_implementation
namespace Magnum { namespace Math { namespace {

constexpr UnsignedInt HalfMantissaTable[2048] = {
    0x00000000, 0x33800000, 0x34000000, 0x34400000, 0x34800000, 0x34a00000,
    0x34c00000, 0x34e00000, 0x35000000, 0x35100000, 0x35200000, 0x35300000,
    0x35400000, 0x35500000, 0x35600000, 0x35700000, 0x35800000, 0x35880000,
    0x35900000, 0x35980000, 0x35a00000, 0x35a80000, 0x35b00000, 0x35b80000,
    0x35c00000, 0x35c80000, 0x35d00000, 0x35d80000, 0x35e00000, 0x35e80000,
    0x35f00000, 0x35f80000, 0x36000000, 0x36040000, 0x36080000, 0x360c0000,
    0x36100000, 0x36140000, 0x36180000, 0x361c0000, 0x36200000, 0x36240000,
    0x36280000, 0x362c0000, 0x36300000, 0x36340000, 0x36380000, 0x363c0000,
    0x36400000, 0x36440000, 0x36480000, 0x364c0000, 0x36500000, 0x36540000,
    0x36580000, 0x365c0000, 0x36600000, 0x36640000, 0x36680000, 0x366c0000,
    0x36700000, 0x36740000, 0x36780000, 0x367c0000, 0x36800000, 0x36820000,
    0x36840000, 0x36860000, 0x36880000, 0x368a0000, 0x368c0000, 0x368e0000,
    0x36900000, 0x36920000, 0x36940000, 0x36960000, 0x36980000, 0x369a0000,
    0x369c0000, 0x369e0000, 0x36a00000, 0x36a20000, 0x36a40000, 0x36a60000,
    0x36a80000, 0x36aa0000, 0x36ac0000, 0x36ae0000, 0x36b00000, 0x36b20000,
    0x36b40000, 0x36b60000, 0x36b80000, 0x36ba0000, 0x36bc0000, 0x36be0000,
    0x36c00000, 0x36c20000, 0x36c40000, 0x36c60000, 0x36c80000, 0x36ca0000,
    0x36cc0000, 0x36ce0000, 0x36d00000, 0x36d20000, 0x36d40000, 0x36d60000,
    0x36d80000, 0x36da0000, 0x36dc0000, 0x36de0000, 0x36e00000, 0x36e20000,
    0x36e40000, 0x36e60000, 0x36e80000, 0x36ea0000, 0x36ec0000, 0x36ee0000,
    0x36f00000, 0x36f20000, 0x36f40000, 0x36f60000, 0x36f80000, 0x36fa0000,
    0x36fc0000, 0x36fe0000, 0x37000000, 0x37010000, 0x37020000, 0x37030000,
    0x37040000, 0x37050000, 0x37060000, 0x37070000, 0x37080000, 0x37090000,
    0x370a0000, 0x370b0000, 0x370c0000, 0x370d0000, 0x370e0000, 0x370f0000,
    0x37100000, 0x37110000, 0x37120000, 0x37130000, 0x37140000, 0x37150000,
    0x37160000, 0x37170000, 0x37180000, 0x37190000, 0x371a0000, 0x371b0000,
    0x371c0000, 0x371d0000, 0x371e0000, 0x371f0000, 0x37200000, 0x37210000,
    0x37220000, 0x37230000, 0x37240000, 0x37250000, 0x37260000, 0x37270000,
    0x37280000, 0x37290000, 0x372a0000, 0x372b0000, 0x372c0000, 0x372d0000,
    0x372e0000, 0x372f0000, 0x37300000, 0x37310000, 0x37320000, 0x37330000,
    0x37340000, 0x37350000, 0x37360000, 0x37370000, 0x37380000, 0x37390000,
    0x373a0000, 0x373b0000, 0x373c0000, 0x373d0000, 0x373e0000, 0x373f0000,
    0x37400000, 0x37410000, 0x37420000, 0x37430000, 0x37440000, 0x37450000,
    0x37460000, 0x37470000, 0x37480000, 0x37490000, 0x374a0000, 0x374b0000,
    0x374c0000, 0x374d0000, 0x374e0000, 0x374f0000, 0x37500000, 0x37510000,
    0x37520000, 0x37530000, 0x37540000, 0x37550000, 0x37560000, 0x37570000,
    0x37580000, 0x37590000, 0x375a0000, 0x375b0000, 0x375c0000, 0x375d0000,
    0x375e0000, 0x375f0000, 0x37600000, 0x37610000, 0x37620000, 0x37630000,
    0x37640000, 0x37650000, 0x37660000, 0x37670000, 0x37680000, 0x37690000,
    0x376a0000, 0x376b0000, 0x376c0000, 0x376d0000, 0x376e0000, 0x376f0000,
    0x37700000, 0x37710000, 0x37720000, 0x37730000, 0x37740000, 0x37750000,
    0x37760000, 0x37770000, 0x37780000, 0x37790000, 0x377a0000, 0x377b0000,
    0x377c0000, 0x377d0000, 0x377e0000, 0x377f0000, 0x37800000, 0x37808000,
    0x37810000, 0x37818000, 0x37820000, 0x37828000, 0x37830000, 0x37838000,
    0x37840000, 0x37848000, 0x37850000, 0x37858000, 0x37860000, 0x37868000,
    0x37870000, 0x37878000, 0x37880000, 0x37888000, 0x37890000, 0x37898000,
    0x378a0000, 0x378a8000, 0x378b0000, 0x378b8000, 0x378c0000, 0x378c8000,
    0x378d0000, 0x378d8000, 0x378e0000, 0x378e8000, 0x378f0000, 0x378f8000,
    0x37900000, 0x37908000, 0x37910000, 0x37918000, 0x37920000, 0x37928000,
    0x37930000, 0x37938000, 0x37940000, 0x37948000, 0x37950000, 0x37958000,
    0x37960000, 0x37968000, 0x37970000, 0x37978000, 0x37980000, 0x37988000,
    0x37990000, 0x37998000, 0x379a0000, 0x379a8000, 0x379b0000, 0x379b8000,
    0x379c0000, 0x379c8000, 0x379d0000, 0x379d8000, 0x379e0000, 0x379e8000,
    0x379f0000, 0x379f8000, 0x37a00000, 0x37a08000, 0x37a10000, 0x37a18000,
    0x37a20000, 0x37a28000, 0x37a30000, 0x37a38000, 0x37a40000, 0x37a48000,
    0x37a50000, 0x37a58000, 0x37a60000, 0x37a68000, 0x37a70000, 0x37a78000,
    0x37a80000, 0x37a88000, 0x37a90000, 0x37a98000, 0x37aa0000, 0x37aa8000,
    0x37ab0000, 0x37ab8000, 0x37ac0000, 0x37ac8000, 0x37ad0000, 0x37ad8000,
    0x37ae0000, 0x37ae8000, 0x37af0000, 0x37af8000, 0x37b00000, 0x37b08000,
    0x37b10000, 0x37b18000, 0x37b20000, 0x37b28000, 0x37b30000, 0x37b38000,
    0x37b40000, 0x37b48000, 0x37b50000, 0x37b58000, 0x37b60000, 0x37b68000,
    0x37b70000, 0x37b78000, 0x37b80000, 0x37b88000, 0x37b90000, 0x37b98000,
    0x37ba0000, 0x37ba8000, 0x37bb0000, 0x37bb8000, 0x37bc0000, 0x37bc8000,
    0x37bd0000, 0x37bd8000, 0x37be0000, 0x37be8000, 0x37bf0000, 0x37bf8000,
    0x37c00000, 0x37c08000, 0x37c10000, 0x37c18000, 0x37c20000, 0x37c28000,
    0x37c30000, 0x37c38000, 0x37c40000, 0x37c48000, 0x37c50000, 0x37c58000,
    0x37c60000, 0x37c68000, 0x37c70000, 0x37c78000, 0x37c80000, 0x37c88000,
    0x37c90000, 0x37c98000, 0x37ca0000, 0x37ca8000, 0x37cb0000, 0x37cb8000,
    0x37cc0000, 0x37cc8000, 0x37cd0000, 0x37cd8000, 0x37ce0000, 0x37ce8000,
    0x37cf0000, 0x37cf8000, 0x37d00000, 0x37d08000, 0x37d10000, 0x37d18000,
    0x37d20000, 0x37d28000, 0x37d30000, 0x37d38000, 0x37d40000, 0x37d48000,
    0x37d50000, 0x37d58000, 0x37d60000, 0x37d68000, 0x37d70000, 0x37d78000,
    0x37d80000, 0x37d88000, 0x37d90000, 0x37d98000, 0x37da0000, 0x37da8000,
    0x37db0000, 0x37db8000, 0x37dc0000, 0x37dc8000, 0x37dd0000, 0x37dd8000,
    0x37de0000, 0x37de8000, 0x37df0000, 0x37df8000, 0x37e00000, 0x37e08000,
    0x37e10000, 0x37e18000, 0x37e20000, 0x37e28000, 0x37e30000, 0x37e38000,
    0x37e40000, 0x37e48000, 0x37e50000, 0x37e58000, 0x37e60000, 0x37e68000,
    0x37e70000, 0x37e78000, 0x37e80000, 0x37e88000, 0x37e90000, 0x37e98000,
    0x37ea0000, 0x37ea8000, 0x37eb0000, 0x37eb8000, 0x37ec0000, 0x37ec8000,
    0x37ed0000, 0x37ed8000, 0x37ee0000, 0x37ee8000, 0x37ef0000, 0x37ef8000,
    0x37f00000, 0x37f08000, 0x37f10000, 0x37f18000, 0x37f20000, 0x37f28000,
    0x37f30000, 0x37f38000, 0x37f40000, 0x37f48000, 0x37f50000, 0x37f58000,
    0x37f60000, 0x37f68000, 0x37f70000, 0x37f78000, 0x37f80000, 0x37f88000,
    0x37f90000, 0x37f98000, 0x37fa0000, 0x37fa8000, 0x37fb0000, 0x37fb8000,
    0x37fc0000, 0x37fc8000, 0x37fd0000, 0x37fd8000, 0x37fe0000, 0x37fe8000,
    0x37ff0000, 0x37ff8000, 0x38000000, 0x38004000, 0x38008000, 0x3800c000,
    0x38010000, 0x38014000, 0x38018000, 0x3801c000, 0x38020000, 0x38024000,
    0x38028000, 0x3802c000, 0x38030000, 0x38034000, 0x38038000, 0x3803c000,
    0x38040000, 0x38044000, 0x38048000, 0x3804c000, 0x38050000, 0x38054000,
    0x38058000, 0x3805c000, 0x38060000, 0x38064000, 0x38068000, 0x3806c000,
    0x38070000, 0x38074000, 0x38078000, 0x3807c000, 0x38080000, 0x38084000,
    0x38088000, 0x3808c000, 0x38090000, 0x38094000, 0x38098000, 0x3809c000,
    0x380a0000, 0x380a4000, 0x380a8000, 0x380ac000, 0x380b0000, 0x380b4000,
    0x380b8000, 0x380bc000, 0x380c0000, 0x380c4000, 0x380c8000, 0x380cc000,
    0x380d0000, 0x380d4000, 0x380d8000, 0x380dc000, 0x380e0000, 0x380e4000,
    0x380e8000, 0x380ec000, 0x380f0000, 0x380f4000, 0x380f8000, 0x380fc000,
    0x38100000, 0x38104000, 0x38108000, 0x3810c000, 0x38110000, 0x38114000,
    0x38118000, 0x3811c000, 0x38120000, 0x38124000, 0x38128000, 0x3812c000,
    0x38130000, 0x38134000, 0x38138000, 0x3813c000, 0x38140000, 0x38144000,
    0x38148000, 0x3814c000, 0x38150000, 0x38154000, 0x38158000, 0x3815c000,
    0x38160000, 0x38164000, 0x38168000, 0x3816c000, 0x38170000, 0x38174000,
    0x38178000, 0x3817c000, 0x38180000, 0x38184000, 0x38188000, 0x3818c000,
    0x38190000, 0x38194000, 0x38198000, 0x3819c000, 0x381a0000, 0x381a4000,
    0x381a8000, 0x381ac000, 0x381b0000, 0x381b4000, 0x381b8000, 0x381bc000,
    0x381c0000, 0x381c4000, 0x381c8000, 0x381cc000, 0x381d0000, 0x381d4000,
    0x381d8000, 0x381dc000, 0x381e0000, 0x381e4000, 0x381e8000, 0x381ec000,
    0x381f0000, 0x381f4000, 0x381f8000, 0x381fc000, 0x38200000, 0x38204000,
    0x38208000, 0x3820c000, 0x38210000, 0x38214000, 0x38218000, 0x3821c000,
    0x38220000, 0x38224000, 0x38228000, 0x3822c000, 0x38230000, 0x38234000,
    0x38238000, 0x3823c000, 0x38240000, 0x38244000, 0x38248000, 0x3824c000,
    0x38250000, 0x38254000, 0x38258000, 0x3825c000, 0x38260000, 0x38264000,
    0x38268000, 0x3826c000, 0x38270000, 0x38274000, 0x38278000, 0x3827c000,
    0x38280000, 0x38284000, 0x38288000, 0x3828c000, 0x38290000, 0x38294000,
    0x38298000, 0x3829c000, 0x382a0000, 0x382a4000, 0x382a8000, 0x382ac000,
    0x382b0000, 0x382b4000, 0x382b8000, 0x382bc000, 0x382c0000, 0x382c4000,
    0x382c8000, 0x382cc000, 0x382d0000, 0x382d4000, 0x382d8000, 0x382dc000,
    0x382e0000, 0x382e4000, 0x382e8000, 0x382ec000, 0x382f0000, 0x382f4000,
    0x382f8000, 0x382fc000, 0x38300000, 0x38304000, 0x38308000, 0x3830c000,
    0x38310000, 0x38314000, 0x38318000, 0x3831c000, 0x38320000, 0x38324000,
    0x38328000, 0x3832c000, 0x38330000, 0x38334000, 0x38338000, 0x3833c000,
    0x38340000, 0x38344000, 0x38348000, 0x3834c000, 0x38350000, 0x38354000,
    0x38358000, 0x3835c000, 0x38360000, 0x38364000, 0x38368000, 0x3836c000,
    0x38370000, 0x38374000, 0x38378000, 0x3837c000, 0x38380000, 0x38384000,
    0x38388000, 0x3838c000, 0x38390000, 0x38394000, 0x38398000, 0x3839c000,
    0x383a0000, 0x383a4000, 0x383a8000, 0x383ac000, 0x383b0000, 0x383b4000,
    0x383b8000, 0x383bc000, 0x383c0000, 0x383c4000, 0x383c8000, 0x383cc000,
    0x383d0000, 0x383d4000, 0x383d8000, 0x383dc000, 0x383e0000, 0x383e4000,
    0x383e8000, 0x383ec000, 0x383f0000, 0x383f4000, 0x383f8000, 0x383fc000,
    0x38400000, 0x38404000, 0x38408000, 0x3840c000, 0x38410000, 0x38414000,
    0x38418000, 0x3841c000, 0x38420000, 0x38424000, 0x38428000, 0x3842c000,
    0x38430000, 0x38434000, 0x38438000, 0x3843c000, 0x38440000, 0x38444000,
    0x38448000, 0x3844c000, 0x38450000, 0x38454000, 0x38458000, 0x3845c000,
    0x38460000, 0x38464000, 0x38468000, 0x3846c000, 0x38470000, 0x38474000,
    0x38478000, 0x3847c000, 0x38480000, 0x38484000, 0x38488000, 0x3848c000,
    0x38490000, 0x38494000, 0x38498000, 0x3849c000, 0x384a0000, 0x384a4000,
    0x384a8000, 0x384ac000, 0x384b0000, 0x384b4000, 0x384b8000, 0x384bc000,
    0x384c0000, 0x384c4000, 0x384c8000, 0x384cc000, 0x384d0000, 0x384d4000,
    0x384d8000, 0x384dc000, 0x384e0000, 0x384e4000, 0x384e8000, 0x384ec000,
    0x384f0000, 0x384f4000, 0x384f8000, 0x384fc000, 0x38500000, 0x38504000,
    0x38508000, 0x3850c000, 0x38510000, 0x38514000, 0x38518000, 0x3851c000,
    0x38520000, 0x38524000, 0x38528000, 0x3852c000, 0x38530000, 0x38534000,
    0x38538000, 0x3853c000, 0x38540000, 0x38544000, 0x38548000, 0x3854c000,
    0x38550000, 0x38554000, 0x38558000, 0x3855c000, 0x38560000, 0x38564000,
    0x38568000, 0x3856c000, 0x38570000, 0x38574000, 0x38578000, 0x3857c000,
    0x38580000, 0x38584000, 0x38588000, 0x3858c000, 0x38590000, 0x38594000,
    0x38598000, 0x3859c000, 0x385a0000, 0x385a4000, 0x385a8000, 0x385ac000,
    0x385b0000, 0x385b4000, 0x385b8000, 0x385bc000, 0x385c0000, 0x385c4000,
    0x385c8000, 0x385cc000, 0x385d0000, 0x385d4000, 0x385d8000, 0x385dc000,
    0x385e0000, 0x385e4000, 0x385e8000, 0x385ec000, 0x385f0000, 0x385f4000,
    0x385f8000, 0x385fc000, 0x38600000, 0x38604000, 0x38608000, 0x3860c000,
    0x38610000, 0x38614000, 0x38618000, 0x3861c000, 0x38620000, 0x38624000,
    0x38628000, 0x3862c000, 0x38630000, 0x38634000, 0x38638000, 0x3863c000,
    0x38640000, 0x38644000, 0x38648000, 0x3864c000, 0x38650000, 0x38654000,
    0x38658000, 0x3865c000, 0x38660000, 0x38664000, 0x38668000, 0x3866c000,
    0x38670000, 0x38674000, 0x38678000, 0x3867c000, 0x38680000, 0x38684000,
    0x38688000, 0x3868c000, 0x38690000, 0x38694000, 0x38698000, 0x3869c000,
    0x386a0000, 0x386a4000, 0x386a8000, 0x386ac000, 0x386b0000, 0x386b4000,
    0x386b8000, 0x386bc000, 0x386c0000, 0x386c4000, 0x386c8000, 0x386cc000,
    0x386d0000, 0x386d4000, 0x386d8000, 0x386dc000, 0x386e0000, 0x386e4000,
    0x386e8000, 0x386ec000, 0x386f0000, 0x386f4000, 0x386f8000, 0x386fc000,
    0x38700000, 0x38704000, 0x38708000, 0x3870c000, 0x38710000, 0x38714000,
    0x38718000, 0x3871c000, 0x38720000, 0x38724000, 0x38728000, 0x3872c000,
    0x38730000, 0x38734000, 0x38738000, 0x3873c000, 0x38740000, 0x38744000,
    0x38748000, 0x3874c000, 0x38750000, 0x38754000, 0x38758000, 0x3875c000,
    0x38760000, 0x38764000, 0x38768000, 0x3876c000, 0x38770000, 0x38774000,
    0x38778000, 0x3877c000, 0x38780000, 0x38784000, 0x38788000, 0x3878c000,
    0x38790000, 0x38794000, 0x38798000, 0x3879c000, 0x387a0000, 0x387a4000,
    0x387a8000, 0x387ac000, 0x387b0000, 0x387b4000, 0x387b8000, 0x387bc000,
    0x387c0000, 0x387c4000, 0x387c8000, 0x387cc000, 0x387d0000, 0x387d4000,
    0x387d8000, 0x387dc000, 0x387e0000, 0x387e4000, 0x387e8000, 0x387ec000,
    0x387f0000, 0x387f4000, 0x387f8000, 0x387fc000, 0x38000000, 0x38002000,
    0x38004000, 0x38006000, 0x38008000, 0x3800a000, 0x3800c000, 0x3800e000,
    0x38010000, 0x38012000, 0x38014000, 0x38016000, 0x38018000, 0x3801a000,
    0x3801c000, 0x3801e000, 0x38020000, 0x38022000, 0x38024000, 0x38026000,
    0x38028000, 0x3802a000, 0x3802c000, 0x3802e000, 0x38030000, 0x38032000,
    0x38034000, 0x38036000, 0x38038000, 0x3803a000, 0x3803c000, 0x3803e000,
    0x38040000, 0x38042000, 0x38044000, 0x38046000, 0x38048000, 0x3804a000,
    0x3804c000, 0x3804e000, 0x38050000, 0x38052000, 0x38054000, 0x38056000,
    0x38058000, 0x3805a000, 0x3805c000, 0x3805e000, 0x38060000, 0x38062000,
    0x38064000, 0x38066000, 0x38068000, 0x3806a000, 0x3806c000, 0x3806e000,
    0x38070000, 0x38072000, 0x38074000, 0x38076000, 0x38078000, 0x3807a000,
    0x3807c000, 0x3807e000, 0x38080000, 0x38082000, 0x38084000, 0x38086000,
    0x38088000, 0x3808a000, 0x3808c000, 0x3808e000, 0x38090000, 0x38092000,
    0x38094000, 0x38096000, 0x38098000, 0x3809a000, 0x3809c000, 0x3809e000,
    0x380a0000, 0x380a2000, 0x380a4000, 0x380a6000, 0x380a8000, 0x380aa000,
    0x380ac000, 0x380ae000, 0x380b0000, 0x380b2000, 0x380b4000, 0x380b6000,
    0x380b8000, 0x380ba000, 0x380bc000, 0x380be000, 0x380c0000, 0x380c2000,
    0x380c4000, 0x380c6000, 0x380c8000, 0x380ca000, 0x380cc000, 0x380ce000,
    0x380d0000, 0x380d2000, 0x380d4000, 0x380d6000, 0x380d8000, 0x380da000,
    0x380dc000, 0x380de000, 0x380e0000, 0x380e2000, 0x380e4000, 0x380e6000,
    0x380e8000, 0x380ea000, 0x380ec000, 0x380ee000, 0x380f0000, 0x380f2000,
    0x380f4000, 0x380f6000, 0x380f8000, 0x380fa000, 0x380fc000, 0x380fe000,
    0x38100000, 0x38102000, 0x38104000, 0x38106000, 0x38108000, 0x3810a000,
    0x3810c000, 0x3810e000, 0x38110000, 0x38112000, 0x38114000, 0x38116000,
    0x38118000, 0x3811a000, 0x3811c000, 0x3811e000, 0x38120000, 0x38122000,
    0x38124000, 0x38126000, 0x38128000, 0x3812a000, 0x3812c000, 0x3812e000,
    0x38130000, 0x38132000, 0x38134000, 0x38136000, 0x38138000, 0x3813a000,
    0x3813c000, 0x3813e000, 0x38140000, 0x38142000, 0x38144000, 0x38146000,
    0x38148000, 0x3814a000, 0x3814c000, 0x3814e000, 0x38150000, 0x38152000,
    0x38154000, 0x38156000, 0x38158000, 0x3815a000, 0x3815c000, 0x3815e000,
    0x38160000, 0x38162000, 0x38164000, 0x38166000, 0x38168000, 0x3816a000,
    0x3816c000, 0x3816e000, 0x38170000, 0x38172000, 0x38174000, 0x38176000,
    0x38178000, 0x3817a000, 0x3817c000, 0x3817e000, 0x38180000, 0x38182000,
    0x38184000, 0x38186000, 0x38188000, 0x3818a000, 0x3818c000, 0x3818e000,
    0x38190000, 0x38192000, 0x38194000, 0x38196000, 0x38198000, 0x3819a000,
    0x3819c000, 0x3819e000, 0x381a0000, 0x381a2000, 0x381a4000, 0x381a6000,
    0x381a8000, 0x381aa000, 0x381ac000, 0x381ae000, 0x381b0000, 0x381b2000,
    0x381b4000, 0x381b6000, 0x381b8000, 0x381ba000, 0x381bc000, 0x381be000,
    0x381c0000, 0x381c2000, 0x381c4000, 0x381c6000, 0x381c8000, 0x381ca000,
    0x381cc000, 0x381ce000, 0x381d0000, 0x381d2000, 0x381d4000, 0x381d6000,
    0x381d8000, 0x381da000, 0x381dc000, 0x381de000, 0x381e0000, 0x381e2000,
    0x381e4000, 0x381e6000, 0x381e8000, 0x381ea000, 0x381ec000, 0x381ee000,
    0x381f0000, 0x381f2000, 0x381f4000, 0x381f6000, 0x381f8000, 0x381fa000,
    0x381fc000, 0x381fe000, 0x38200000, 0x38202000, 0x38204000, 0x38206000,
    0x38208000, 0x3820a000, 0x3820c000, 0x3820e000, 0x38210000, 0x38212000,
    0x38214000, 0x38216000, 0x38218000, 0x3821a000, 0x3821c000, 0x3821e000,
    0x38220000, 0x38222000, 0x38224000, 0x38226000, 0x38228000, 0x3822a000,
    0x3822c000, 0x3822e000, 0x38230000, 0x38232000, 0x38234000, 0x38236000,
    0x38238000, 0x3823a000, 0x3823c000, 0x3823e000, 0x38240000, 0x38242000,
    0x38244000, 0x38246000, 0x38248000, 0x3824a000, 0x3824c000, 0x3824e000,
    0x38250000, 0x38252000, 0x38254000, 0x38256000, 0x38258000, 0x3825a000,
    0x3825c000, 0x3825e000, 0x38260000, 0x38262000, 0x38264000, 0x38266000,
    0x38268000, 0x3826a000, 0x3826c000, 0x3826e000, 0x38270000, 0x38272000,
    0x38274000, 0x38276000, 0x38278000, 0x3827a000, 0x3827c000, 0x3827e000,
    0x38280000, 0x38282000, 0x38284000, 0x38286000, 0x38288000, 0x3828a000,
    0x3828c000, 0x3828e000, 0x38290000, 0x38292000, 0x38294000, 0x38296000,
    0x38298000, 0x3829a000, 0x3829c000, 0x3829e000, 0x382a0000, 0x382a2000,
    0x382a4000, 0x382a6000, 0x382a8000, 0x382aa000, 0x382ac000, 0x382ae000,
    0x382b0000, 0x382b2000, 0x382b4000, 0x382b6000, 0x382b8000, 0x382ba000,
    0x382bc000, 0x382be000, 0x382c0000, 0x382c2000, 0x382c4000, 0x382c6000,
    0x382c8000, 0x382ca000, 0x382cc000, 0x382ce000, 0x382d0000, 0x382d2000,
    0x382d4000, 0x382d6000, 0x382d8000, 0x382da000, 0x382dc000, 0x382de000,
    0x382e0000, 0x382e2000, 0x382e4000, 0x382e6000, 0x382e8000, 0x382ea000,
    0x382ec000, 0x382ee000, 0x382f0000, 0x382f2000, 0x382f4000, 0x382f6000,
    0x382f8000, 0x382fa000, 0x382fc000, 0x382fe000, 0x38300000, 0x38302000,
    0x38304000, 0x38306000, 0x38308000, 0x3830a000, 0x3830c000, 0x3830e000,
    0x38310000, 0x38312000, 0x38314000, 0x38316000, 0x38318000, 0x3831a000,
    0x3831c000, 0x3831e000, 0x38320000, 0x38322000, 0x38324000, 0x38326000,
    0x38328000, 0x3832a000, 0x3832c000, 0x3832e000, 0x38330000, 0x38332000,
    0x38334000, 0x38336000, 0x38338000, 0x3833a000, 0x3833c000, 0x3833e000,
    0x38340000, 0x38342000, 0x38344000, 0x38346000, 0x38348000, 0x3834a000,
    0x3834c000, 0x3834e000, 0x38350000, 0x38352000, 0x38354000, 0x38356000,
    0x38358000, 0x3835a000, 0x3835c000, 0x3835e000, 0x38360000, 0x38362000,
    0x38364000, 0x38366000, 0x38368000, 0x3836a000, 0x3836c000, 0x3836e000,
    0x38370000, 0x38372000, 0x38374000, 0x38376000, 0x38378000, 0x3837a000,
    0x3837c000, 0x3837e000, 0x38380000, 0x38382000, 0x38384000, 0x38386000,
    0x38388000, 0x3838a000, 0x3838c000, 0x3838e000, 0x38390000, 0x38392000,
    0x38394000, 0x38396000, 0x38398000, 0x3839a000, 0x3839c000, 0x3839e000,
    0x383a0000, 0x383a2000, 0x383a4000, 0x383a6000, 0x383a8000, 0x383aa000,
    0x383ac000, 0x383ae000, 0x383b0000, 0x383b2000, 0x383b4000, 0x383b6000,
    0x383b8000, 0x383ba000, 0x383bc000, 0x383be000, 0x383c0000, 0x383c2000,
    0x383c4000, 0x383c6000, 0x383c8000, 0x383ca000, 0x383cc000, 0x383ce000,
    0x383d0000, 0x383d2000, 0x383d4000, 0x383d6000, 0x383d8000, 0x383da000,
    0x383dc000, 0x383de000, 0x383e0000, 0x383e2000, 0x383e4000, 0x383e6000,
    0x383e8000, 0x383ea000, 0x383ec000, 0x383ee000, 0x383f0000, 0x383f2000,
    0x383f4000, 0x383f6000, 0x383f8000, 0x383fa000, 0x383fc000, 0x383fe000,
    0x38400000, 0x38402000, 0x38404000, 0x38406000, 0x38408000, 0x3840a000,
    0x3840c000, 0x3840e000, 0x38410000, 0x38412000, 0x38414000, 0x38416000,
    0x38418000, 0x3841a000, 0x3841c000, 0x3841e000, 0x38420000, 0x38422000,
    0x38424000, 0x38426000, 0x38428000, 0x3842a000, 0x3842c000, 0x3842e000,
    0x38430000, 0x38432000, 0x38434000, 0x38436000, 0x38438000, 0x3843a000,
    0x3843c000, 0x3843e000, 0x38440000, 0x38442000, 0x38444000, 0x38446000,
    0x38448000, 0x3844a000, 0x3844c000, 0x3844e000, 0x38450000, 0x38452000,
    0x38454000, 0x38456000, 0x38458000, 0x3845a000, 0x3845c000, 0x3845e000,
    0x38460000, 0x38462000, 0x38464000, 0x38466000, 0x38468000, 0x3846a000,
    0x3846c000, 0x3846e000, 0x38470000, 0x38472000, 0x38474000, 0x38476000,
    0x38478000, 0x3847a000, 0x3847c000, 0x3847e000, 0x38480000, 0x38482000,
    0x38484000, 0x38486000, 0x38488000, 0x3848a000, 0x3848c000, 0x3848e000,
    0x38490000, 0x38492000, 0x38494000, 0x38496000, 0x38498000, 0x3849a000,
    0x3849c000, 0x3849e000, 0x384a0000, 0x384a2000, 0x384a4000, 0x384a6000,
    0x384a8000, 0x384aa000, 0x384ac000, 0x384ae000, 0x384b0000, 0x384b2000,
    0x384b4000, 0x384b6000, 0x384b8000, 0x384ba000, 0x384bc000, 0x384be000,
    0x384c0000, 0x384c2000, 0x384c4000, 0x384c6000, 0x384c8000, 0x384ca000,
    0x384cc000, 0x384ce000, 0x384d0000, 0x384d2000, 0x384d4000, 0x384d6000,
    0x384d8000, 0x384da000, 0x384dc000, 0x384de000, 0x384e0000, 0x384e2000,
    0x384e4000, 0x384e6000, 0x384e8000, 0x384ea000, 0x384ec000, 0x384ee000,
    0x384f0000, 0x384f2000, 0x384f4000, 0x384f6000, 0x384f8000, 0x384fa000,
    0x384fc000, 0x384fe000, 0x38500000, 0x38502000, 0x38504000, 0x38506000,
    0x38508000, 0x3850a000, 0x3850c000, 0x3850e000, 0x38510000, 0x38512000,
    0x38514000, 0x38516000, 0x38518000, 0x3851a000, 0x3851c000, 0x3851e000,
    0x38520000, 0x38522000, 0x38524000, 0x38526000, 0x38528000, 0x3852a000,
    0x3852c000, 0x3852e000, 0x38530000, 0x38532000, 0x38534000, 0x38536000,
    0x38538000, 0x3853a000, 0x3853c000, 0x3853e000, 0x38540000, 0x38542000,
    0x38544000, 0x38546000, 0x38548000, 0x3854a000, 0x3854c000, 0x3854e000,
    0x38550000, 0x38552000, 0x38554000, 0x38556000, 0x38558000, 0x3855a000,
    0x3855c000, 0x3855e000, 0x38560000, 0x38562000, 0x38564000, 0x38566000,
    0x38568000, 0x3856a000, 0x3856c000, 0x3856e000, 0x38570000, 0x38572000,
    0x38574000, 0x38576000, 0x38578000, 0x3857a000, 0x3857c000, 0x3857e000,
    0x38580000, 0x38582000, 0x38584000, 0x38586000, 0x38588000, 0x3858a000,
    0x3858c000, 0x3858e000, 0x38590000, 0x38592000, 0x38594000, 0x38596000,
    0x38598000, 0x3859a000, 0x3859c000, 0x3859e000, 0x385a0000, 0x385a2000,
    0x385a4000, 0x385a6000, 0x385a8000, 0x385aa000, 0x385ac000, 0x385ae000,
    0x385b0000, 0x385b2000, 0x385b4000, 0x385b6000, 0x385b8000, 0x385ba000,
    0x385bc000, 0x385be000, 0x385c0000, 0x385c2000, 0x385c4000, 0x385c6000,
    0x385c8000, 0x385ca000, 0x385cc000, 0x385ce000, 0x385d0000, 0x385d2000,
    0x385d4000, 0x385d6000, 0x385d8000, 0x385da000, 0x385dc000, 0x385de000,
    0x385e0000, 0x385e2000, 0x385e4000, 0x385e6000, 0x385e8000, 0x385ea000,
    0x385ec000, 0x385ee000, 0x385f0000, 0x385f2000, 0x385f4000, 0x385f6000,
    0x385f8000, 0x385fa000, 0x385fc000, 0x385fe000, 0x38600000, 0x38602000,
    0x38604000, 0x38606000, 0x38608000, 0x3860a000, 0x3860c000, 0x3860e000,
    0x38610000, 0x38612000, 0x38614000, 0x38616000, 0x38618000, 0x3861a000,
    0x3861c000, 0x3861e000, 0x38620000, 0x38622000, 0x38624000, 0x38626000,
    0x38628000, 0x3862a000, 0x3862c000, 0x3862e000, 0x38630000, 0x38632000,
    0x38634000, 0x38636000, 0x38638000, 0x3863a000, 0x3863c000, 0x3863e000,
    0x38640000, 0x38642000, 0x38644000, 0x38646000, 0x38648000, 0x3864a000,
    0x3864c000, 0x3864e000, 0x38650000, 0x38652000, 0x38654000, 0x38656000,
    0x38658000, 0x3865a000, 0x3865c000, 0x3865e000, 0x38660000, 0x38662000,
    0x38664000, 0x38666000, 0x38668000, 0x3866a000, 0x3866c000, 0x3866e000,
    0x38670000, 0x38672000, 0x38674000, 0x38676000, 0x38678000, 0x3867a000,
    0x3867c000, 0x3867e000, 0x38680000, 0x38682000, 0x38684000, 0x38686000,
    0x38688000, 0x3868a000, 0x3868c000, 0x3868e000, 0x38690000, 0x38692000,
    0x38694000, 0x38696000, 0x38698000, 0x3869a000, 0x3869c000, 0x3869e000,
    0x386a0000, 0x386a2000, 0x386a4000, 0x386a6000, 0x386a8000, 0x386aa000,
    0x386ac000, 0x386ae000, 0x386b0000, 0x386b2000, 0x386b4000, 0x386b6000,
    0x386b8000, 0x386ba000, 0x386bc000, 0x386be000, 0x386c0000, 0x386c2000,
    0x386c4000, 0x386c6000, 0x386c8000, 0x386ca000, 0x386cc000, 0x386ce000,
    0x386d0000, 0x386d2000, 0x386d4000, 0x386d6000, 0x386d8000, 0x386da000,
    0x386dc000, 0x386de000, 0x386e0000, 0x386e2000, 0x386e4000, 0x386e6000,
    0x386e8000, 0x386ea000, 0x386ec000, 0x386ee000, 0x386f0000, 0x386f2000,
    0x386f4000, 0x386f6000, 0x386f8000, 0x386fa000, 0x386fc000, 0x386fe000,
    0x38700000, 0x38702000, 0x38704000, 0x38706000, 0x38708000, 0x3870a000,
    0x3870c000, 0x3870e000, 0x38710000, 0x38712000, 0x38714000, 0x38716000,
    0x38718000, 0x3871a000, 0x3871c000, 0x3871e000, 0x38720000, 0x38722000,
    0x38724000, 0x38726000, 0x38728000, 0x3872a000, 0x3872c000, 0x3872e000,
    0x38730000, 0x38732000, 0x38734000, 0x38736000, 0x38738000, 0x3873a000,
    0x3873c000, 0x3873e000, 0x38740000, 0x38742000, 0x38744000, 0x38746000,
    0x38748000, 0x3874a000, 0x3874c000, 0x3874e000, 0x38750000, 0x38752000,
    0x38754000, 0x38756000, 0x38758000, 0x3875a000, 0x3875c000, 0x3875e000,
    0x38760000, 0x38762000, 0x38764000, 0x38766000, 0x38768000, 0x3876a000,
    0x3876c000, 0x3876e000, 0x38770000, 0x38772000, 0x38774000, 0x38776000,
    0x38778000, 0x3877a000, 0x3877c000, 0x3877e000, 0x38780000, 0x38782000,
    0x38784000, 0x38786000, 0x38788000, 0x3878a000, 0x3878c000, 0x3878e000,
    0x38790000, 0x38792000, 0x38794000, 0x38796000, 0x38798000, 0x3879a000,
    0x3879c000, 0x3879e000, 0x387a0000, 0x387a2000, 0x387a4000, 0x387a6000,
    0x387a8000, 0x387aa000, 0x387ac000, 0x387ae000, 0x387b0000, 0x387b2000,
    0x387b4000, 0x387b6000, 0x387b8000, 0x387ba000, 0x387bc000, 0x387be000,
    0x387c0000, 0x387c2000, 0x387c4000, 0x387c6000, 0x387c8000, 0x387ca000,
    0x387cc000, 0x387ce000, 0x387d0000, 0x387d2000, 0x387d4000, 0x387d6000,
    0x387d8000, 0x387da000, 0x387dc000, 0x387de000, 0x387e0000, 0x387e2000,
    0x387e4000, 0x387e6000, 0x387e8000, 0x387ea000, 0x387ec000, 0x387ee000,
    0x387f0000, 0x387f2000, 0x387f4000, 0x387f6000, 0x387f8000, 0x387fa000,
    0x387fc000, 0x387fe000
};

constexpr UnsignedInt HalfExponentTable[64] = {
    0x00000000, 0x00800000, 0x01000000, 0x01800000, 0x02000000, 0x02800000,
    0x03000000, 0x03800000, 0x04000000, 0x04800000, 0x05000000, 0x05800000,
    0x06000000, 0x06800000, 0x07000000, 0x07800000, 0x08000000, 0x08800000,
    0x09000000, 0x09800000, 0x0a000000, 0x0a800000, 0x0b000000, 0x0b800000,
    0x0c000000, 0x0c800000, 0x0d000000, 0x0d800000, 0x0e000000, 0x0e800000,
    0x0f000000, 0x47800000, 0x80000000, 0x80800000, 0x81000000, 0x81800000,
    0x82000000, 0x82800000, 0x83000000, 0x83800000, 0x84000000, 0x84800000,
    0x85000000, 0x85800000, 0x86000000, 0x86800000, 0x87000000, 0x87800000,
    0x88000000, 0x88800000, 0x89000000, 0x89800000, 0x8a000000, 0x8a800000,
    0x8b000000, 0x8b800000, 0x8c000000, 0x8c800000, 0x8d000000, 0x8d800000,
    0x8e000000, 0x8e800000, 0x8f000000, 0xc7800000
};

constexpr UnsignedShort HalfOffsetTable[64] = {
    0x0000, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400,
    0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400,
    0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400,
    0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0000, 0x0400, 0x0400, 0x0400,
    0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400,
    0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400,
    0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400,
    0x0400
};

constexpr UnsignedShort HalfBaseTable[512] = {
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0001, 0x0002, 0x0004, 0x0008, 0x0010,
    0x0020, 0x0040, 0x0080, 0x0100, 0x0200, 0x0400, 0x0800, 0x0c00, 0x1000,
    0x1400, 0x1800, 0x1c00, 0x2000, 0x2400, 0x2800, 0x2c00, 0x3000, 0x3400,
    0x3800, 0x3c00, 0x4000, 0x4400, 0x4800, 0x4c00, 0x5000, 0x5400, 0x5800,
    0x5c00, 0x6000, 0x6400, 0x6800, 0x6c00, 0x7000, 0x7400, 0x7800, 0x7c00,
    0x7c00, 0x7c00, 0x7c00, 0x7c00, 0x7c00, 0x7c00, 0x7c00, 0x7c00, 0x7c00,
    0x7c00, 0x7c00, 0x7c00, 0x7c00, 0x7c00, 0x7c00, 0x7c00, 0x7c00, 0x7c00,
    0x7c00, 0x7c00, 0x7c00, 0x7c00, 0x7c00, 0x7c00, 0x7c00, 0x7c00, 0x7c00,
    0x7c00, 0x7c00, 0x7c00, 0x7c00, 0x7c00, 0x7c00, 0x7c00, 0x7c00, 0x7c00,
    0x7c00, 0x7c00, 0x7c00, 0x7c00, 0x7c00, 0x7c00, 0x7c00, 0x7c00, 0x7c00,
    0x7c00, 0x7c00, 0x7c00, 0x7c00, 0x7c00, 0x7c00, 0x7c00, 0x7c00, 0x7c00,
    0x7c00, 0x7c00, 0x7c00, 0x7c00, 0x7c00, 0x7c00, 0x7c00, 0x7c00, 0x7c00,
    0x7c00, 0x7c00, 0x7c00, 0x7c00, 0x7c00, 0x7c00, 0x7c00, 0x7c00, 0x7c00,
    0x7c00, 0x7c00, 0x7c00, 0x7c00, 0x7c00, 0x7c00, 0x7c00, 0x7c00, 0x7c00,
    0x7c00, 0x7c00, 0x7c00, 0x7c00, 0x7c00, 0x7c00, 0x7c00, 0x7c00, 0x7c00,
    0x7c00, 0x7c00, 0x7c00, 0x7c00, 0x7c00, 0x7c00, 0x7c00, 0x7c00, 0x7c00,
    0x7c00, 0x7c00, 0x7c00, 0x7c00, 0x7c00, 0x7c00, 0x7c00, 0x7c00, 0x7c00,
    0x7c00, 0x7c00, 0x7c00, 0x7c00, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000,
    0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000,
    0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000,
    0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000,
    0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000,
    0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000,
    0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000,
    0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000,
    0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000,
    0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000,
    0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000,
    0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8001,
    0x8002, 0x8004, 0x8008, 0x8010, 0x8020, 0x8040, 0x8080, 0x8100, 0x8200,
    0x8400, 0x8800, 0x8c00, 0x9000, 0x9400, 0x9800, 0x9c00, 0xa000, 0xa400,
    0xa800, 0xac00, 0xb000, 0xb400, 0xb800, 0xbc00, 0xc000, 0xc400, 0xc800,
    0xcc00, 0xd000, 0xd400, 0xd800, 0xdc00, 0xe000, 0xe400, 0xe800, 0xec00,
    0xf000, 0xf400, 0xf800, 0xfc00, 0xfc00, 0xfc00, 0xfc00, 0xfc00, 0xfc00,
    0xfc00, 0xfc00, 0xfc00, 0xfc00, 0xfc00, 0xfc00, 0xfc00, 0xfc00, 0xfc00,
    0xfc00, 0xfc00, 0xfc00, 0xfc00, 0xfc00, 0xfc00, 0xfc00, 0xfc00, 0xfc00,
    0xfc00, 0xfc00, 0xfc00, 0xfc00, 0xfc00, 0xfc00, 0xfc00, 0xfc00, 0xfc00,
    0xfc00, 0xfc00, 0xfc00, 0xfc00, 0xfc00, 0xfc00, 0xfc00, 0xfc00, 0xfc00,
    0xfc00, 0xfc00, 0xfc00, 0xfc00, 0xfc00, 0xfc00, 0xfc00, 0xfc00, 0xfc00,
    0xfc00, 0xfc00, 0xfc00, 0xfc00, 0xfc00, 0xfc00, 0xfc00, 0xfc00, 0xfc00,
    0xfc00, 0xfc00, 0xfc00, 0xfc00, 0xfc00, 0xfc00, 0xfc00, 0xfc00, 0xfc00,
    0xfc00, 0xfc00, 0xfc00, 0xfc00, 0xfc00, 0xfc00, 0xfc00, 0xfc00, 0xfc00,
    0xfc00, 0xfc00, 0xfc00, 0xfc00, 0xfc00, 0xfc00, 0xfc00, 0xfc00, 0xfc00,
    0xfc00, 0xfc00, 0xfc00, 0xfc00, 0xfc00, 0xfc00, 0xfc00, 0xfc00, 0xfc00,
    0xfc00, 0xfc00, 0xfc00, 0xfc00, 0xfc00, 0xfc00, 0xfc00, 0xfc00, 0xfc00,
    0xfc00, 0xfc00, 0xfc00, 0xfc00, 0xfc00, 0xfc00, 0xfc00, 0xfc00
};

constexpr UnsignedByte HalfShiftTable[512] = {
    0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
    0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
    0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
    0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
    0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
    0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
    0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
    0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
    0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x17, 0x16, 0x15, 0x14, 0x13,
    0x12, 0x11, 0x10, 0x0f, 0x0e, 0x0d, 0x0d, 0x0d, 0x0d, 0x0d, 0x0d, 0x0d,
    0x0d, 0x0d, 0x0d, 0x0d, 0x0d, 0x0d, 0x0d, 0x0d, 0x0d, 0x0d, 0x0d, 0x0d,
    0x0d, 0x0d, 0x0d, 0x0d, 0x0d, 0x0d, 0x0d, 0x0d, 0x0d, 0x0d, 0x0d, 0x18,
    0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
    0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
    0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
    0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
    0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
    0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
    0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
    0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
    0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
    0x18, 0x18, 0x18, 0x0d, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
    0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
    0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
    0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
    0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
    0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
    0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
    0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
    0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x17,
    0x16, 0x15, 0x14, 0x13, 0x12, 0x11, 0x10, 0x0f, 0x0e, 0x0d, 0x0d, 0x0d,
    0x0d, 0x0d, 0x0d, 0x0d, 0x0d, 0x0d, 0x0d, 0x0d, 0x0d, 0x0d, 0x0d, 0x0d,
    0x0d, 0x0d, 0x0d, 0x0d, 0x0d, 0x0d, 0x0d, 0x0d, 0x0d, 0x0d, 0x0d, 0x0d,
    0x0d, 0x0d, 0x0d, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
    0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
    0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
    0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
    0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
    0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
    0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
    0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
    0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
    0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x0d
};

}}}
namespace Magnum { namespace Math {

namespace {

template<class T> inline void unpackUnsignedIntoImplementation(const Containers::StridedArrayView2D<const T>& src, const Containers::StridedArrayView2D<Float>& dst) {
    CORRADE_ASSERT(src.size() == dst.size(),
        "Math::unpackInto(): wrong destination size, got" << dst.size() << "but expected" << src.size(), );
    CORRADE_ASSERT(src.template isContiguous<1>(),
        "Math::unpackInto(): second source view dimension is not contiguous", );
    CORRADE_ASSERT(dst.isContiguous<1>(),
        "Math::unpackInto(): second destination view dimension is not contiguous", );

    constexpr Float bitMax = Implementation::bitMax<T>();
    const char* srcPtr = reinterpret_cast<const char*>(src.data());
    char* dstPtr = reinterpret_cast<char*>(dst.data());
    const std::ptrdiff_t srcStride = src.stride()[0];
    const std::ptrdiff_t dstStride = dst.stride()[0];
    const std::size_t maxJ = src.size()[1];
    for(std::size_t i = 0, maxI = src.size()[0]; i != maxI; ++i) {
        const T* srcPtrI = reinterpret_cast<const T*>(srcPtr);
        Float* dstPtrI = reinterpret_cast<Float*>(dstPtr);
        for(std::size_t j = 0; j != maxJ; ++j)
            *dstPtrI++ = *srcPtrI++/bitMax;

        srcPtr += srcStride;
        dstPtr += dstStride;
    }
}

}

void unpackInto(const Containers::StridedArrayView2D<const UnsignedByte>& src, const Containers::StridedArrayView2D<Float>& dst) {
    unpackUnsignedIntoImplementation(src, dst);
}

void unpackInto(const Containers::StridedArrayView2D<const UnsignedShort>& src, const Containers::StridedArrayView2D<Float>& dst) {
    unpackUnsignedIntoImplementation(src, dst);
}

namespace {

template<class T> inline void unpackSignedIntoImplementation(const Containers::StridedArrayView2D<const T>& src, const Containers::StridedArrayView2D<Float>& dst) {
    CORRADE_ASSERT(src.size() == dst.size(),
        "Math::unpackInto(): wrong destination size, got" << dst.size() << "but expected" << src.size(), );
    CORRADE_ASSERT(src.template isContiguous<1>(),
        "Math::unpackInto(): second source view dimension is not contiguous", );
    CORRADE_ASSERT(dst.isContiguous<1>(),
        "Math::unpackInto(): second destination view dimension is not contiguous", );

    constexpr Float bitMax = Implementation::bitMax<T>();
    const char* srcPtr = reinterpret_cast<const char*>(src.data());
    char* dstPtr = reinterpret_cast<char*>(dst.data());
    const std::ptrdiff_t srcStride = src.stride()[0];
    const std::ptrdiff_t dstStride = dst.stride()[0];
    const std::size_t maxJ = src.size()[1];
    for(std::size_t i = 0, maxI = src.size()[0]; i != maxI; ++i) {
        const T* srcPtrI = reinterpret_cast<const T*>(srcPtr);
        Float* dstPtrI = reinterpret_cast<Float*>(dstPtr);
        for(std::size_t j = 0; j != maxJ; ++j) {
            const Float value = *srcPtrI++/bitMax;
            *dstPtrI++ = value < -1.0f ? -1.0f : value;
        }

        srcPtr += srcStride;
        dstPtr += dstStride;
    }
}

}

void unpackInto(const Containers::StridedArrayView2D<const Byte>& src, const Containers::StridedArrayView2D<Float>& dst) {
    unpackSignedIntoImplementation(src, dst);
}

void unpackInto(const Containers::StridedArrayView2D<const Short>& src, const Containers::StridedArrayView2D<Float>& dst) {
    unpackSignedIntoImplementation(src, dst);
}

namespace {

template<class T> inline void packIntoImplementation(const Containers::StridedArrayView2D<const Float>& src, const Containers::StridedArrayView2D<T>& dst) {
    CORRADE_ASSERT(src.size() == dst.size(),
        "Math::packInto(): wrong destination size, got" << dst.size() << "but expected" << src.size(), );
    CORRADE_ASSERT(src.isContiguous<1>(),
        "Math::packInto(): second source view dimension is not contiguous", );
    CORRADE_ASSERT(dst.template isContiguous<1>(),
        "Math::packInto(): second destination view dimension is not contiguous", );

    constexpr Float bitMax = Implementation::bitMax<T>();
    const char* srcPtr = reinterpret_cast<const char*>(src.data());
    char* dstPtr = reinterpret_cast<char*>(dst.data());
    const std::ptrdiff_t srcStride = src.stride()[0];
    const std::ptrdiff_t dstStride = dst.stride()[0];
    const std::size_t maxJ = src.size()[1];
    for(std::size_t i = 0, maxI = src.size()[0]; i != maxI; ++i) {
        const Float* srcPtrI = reinterpret_cast<const Float*>(srcPtr);
        T* dstPtrI = reinterpret_cast<T*>(dstPtr);
        for(std::size_t j = 0; j != maxJ; ++j)
            *dstPtrI++ = std::round(*srcPtrI++*bitMax);

        srcPtr += srcStride;
        dstPtr += dstStride;
    }
}

}

void packInto(const Containers::StridedArrayView2D<const Float>& src, const Containers::StridedArrayView2D<UnsignedByte>& dst) {
    packIntoImplementation(src, dst);
}

void packInto(const Containers::StridedArrayView2D<const Float>& src, const Containers::StridedArrayView2D<UnsignedShort>& dst) {
    packIntoImplementation(src, dst);
}

void packInto(const Containers::StridedArrayView2D<const Float>& src, const Containers::StridedArrayView2D<Byte>& dst) {
    packIntoImplementation(src, dst);
}

void packInto(const Containers::StridedArrayView2D<const Float>& src, const Containers::StridedArrayView2D<Short>& dst) {
    packIntoImplementation(src, dst);
}

namespace {

template<class T, class U> inline void castIntoImplementation(const Containers::StridedArrayView2D<const T>& src, const Containers::StridedArrayView2D<U>& dst) {
    CORRADE_ASSERT(src.size() == dst.size(),
        "Math::castInto(): wrong destination size, got" << dst.size() << "but expected" << src.size(), );
    CORRADE_ASSERT(src.template isContiguous<1>(),
        "Math::castInto(): second source view dimension is not contiguous", );
    CORRADE_ASSERT(dst.template isContiguous<1>(),
        "Math::castInto(): second destination view dimension is not contiguous", );

    const char* srcPtr = reinterpret_cast<const char*>(src.data());
    char* dstPtr = reinterpret_cast<char*>(dst.data());
    const std::ptrdiff_t srcStride = src.stride()[0];
    const std::ptrdiff_t dstStride = dst.stride()[0];
    const std::size_t maxJ = src.size()[1];
    for(std::size_t i = 0, maxI = src.size()[0]; i != maxI; ++i) {
        const T* srcPtrI = reinterpret_cast<const T*>(srcPtr);
        U* dstPtrI = reinterpret_cast<U*>(dstPtr);
        for(std::size_t j = 0; j != maxJ; ++j)
            *dstPtrI++ = U(*srcPtrI++);

        srcPtr += srcStride;
        dstPtr += dstStride;
    }
}

}

void castInto(const Containers::StridedArrayView2D<const UnsignedByte>& src, const Containers::StridedArrayView2D<Float>& dst) {
    castIntoImplementation(src, dst);
}

void castInto(const Containers::StridedArrayView2D<const Byte>& src, const Containers::StridedArrayView2D<Float>& dst) {
    castIntoImplementation(src, dst);
}

void castInto(const Containers::StridedArrayView2D<const UnsignedShort>& src, const Containers::StridedArrayView2D<Float>& dst) {
    castIntoImplementation(src, dst);
}

void castInto(const Containers::StridedArrayView2D<const Short>& src, const Containers::StridedArrayView2D<Float>& dst) {
    castIntoImplementation(src, dst);
}

void castInto(const Containers::StridedArrayView2D<const UnsignedInt>& src, const Containers::StridedArrayView2D<Float>& dst) {
    castIntoImplementation(src, dst);
}

void castInto(const Containers::StridedArrayView2D<const Int>& src, const Containers::StridedArrayView2D<Float>& dst) {
    castIntoImplementation(src, dst);
}

void castInto(const Containers::StridedArrayView2D<const UnsignedByte>& src, const Containers::StridedArrayView2D<Double>& dst) {
    castIntoImplementation(src, dst);
}

void castInto(const Containers::StridedArrayView2D<const Byte>& src, const Containers::StridedArrayView2D<Double>& dst) {
    castIntoImplementation(src, dst);
}

void castInto(const Containers::StridedArrayView2D<const UnsignedShort>& src, const Containers::StridedArrayView2D<Double>& dst) {
    castIntoImplementation(src, dst);
}

void castInto(const Containers::StridedArrayView2D<const Short>& src, const Containers::StridedArrayView2D<Double>& dst) {
    castIntoImplementation(src, dst);
}

void castInto(const Containers::StridedArrayView2D<const UnsignedInt>& src, const Containers::StridedArrayView2D<Double>& dst) {
    castIntoImplementation(src, dst);
}

void castInto(const Containers::StridedArrayView2D<const Int>& src, const Containers::StridedArrayView2D<Double>& dst) {
    castIntoImplementation(src, dst);
}

void castInto(const Containers::StridedArrayView2D<const Float>& src, const Containers::StridedArrayView2D<UnsignedByte>& dst) {
    castIntoImplementation(src, dst);
}

void castInto(const Containers::StridedArrayView2D<const Float>& src, const Containers::StridedArrayView2D<Byte>& dst) {
    castIntoImplementation(src, dst);
}

void castInto(const Containers::StridedArrayView2D<const Float>& src, const Containers::StridedArrayView2D<UnsignedShort>& dst) {
    castIntoImplementation(src, dst);
}

void castInto(const Containers::StridedArrayView2D<const Float>& src, const Containers::StridedArrayView2D<Short>& dst) {
    castIntoImplementation(src, dst);
}

void castInto(const Containers::StridedArrayView2D<const Float>& src, const Containers::StridedArrayView2D<UnsignedInt>& dst) {
    castIntoImplementation(src, dst);
}

void castInto(const Containers::StridedArrayView2D<const Float>& src, const Containers::StridedArrayView2D<Int>& dst) {
    castIntoImplementation(src, dst);
}

void castInto(const Containers::StridedArrayView2D<const Double>& src, const Containers::StridedArrayView2D<UnsignedByte>& dst) {
    castIntoImplementation(src, dst);
}

void castInto(const Containers::StridedArrayView2D<const Double>& src, const Containers::StridedArrayView2D<Byte>& dst) {
    castIntoImplementation(src, dst);
}

void castInto(const Containers::StridedArrayView2D<const Double>& src, const Containers::StridedArrayView2D<UnsignedShort>& dst) {
    castIntoImplementation(src, dst);
}

void castInto(const Containers::StridedArrayView2D<const Double>& src, const Containers::StridedArrayView2D<Short>& dst) {
    castIntoImplementation(src, dst);
}

void castInto(const Containers::StridedArrayView2D<const Double>& src, const Containers::StridedArrayView2D<UnsignedInt>& dst) {
    castIntoImplementation(src, dst);
}

void castInto(const Containers::StridedArrayView2D<const Double>& src, const Containers::StridedArrayView2D<Int>& dst) {
    castIntoImplementation(src, dst);
}

void castInto(const Containers::StridedArrayView2D<const UnsignedByte>& src, const Containers::StridedArrayView2D<UnsignedShort>& dst) {
    castIntoImplementation(src, dst);
}

void castInto(const Containers::StridedArrayView2D<const Byte>& src, const Containers::StridedArrayView2D<Short>& dst) {
    castIntoImplementation(src, dst);
}

void castInto(const Containers::StridedArrayView2D<const UnsignedByte>& src, const Containers::StridedArrayView2D<UnsignedInt>& dst) {
    castIntoImplementation(src, dst);
}

void castInto(const Containers::StridedArrayView2D<const Byte>& src, const Containers::StridedArrayView2D<Int>& dst) {
    castIntoImplementation(src, dst);
}

void castInto(const Containers::StridedArrayView2D<const UnsignedShort>& src, const Containers::StridedArrayView2D<UnsignedInt>& dst) {
    castIntoImplementation(src, dst);
}

void castInto(const Containers::StridedArrayView2D<const Short>& src, const Containers::StridedArrayView2D<Int>& dst) {
    castIntoImplementation(src, dst);
}

void castInto(const Containers::StridedArrayView2D<const UnsignedByte>& src, const Containers::StridedArrayView2D<UnsignedLong>& dst) {
    castIntoImplementation(src, dst);
}

void castInto(const Containers::StridedArrayView2D<const Byte>& src, const Containers::StridedArrayView2D<Long>& dst) {
    castIntoImplementation(src, dst);
}

void castInto(const Containers::StridedArrayView2D<const UnsignedShort>& src, const Containers::StridedArrayView2D<UnsignedLong>& dst) {
    castIntoImplementation(src, dst);
}

void castInto(const Containers::StridedArrayView2D<const Short>& src, const Containers::StridedArrayView2D<Long>& dst) {
    castIntoImplementation(src, dst);
}

void castInto(const Containers::StridedArrayView2D<const UnsignedInt>& src, const Containers::StridedArrayView2D<UnsignedLong>& dst) {
    castIntoImplementation(src, dst);
}

void castInto(const Containers::StridedArrayView2D<const Int>& src, const Containers::StridedArrayView2D<Long>& dst) {
    castIntoImplementation(src, dst);
}

void castInto(const Containers::StridedArrayView2D<const UnsignedLong>& src, const Containers::StridedArrayView2D<UnsignedInt>& dst) {
    castIntoImplementation(src, dst);
}

void castInto(const Containers::StridedArrayView2D<const Long>& src, const Containers::StridedArrayView2D<Int>& dst) {
    castIntoImplementation(src, dst);
}

void castInto(const Containers::StridedArrayView2D<const UnsignedLong>& src, const Containers::StridedArrayView2D<UnsignedShort>& dst) {
    castIntoImplementation(src, dst);
}

void castInto(const Containers::StridedArrayView2D<const Long>& src, const Containers::StridedArrayView2D<Short>& dst) {
    castIntoImplementation(src, dst);
}

void castInto(const Containers::StridedArrayView2D<const UnsignedLong>& src, const Containers::StridedArrayView2D<UnsignedByte>& dst) {
    castIntoImplementation(src, dst);
}

void castInto(const Containers::StridedArrayView2D<const Long>& src, const Containers::StridedArrayView2D<Byte>& dst) {
    castIntoImplementation(src, dst);
}

void castInto(const Containers::StridedArrayView2D<const UnsignedInt>& src, const Containers::StridedArrayView2D<UnsignedShort>& dst) {
    castIntoImplementation(src, dst);
}

void castInto(const Containers::StridedArrayView2D<const Int>& src, const Containers::StridedArrayView2D<Short>& dst) {
    castIntoImplementation(src, dst);
}

void castInto(const Containers::StridedArrayView2D<const UnsignedInt>& src, const Containers::StridedArrayView2D<UnsignedByte>& dst) {
    castIntoImplementation(src, dst);
}

void castInto(const Containers::StridedArrayView2D<const Int>& src, const Containers::StridedArrayView2D<Byte>& dst) {
    castIntoImplementation(src, dst);
}

void castInto(const Containers::StridedArrayView2D<const UnsignedShort>& src, const Containers::StridedArrayView2D<UnsignedByte>& dst) {
    castIntoImplementation(src, dst);
}

void castInto(const Containers::StridedArrayView2D<const Short>& src, const Containers::StridedArrayView2D<Byte>& dst) {
    castIntoImplementation(src, dst);
}

void castInto(const Containers::StridedArrayView2D<const Float>& src, const Containers::StridedArrayView2D<Double>& dst) {
    castIntoImplementation(src, dst);
}

void castInto(const Containers::StridedArrayView2D<const Double>& src, const Containers::StridedArrayView2D<Float>& dst) {
    castIntoImplementation(src, dst);
}

namespace {

template<class T> inline void copyImplementation(const Containers::StridedArrayView2D<const T>& src, const Containers::StridedArrayView2D<T>& dst) {
    castIntoImplementation(src, dst);
}

}

void castInto(const Containers::StridedArrayView2D<const UnsignedByte>& src, const Containers::StridedArrayView2D<UnsignedByte>& dst) {
    copyImplementation(src, dst);
}

void castInto(const Containers::StridedArrayView2D<const Byte>& src, const Containers::StridedArrayView2D<Byte>& dst) {
    copyImplementation(src, dst);
}

void castInto(const Containers::StridedArrayView2D<const UnsignedShort>& src, const Containers::StridedArrayView2D<UnsignedShort>& dst) {
    copyImplementation(src, dst);
}

void castInto(const Containers::StridedArrayView2D<const Short>& src, const Containers::StridedArrayView2D<Short>& dst) {
    copyImplementation(src, dst);
}

void castInto(const Containers::StridedArrayView2D<const UnsignedInt>& src, const Containers::StridedArrayView2D<UnsignedInt>& dst) {
    copyImplementation(src, dst);
}

void castInto(const Containers::StridedArrayView2D<const Int>& src, const Containers::StridedArrayView2D<Int>& dst) {
    copyImplementation(src, dst);
}

void castInto(const Containers::StridedArrayView2D<const UnsignedLong>& src, const Containers::StridedArrayView2D<UnsignedLong>& dst) {
    copyImplementation(src, dst);
}

void castInto(const Containers::StridedArrayView2D<const Long>& src, const Containers::StridedArrayView2D<Long>& dst) {
    copyImplementation(src, dst);
}

void castInto(const Containers::StridedArrayView2D<const Float>& src, const Containers::StridedArrayView2D<Float>& dst) {
    copyImplementation(src, dst);
}

void castInto(const Containers::StridedArrayView2D<const Double>& src, const Containers::StridedArrayView2D<Double>& dst) {
    copyImplementation(src, dst);
}

static_assert(sizeof(HalfMantissaTable) + sizeof(HalfOffsetTable) + sizeof(HalfExponentTable) == 8576,
    "improper size of half->float conversion tables");

static_assert(sizeof(HalfBaseTable) + sizeof(HalfShiftTable) == 1536,
    "improper size of float->half conversion tables");

void unpackHalfInto(const Containers::StridedArrayView2D<const UnsignedShort>& src, const Containers::StridedArrayView2D<Float>& dst) {
    CORRADE_ASSERT(src.size() == dst.size(),
        "Math::unpackHalfInto(): wrong destination size, got" << dst.size() << "but expected" << src.size(), );
    CORRADE_ASSERT(src.isContiguous<1>(),
        "Math::unpackHalfInto(): second source view dimension is not contiguous", );
    CORRADE_ASSERT(dst.isContiguous<1>(),
        "Math::unpackHalfInto(): second destination view dimension is not contiguous", );

    const char* srcPtr = reinterpret_cast<const char*>(src.data());
    char* dstPtr = reinterpret_cast<char*>(dst.data());
    const std::ptrdiff_t srcStride = src.stride()[0];
    const std::ptrdiff_t dstStride = dst.stride()[0];
    const std::size_t maxJ = src.size()[1];
    for(std::size_t i = 0, maxI = src.size()[0]; i != maxI; ++i) {
        const UnsignedShort* srcPtrI = reinterpret_cast<const UnsignedShort*>(srcPtr);
        UnsignedInt* dstPtrI = reinterpret_cast<UnsignedInt*>(dstPtr);
        for(std::size_t j = 0; j != maxJ; ++j) {
            const UnsignedShort h = *srcPtrI++;
            *dstPtrI++ = HalfMantissaTable[HalfOffsetTable[h >> 10] + (h & 0x3ff)] + HalfExponentTable[h >> 10];
        }

        srcPtr += srcStride;
        dstPtr += dstStride;
    }
}

void packHalfInto(const Containers::StridedArrayView2D<const Float>& src, const Containers::StridedArrayView2D<UnsignedShort>& dst) {
    CORRADE_ASSERT(src.size() == dst.size(),
        "Math::packHalfInto(): wrong destination size, got" << dst.size() << "but expected" << src.size(), );
    CORRADE_ASSERT(src.isContiguous<1>(),
        "Math::packHalfInto(): second source view dimension is not contiguous", );
    CORRADE_ASSERT(dst.isContiguous<1>(),
        "Math::packHalfInto(): second destination view dimension is not contiguous", );

    const char* srcPtr = reinterpret_cast<const char*>(src.data());
    char* dstPtr = reinterpret_cast<char*>(dst.data());
    const std::ptrdiff_t srcStride = src.stride()[0];
    const std::ptrdiff_t dstStride = dst.stride()[0];
    const std::size_t maxJ = src.size()[1];
    for(std::size_t i = 0, maxI = src.size()[0]; i != maxI; ++i) {
        const UnsignedInt* srcPtrI = reinterpret_cast<const UnsignedInt*>(srcPtr);
        UnsignedShort* dstPtrI = reinterpret_cast<UnsignedShort*>(dstPtr);
        for(std::size_t j = 0; j != maxJ; ++j) {
            const UnsignedInt f = *srcPtrI++;
            *dstPtrI++ = HalfBaseTable[(f >> 23) & 0x1ff] + ((f & 0x007fffff) >> HalfShiftTable[(f >> 23) & 0x1ff]);
        }

        srcPtr += srcStride;
        dstPtr += dstStride;
    }
}

}}
#endif
