/*
    Magnum::MeshTools
        — algorithms for dealing with mesh data

    https://doc.magnum.graphics/magnum/namespaceMagnum_1_1MeshTools.html

    Depends on CorradeArray.h and MagnumMathBatch.hpp.

    This is a single-header library generated from the Magnum project. With the
    goal being easy integration, it's deliberately free of all comments to keep
    the file size small. More info, changelogs and full docs here:

    -   Project homepage — https://magnum.graphics/magnum/
    -   Documentation — https://doc.magnum.graphics/
    -   GitHub project page — https://github.com/mosra/magnum
    -   GitHub Singles repository — https://github.com/mosra/magnum-singles

    The library has a separate non-inline implementation part, enable it *just
    once* like this:

        #define MAGNUM_MESHTOOLS_IMPLEMENTATION
        #include <MagnumMeshTools.hpp>

    If you need the deinlined symbols to be exported from a shared library,
    `#define MAGNUM_MESHTOOLS_EXPORT` as appropriate.

    v2020.06-3128-g47b22 (2025-01-07)
    -   Initial release

    Generated from Corrade v2020.06-1847-g9be0 (2025-01-07) and
        Magnum v2020.06-3128-g47b22 (2025-01-07), 765 / 283 LoC
*/

/*
    This file is part of Magnum.

    Copyright © 2007, 2008, 2009, 2010, 2011, 2012, 2013, 2014, 2015, 2016,
                2017, 2018, 2019, 2020, 2021, 2022, 2023, 2024, 2025
              Vladimír Vondruš <mosra@centrum.cz>
    Copyright © 2022 Pablo Escobar <mail@rvrs.in>

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
#include <cstdint>

#ifndef MAGNUM_MESHTOOLS_EXPORT
#define MAGNUM_MESHTOOLS_EXPORT
#endif

#ifndef Magnum_Types_h
#define Magnum_Types_h

namespace Magnum {

typedef std::uint8_t UnsignedByte;
typedef std::int8_t Byte;
typedef std::uint16_t UnsignedShort;
typedef std::int16_t Short;
typedef std::uint32_t UnsignedInt;
typedef std::int32_t Int;
typedef std::uint64_t UnsignedLong;
typedef std::int64_t Long;

typedef float Float;
typedef double Double;

}

#endif
#ifndef MagnumMeshTools_hpp
#define MagnumMeshTools_hpp
namespace Corrade { namespace Containers {

template<class, class> class Pair;
template<unsigned, class> class StridedArrayView;
template<class T> using StridedArrayView1D = StridedArrayView<1, T>;
template<class T> using StridedArrayView2D = StridedArrayView<2, T>;

}}
namespace Magnum {

using namespace Corrade;

namespace Math {
    template<class> class Vector3;
    template<class> class Range3D;
}

typedef Math::Vector3<Float> Vector3;
typedef Math::Range3D<Float> Range3D;

}
#endif
#ifndef CorradeArray_h
#define CorradeArray_h
namespace Corrade { namespace Containers {

#ifndef Corrade_Containers_Containers_h
template<class T, class = void(*)(T*, std::size_t)> class Array;
#endif
template<std::size_t, class> class StaticArray;
template<class T> using Array2 = StaticArray<2, T>;
template<class T> using Array3 = StaticArray<3, T>;
template<class T> using Array4 = StaticArray<4, T>;

}}
#endif
#ifndef Magnum_MeshTools_BoundingVolume_h
#define Magnum_MeshTools_BoundingVolume_h

namespace Magnum { namespace MeshTools {

MAGNUM_MESHTOOLS_EXPORT Range3D boundingRange(const Containers::StridedArrayView1D<const Vector3>& positions);

MAGNUM_MESHTOOLS_EXPORT Containers::Pair<Vector3, Float> boundingSphereBouncingBubble(const Containers::StridedArrayView1D<const Vector3>& positions);

}}

#endif
#ifndef Magnum_MeshTools_GenerateIndices_h
#define Magnum_MeshTools_GenerateIndices_h

namespace Magnum { namespace MeshTools {

MAGNUM_MESHTOOLS_EXPORT Containers::Array<UnsignedInt> generateTrivialIndices(UnsignedInt vertexCount, UnsignedInt offset = 0);

MAGNUM_MESHTOOLS_EXPORT void generateTrivialIndicesInto(const Containers::StridedArrayView1D<UnsignedInt>& output, UnsignedInt offset = 0);

MAGNUM_MESHTOOLS_EXPORT Containers::Array<UnsignedInt> generateLineStripIndices(UnsignedInt vertexCount, UnsignedInt offset = 0);

MAGNUM_MESHTOOLS_EXPORT Containers::Array<UnsignedInt> generateLineStripIndices(const Containers::StridedArrayView1D<const UnsignedInt>& indices, UnsignedInt offset = 0);

MAGNUM_MESHTOOLS_EXPORT Containers::Array<UnsignedInt> generateLineStripIndices(const Containers::StridedArrayView1D<const UnsignedShort>& indices, UnsignedInt offset = 0);

MAGNUM_MESHTOOLS_EXPORT Containers::Array<UnsignedInt> generateLineStripIndices(const Containers::StridedArrayView1D<const UnsignedByte>& indices, UnsignedInt offset = 0);

MAGNUM_MESHTOOLS_EXPORT Containers::Array<UnsignedInt> generateLineStripIndices(const Containers::StridedArrayView2D<const char>& indices, UnsignedInt offset = 0);

MAGNUM_MESHTOOLS_EXPORT void generateLineStripIndicesInto(UnsignedInt vertexCount, const Containers::StridedArrayView1D<UnsignedInt>& output, UnsignedInt offset = 0);

MAGNUM_MESHTOOLS_EXPORT void generateLineStripIndicesInto(const Containers::StridedArrayView1D<const UnsignedInt>& indices, const Containers::StridedArrayView1D<UnsignedInt>& output, UnsignedInt offset = 0);

MAGNUM_MESHTOOLS_EXPORT void generateLineStripIndicesInto(const Containers::StridedArrayView1D<const UnsignedShort>& indices, const Containers::StridedArrayView1D<UnsignedInt>& output, UnsignedInt offset = 0);

MAGNUM_MESHTOOLS_EXPORT void generateLineStripIndicesInto(const Containers::StridedArrayView1D<const UnsignedByte>& indices, const Containers::StridedArrayView1D<UnsignedInt>& output, UnsignedInt offset = 0);

MAGNUM_MESHTOOLS_EXPORT void generateLineStripIndicesInto(const Containers::StridedArrayView2D<const char>& indices, const Containers::StridedArrayView1D<UnsignedInt>& output, UnsignedInt offset = 0);

MAGNUM_MESHTOOLS_EXPORT Containers::Array<UnsignedInt> generateLineLoopIndices(UnsignedInt vertexCount, UnsignedInt offset = 0);

MAGNUM_MESHTOOLS_EXPORT Containers::Array<UnsignedInt> generateLineLoopIndices(const Containers::StridedArrayView1D<const UnsignedInt>& indices, UnsignedInt offset = 0);

MAGNUM_MESHTOOLS_EXPORT Containers::Array<UnsignedInt> generateLineLoopIndices(const Containers::StridedArrayView1D<const UnsignedShort>& indices, UnsignedInt offset = 0);

MAGNUM_MESHTOOLS_EXPORT Containers::Array<UnsignedInt> generateLineLoopIndices(const Containers::StridedArrayView1D<const UnsignedByte>& indices, UnsignedInt offset = 0);

MAGNUM_MESHTOOLS_EXPORT Containers::Array<UnsignedInt> generateLineLoopIndices(const Containers::StridedArrayView2D<const char>& indices, UnsignedInt offset = 0);

MAGNUM_MESHTOOLS_EXPORT void generateLineLoopIndicesInto(UnsignedInt vertexCount, const Containers::StridedArrayView1D<UnsignedInt>& output, UnsignedInt offset = 0);

MAGNUM_MESHTOOLS_EXPORT void generateLineLoopIndicesInto(const Containers::StridedArrayView1D<const UnsignedInt>& indices, const Containers::StridedArrayView1D<UnsignedInt>& output, UnsignedInt offset = 0);

MAGNUM_MESHTOOLS_EXPORT void generateLineLoopIndicesInto(const Containers::StridedArrayView1D<const UnsignedShort>& indices, const Containers::StridedArrayView1D<UnsignedInt>& output, UnsignedInt offset = 0);

MAGNUM_MESHTOOLS_EXPORT void generateLineLoopIndicesInto(const Containers::StridedArrayView1D<const UnsignedByte>& indices, const Containers::StridedArrayView1D<UnsignedInt>& output, UnsignedInt offset = 0);

MAGNUM_MESHTOOLS_EXPORT void generateLineLoopIndicesInto(const Containers::StridedArrayView2D<const char>& indices, const Containers::StridedArrayView1D<UnsignedInt>& output, UnsignedInt offset = 0);

MAGNUM_MESHTOOLS_EXPORT Containers::Array<UnsignedInt> generateTriangleStripIndices(UnsignedInt vertexCount, UnsignedInt offset = 0);

MAGNUM_MESHTOOLS_EXPORT Containers::Array<UnsignedInt> generateTriangleStripIndices(const Containers::StridedArrayView1D<const UnsignedInt>& indices, UnsignedInt offset = 0);

MAGNUM_MESHTOOLS_EXPORT Containers::Array<UnsignedInt> generateTriangleStripIndices(const Containers::StridedArrayView1D<const UnsignedShort>& indices, UnsignedInt offset = 0);

MAGNUM_MESHTOOLS_EXPORT Containers::Array<UnsignedInt> generateTriangleStripIndices(const Containers::StridedArrayView1D<const UnsignedByte>& indices, UnsignedInt offset = 0);

MAGNUM_MESHTOOLS_EXPORT Containers::Array<UnsignedInt> generateTriangleStripIndices(const Containers::StridedArrayView2D<const char>& indices, UnsignedInt offset = 0);

MAGNUM_MESHTOOLS_EXPORT void generateTriangleStripIndicesInto(UnsignedInt vertexCount, const Containers::StridedArrayView1D<UnsignedInt>& output, UnsignedInt offset = 0);

MAGNUM_MESHTOOLS_EXPORT void generateTriangleStripIndicesInto(const Containers::StridedArrayView1D<const UnsignedInt>& indices, const Containers::StridedArrayView1D<UnsignedInt>& output, UnsignedInt offset = 0);

MAGNUM_MESHTOOLS_EXPORT void generateTriangleStripIndicesInto(const Containers::StridedArrayView1D<const UnsignedShort>& indices, const Containers::StridedArrayView1D<UnsignedInt>& output, UnsignedInt offset = 0);

MAGNUM_MESHTOOLS_EXPORT void generateTriangleStripIndicesInto(const Containers::StridedArrayView1D<const UnsignedByte>& indices, const Containers::StridedArrayView1D<UnsignedInt>& output, UnsignedInt offset = 0);

MAGNUM_MESHTOOLS_EXPORT void generateTriangleStripIndicesInto(const Containers::StridedArrayView2D<const char>& indices, const Containers::StridedArrayView1D<UnsignedInt>& output, UnsignedInt offset = 0);

MAGNUM_MESHTOOLS_EXPORT Containers::Array<UnsignedInt> generateTriangleFanIndices(UnsignedInt vertexCount, UnsignedInt offset = 0);

MAGNUM_MESHTOOLS_EXPORT Containers::Array<UnsignedInt> generateTriangleFanIndices(const Containers::StridedArrayView1D<const UnsignedInt>& indices, UnsignedInt offset = 0);

MAGNUM_MESHTOOLS_EXPORT Containers::Array<UnsignedInt> generateTriangleFanIndices(const Containers::StridedArrayView1D<const UnsignedShort>& indices, UnsignedInt offset = 0);

MAGNUM_MESHTOOLS_EXPORT Containers::Array<UnsignedInt> generateTriangleFanIndices(const Containers::StridedArrayView1D<const UnsignedByte>& indices, UnsignedInt offset = 0);

MAGNUM_MESHTOOLS_EXPORT Containers::Array<UnsignedInt> generateTriangleFanIndices(const Containers::StridedArrayView2D<const char>& indices, UnsignedInt offset = 0);

MAGNUM_MESHTOOLS_EXPORT void generateTriangleFanIndicesInto(UnsignedInt vertexCount, const Containers::StridedArrayView1D<UnsignedInt>& output, UnsignedInt offset = 0);

MAGNUM_MESHTOOLS_EXPORT void generateTriangleFanIndicesInto(const Containers::StridedArrayView1D<const UnsignedInt>& indices, const Containers::StridedArrayView1D<UnsignedInt>& output, UnsignedInt offset = 0);

MAGNUM_MESHTOOLS_EXPORT void generateTriangleFanIndicesInto(const Containers::StridedArrayView1D<const UnsignedShort>& indices, const Containers::StridedArrayView1D<UnsignedInt>& output, UnsignedInt offset = 0);

MAGNUM_MESHTOOLS_EXPORT void generateTriangleFanIndicesInto(const Containers::StridedArrayView1D<const UnsignedByte>& indices, const Containers::StridedArrayView1D<UnsignedInt>& output, UnsignedInt offset = 0);

MAGNUM_MESHTOOLS_EXPORT void generateTriangleFanIndicesInto(const Containers::StridedArrayView2D<const char>& indices, const Containers::StridedArrayView1D<UnsignedInt>& output, UnsignedInt offset = 0);

MAGNUM_MESHTOOLS_EXPORT Containers::Array<UnsignedInt> generateQuadIndices(const Containers::StridedArrayView1D<const Vector3>& positions, const Containers::StridedArrayView1D<const UnsignedInt>& quads, UnsignedInt offset = 0);

MAGNUM_MESHTOOLS_EXPORT Containers::Array<UnsignedInt> generateQuadIndices(const Containers::StridedArrayView1D<const Vector3>& positions, const Containers::StridedArrayView1D<const UnsignedShort>& quads, UnsignedInt offset = 0);

MAGNUM_MESHTOOLS_EXPORT Containers::Array<UnsignedInt> generateQuadIndices(const Containers::StridedArrayView1D<const Vector3>& positions, const Containers::StridedArrayView1D<const UnsignedByte>& quads, UnsignedInt offset = 0);

MAGNUM_MESHTOOLS_EXPORT void generateQuadIndicesInto(const Containers::StridedArrayView1D<const Vector3>& positions, const Containers::StridedArrayView1D<const UnsignedInt>& quads, const Containers::StridedArrayView1D<UnsignedInt>& output, UnsignedInt offset = 0);

MAGNUM_MESHTOOLS_EXPORT void generateQuadIndicesInto(const Containers::StridedArrayView1D<const Vector3>& positions, const Containers::StridedArrayView1D<const UnsignedShort>& quads, const Containers::StridedArrayView1D<UnsignedShort>& output, UnsignedInt offset = 0);

MAGNUM_MESHTOOLS_EXPORT void generateQuadIndicesInto(const Containers::StridedArrayView1D<const Vector3>& positions, const Containers::StridedArrayView1D<const UnsignedByte>& quads, const Containers::StridedArrayView1D<UnsignedByte>& output, UnsignedInt offset = 0);

}}

#endif
#ifdef MAGNUM_MESHTOOLS_IMPLEMENTATION
#if !defined(CORRADE_ASSERT_UNREACHABLE) && !defined(NDEBUG)
#include <cassert>
#endif

#include "CorradeArray.h"
#include "MagnumMathBatch.hpp"

#ifdef __BYTE_ORDER__
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#define CORRADE_TARGET_BIG_ENDIAN
#elif __BYTE_ORDER__ != __ORDER_LITTLE_ENDIAN__
#error what kind of endianness is this?
#endif
#elif defined(__hppa__) || \
    defined(__m68k__) || defined(mc68000) || defined(_M_M68K) || \
    (defined(__MIPS__) && defined(__MIPSEB__)) || \
    defined(__ppc__) || defined(__POWERPC__) || defined(_M_PPC) || \
    defined(__sparc__)
#define CORRADE_TARGET_BIG_ENDIAN
#endif

#ifndef CORRADE_ASSERT_UNREACHABLE
#ifdef NDEBUG
#ifdef CORRADE_TARGET_GCC
#define CORRADE_ASSERT_UNREACHABLE(message, returnValue) __builtin_unreachable()
#elif defined(CORRADE_TARGET_MSVC)
#define CORRADE_ASSERT_UNREACHABLE(message, returnValue) __assume(0)
#else
#define CORRADE_ASSERT_UNREACHABLE(message, returnValue) std::abort()
#endif
#else
#define CORRADE_ASSERT_UNREACHABLE(message, returnValue) assert(!"unreachable code")
#endif
#endif

namespace Magnum { namespace MeshTools {

Range3D boundingRange(const Containers::StridedArrayView1D<const Vector3>& points) {
    return Math::minmax(points);
}

Containers::Pair<Vector3, Float> boundingSphereBouncingBubble(const Containers::StridedArrayView1D<const Vector3>& points) {
    if(points.isEmpty()) return {{}, Math::TypeTraits<Float>::epsilon()};

    Vector3 center = points[0];
    Float radius = Math::TypeTraits<Float>::epsilon();
    Float radiusSquared = radius*radius;

    for(Int i = 0; i < 2; ++i) {
        for(const Vector3& p : points) {
            const Float ds = (p - center).dot();
            if(ds > radiusSquared) {
                const Float alphaInv = radius/Math::sqrt(ds);
                const Float alphaSqInv = radiusSquared/ds;
                radius = (1.0f/alphaInv + alphaInv)*0.5f*radius;
                center = ((1.0f + alphaSqInv)*center + (1.0f - alphaSqInv)*p)*0.5f;
                radiusSquared = radius*radius;
            }
        }
    }

    for(const Vector3& p : points) {
        const Vector3 diff = p - center;
        const Float ds = diff.dot();
        if(ds > radiusSquared) {
            const Float d = Math::sqrt(ds);
            radius = (radius + d)*0.5f;
            center = center + ((d - radius)/d*diff);
            radiusSquared = radius*radius;
        }
    }

    return {center, radius};
}

}}
namespace Magnum { namespace MeshTools {

void generateTrivialIndicesInto(const Containers::StridedArrayView1D<UnsignedInt>& output, /*mutable*/ UnsignedInt offset) {
    for(std::size_t i = 0; i != output.size(); ++i)
        output[i] = offset++;
}

Containers::Array<UnsignedInt> generateTrivialIndices(const UnsignedInt vertexCount, const UnsignedInt offset) {
    Containers::Array<UnsignedInt> output{NoInit, vertexCount};
    generateTrivialIndicesInto(output, offset);
    return output;
}

void generateLineStripIndicesInto(const UnsignedInt vertexCount, const Containers::StridedArrayView1D<UnsignedInt>& output, const UnsignedInt offset) {
    CORRADE_ASSERT(vertexCount == 0 || vertexCount >= 2,
        "MeshTools::generateLineStripIndicesInto(): expected either zero or at least two vertices, got" << vertexCount, );

    const UnsignedInt iMax = Math::max(vertexCount, 1u) - 1;
    CORRADE_ASSERT(output.size() == 2*iMax,
        "MeshTools::generateLineStripIndicesInto(): bad output size, expected" << 2*iMax << "but got" << output.size(), );

    for(std::size_t i = 0; i != iMax; ++i) {
        output[i*2 + 0] = offset + i;
        output[i*2 + 1] = offset + i + 1;
    }
}

namespace {

template<class T> void generateLineStripIndicesIntoImplementation(const Containers::StridedArrayView1D<const T>& indices, const Containers::StridedArrayView1D<UnsignedInt>& output, const UnsignedInt offset) {
    CORRADE_ASSERT(indices.size() == 0 || indices.size() >= 2,
        "MeshTools::generateLineStripIndicesInto(): expected either zero or at least two indices, got" << indices.size(), );

    const UnsignedInt iMax = Math::max(indices.size(), std::size_t{1}) - 1;
    CORRADE_ASSERT(output.size() == 2*iMax,
        "MeshTools::generateLineStripIndicesInto(): bad output size, expected" << 2*iMax << "but got" << output.size(), );

    for(std::size_t i = 0; i != iMax; ++i) {
        output[i*2 + 0] = offset + indices[i];
        output[i*2 + 1] = offset + indices[i + 1];
    }
}

}

void generateLineStripIndicesInto(const Containers::StridedArrayView1D<const UnsignedInt>& indices, const Containers::StridedArrayView1D<UnsignedInt>& output, const UnsignedInt offset) {
    generateLineStripIndicesIntoImplementation(indices, output, offset);
}

void generateLineStripIndicesInto(const Containers::StridedArrayView1D<const UnsignedShort>& indices, const Containers::StridedArrayView1D<UnsignedInt>& output, const UnsignedInt offset) {
    generateLineStripIndicesIntoImplementation(indices, output, offset);
}

void generateLineStripIndicesInto(const Containers::StridedArrayView1D<const UnsignedByte>& indices, const Containers::StridedArrayView1D<UnsignedInt>& output, const UnsignedInt offset) {
    generateLineStripIndicesIntoImplementation(indices, output, offset);
}

void generateLineStripIndicesInto(const Containers::StridedArrayView2D<const char>& indices, const Containers::StridedArrayView1D<UnsignedInt>& output, const UnsignedInt offset) {
    CORRADE_ASSERT(indices.isContiguous<1>(), "MeshTools::generateLineStripIndicesInto(): second index view dimension is not contiguous", );
    if(indices.size()[1] == 4)
        return generateLineStripIndicesInto(Containers::arrayCast<1, const UnsignedInt>(indices), output, offset);
    else if(indices.size()[1] == 2)
        return generateLineStripIndicesInto(Containers::arrayCast<1, const UnsignedShort>(indices), output, offset);
    else if(indices.size()[1] == 1)
        return generateLineStripIndicesInto(Containers::arrayCast<1, const UnsignedByte>(indices), output, offset);
    else CORRADE_ASSERT_UNREACHABLE("MeshTools::generateLineStripIndicesInto(): expected index type size 1, 2 or 4 but got" << indices.size()[1], );
}

Containers::Array<UnsignedInt> generateLineStripIndices(const UnsignedInt vertexCount, const UnsignedInt offset) {
    Containers::Array<UnsignedInt> output{NoInit, 2*(Math::max(vertexCount, 1u) - 1)};
    generateLineStripIndicesInto(vertexCount, output, offset);
    return output;
}

Containers::Array<UnsignedInt> generateLineStripIndices(const Containers::StridedArrayView1D<const UnsignedInt>& indices, const UnsignedInt offset) {
    Containers::Array<UnsignedInt> output{NoInit, 2*(Math::max(indices.size(), std::size_t{1}) - 1)};
    generateLineStripIndicesInto(indices, output, offset);
    return output;
}

Containers::Array<UnsignedInt> generateLineStripIndices(const Containers::StridedArrayView1D<const UnsignedShort>& indices, const UnsignedInt offset) {
    Containers::Array<UnsignedInt> output{NoInit, 2*(Math::max(indices.size(), std::size_t{1}) - 1)};
    generateLineStripIndicesInto(indices, output, offset);
    return output;
}

Containers::Array<UnsignedInt> generateLineStripIndices(const Containers::StridedArrayView1D<const UnsignedByte>& indices, const UnsignedInt offset) {
    Containers::Array<UnsignedInt> output{NoInit, 2*(Math::max(indices.size(), std::size_t{1}) - 1)};
    generateLineStripIndicesInto(indices, output, offset);
    return output;
}

Containers::Array<UnsignedInt> generateLineStripIndices(const Containers::StridedArrayView2D<const char>& indices, const UnsignedInt offset) {
    Containers::Array<UnsignedInt> output{NoInit, 2*(Math::max(indices.size()[0], std::size_t{1}) - 1)};
    generateLineStripIndicesInto(indices, output, offset);
    return output;
}

void generateLineLoopIndicesInto(const UnsignedInt vertexCount, const Containers::StridedArrayView1D<UnsignedInt>& output, const UnsignedInt offset) {
    CORRADE_ASSERT(vertexCount == 0 || vertexCount >= 2,
        "MeshTools::generateLineLoopIndicesInto(): expected either zero or at least two vertices, got" << vertexCount, );
    CORRADE_ASSERT(output.size() == 2*vertexCount,
        "MeshTools::generateLineLoopIndicesInto(): bad output size, expected" << 2*vertexCount << "but got" << output.size(), );

    for(std::size_t i = 0, iMax = Math::max(vertexCount, 1u) - 1; i != iMax; ++i) {
        output[i*2 + 0] = offset + i;
        output[i*2 + 1] = offset + i + 1;
    }
    if(vertexCount >= 2) {
        output[2*vertexCount - 2] = offset + vertexCount - 1;
        output[2*vertexCount - 1] = offset + 0;
    }
}

namespace {

template<class T> void generateLineLoopIndicesIntoImplementation(const Containers::StridedArrayView1D<const T>& indices, const Containers::StridedArrayView1D<UnsignedInt>& output, const UnsignedInt offset) {
    CORRADE_ASSERT(indices.size() == 0 || indices.size() >= 2,
        "MeshTools::generateLineLoopIndicesInto(): expected either zero or at least two indices, got" << indices.size(), );
    CORRADE_ASSERT(output.size() == 2*indices.size(),
        "MeshTools::generateLineLoopIndicesInto(): bad output size, expected" << 2*indices.size() << "but got" << output.size(), );

    for(std::size_t i = 0, iMax = Math::max(indices.size(), std::size_t{1}) - 1; i != iMax; ++i) {
        output[i*2 + 0] = offset + indices[i];
        output[i*2 + 1] = offset + indices[i + 1];
    }
    if(indices.size() >= 2) {
        output[2*indices.size() - 2] = offset + indices[indices.size() - 1];
        output[2*indices.size() - 1] = offset + indices[0];
    }
}

}

void generateLineLoopIndicesInto(const Containers::StridedArrayView1D<const UnsignedInt>& indices, const Containers::StridedArrayView1D<UnsignedInt>& output, const UnsignedInt offset) {
    generateLineLoopIndicesIntoImplementation(indices, output, offset);
}

void generateLineLoopIndicesInto(const Containers::StridedArrayView1D<const UnsignedShort>& indices, const Containers::StridedArrayView1D<UnsignedInt>& output, const UnsignedInt offset) {
    generateLineLoopIndicesIntoImplementation(indices, output, offset);
}

void generateLineLoopIndicesInto(const Containers::StridedArrayView1D<const UnsignedByte>& indices, const Containers::StridedArrayView1D<UnsignedInt>& output, const UnsignedInt offset) {
    generateLineLoopIndicesIntoImplementation(indices, output, offset);
}

void generateLineLoopIndicesInto(const Containers::StridedArrayView2D<const char>& indices, const Containers::StridedArrayView1D<UnsignedInt>& output, const UnsignedInt offset) {
    CORRADE_ASSERT(indices.isContiguous<1>(), "MeshTools::generateLineLoopIndicesInto(): second index view dimension is not contiguous", );
    if(indices.size()[1] == 4)
        return generateLineLoopIndicesInto(Containers::arrayCast<1, const UnsignedInt>(indices), output, offset);
    else if(indices.size()[1] == 2)
        return generateLineLoopIndicesInto(Containers::arrayCast<1, const UnsignedShort>(indices), output, offset);
    else if(indices.size()[1] == 1)
        return generateLineLoopIndicesInto(Containers::arrayCast<1, const UnsignedByte>(indices), output, offset);
    else CORRADE_ASSERT_UNREACHABLE("MeshTools::generateLineLoopIndicesInto(): expected index type size 1, 2 or 4 but got" << indices.size()[1], );
}

Containers::Array<UnsignedInt> generateLineLoopIndices(const UnsignedInt vertexCount, const UnsignedInt offset) {
    Containers::Array<UnsignedInt> output{NoInit, 2*vertexCount};
    generateLineLoopIndicesInto(vertexCount, output, offset);
    return output;
}

Containers::Array<UnsignedInt> generateLineLoopIndices(const Containers::StridedArrayView1D<const UnsignedInt>& indices, const UnsignedInt offset) {
    Containers::Array<UnsignedInt> output{NoInit, 2*indices.size()};
    generateLineLoopIndicesInto(indices, output, offset);
    return output;
}

Containers::Array<UnsignedInt> generateLineLoopIndices(const Containers::StridedArrayView1D<const UnsignedShort>& indices, const UnsignedInt offset) {
    Containers::Array<UnsignedInt> output{NoInit, 2*indices.size()};
    generateLineLoopIndicesInto(indices, output, offset);
    return output;
}

Containers::Array<UnsignedInt> generateLineLoopIndices(const Containers::StridedArrayView1D<const UnsignedByte>& indices, const UnsignedInt offset) {
    Containers::Array<UnsignedInt> output{NoInit, 2*indices.size()};
    generateLineLoopIndicesInto(indices, output, offset);
    return output;
}

Containers::Array<UnsignedInt> generateLineLoopIndices(const Containers::StridedArrayView2D<const char>& indices, const UnsignedInt offset) {
    Containers::Array<UnsignedInt> output{NoInit, 2*indices.size()[0]};
    generateLineLoopIndicesInto(indices, output, offset);
    return output;
}

void generateTriangleStripIndicesInto(const UnsignedInt vertexCount, const Containers::StridedArrayView1D<UnsignedInt>& output, const UnsignedInt offset) {
    CORRADE_ASSERT(vertexCount == 0 || vertexCount >= 3,
        "MeshTools::generateTriangleStripIndicesInto(): expected either zero or at least three vertices, got" << vertexCount, );

    const UnsignedInt iMax = Math::max(vertexCount, 2u) - 2;
    CORRADE_ASSERT(output.size() == 3*iMax,
        "MeshTools::generateTriangleStripIndicesInto(): bad output size, expected" << 3*iMax << "but got" << output.size(), );

    for(std::size_t i = 0; i != iMax; ++i) {
        output[i*3 + 0] = offset + (i % 2 ? i + 1 : i);
        output[i*3 + 1] = offset + (i % 2 ? i : i + 1);
        output[i*3 + 2] = offset + i + 2;
    }
}

namespace {

template<class T> void generateTriangleStripIndicesIntoImplementation(const Containers::StridedArrayView1D<const T>& indices, const Containers::StridedArrayView1D<UnsignedInt>& output, const UnsignedInt offset) {
    CORRADE_ASSERT(indices.size() == 0 || indices.size() >= 3,
        "MeshTools::generateTriangleStripIndicesInto(): expected either zero or at least three indices, got" << indices.size(), );

    const UnsignedInt iMax = Math::max(indices.size(), std::size_t{2}) - 2;
    CORRADE_ASSERT(output.size() == 3*iMax,
        "MeshTools::generateTriangleStripIndicesInto(): bad output size, expected" << 3*iMax << "but got" << output.size(), );

    for(std::size_t i = 0; i != iMax; ++i) {
        output[i*3 + 0] = offset + indices[i % 2 ? i + 1 : i];
        output[i*3 + 1] = offset + indices[i % 2 ? i : i + 1];
        output[i*3 + 2] = offset + indices[i + 2];
    }
}

}

void generateTriangleStripIndicesInto(const Containers::StridedArrayView1D<const UnsignedInt>& indices, const Containers::StridedArrayView1D<UnsignedInt>& output, const UnsignedInt offset) {
    generateTriangleStripIndicesIntoImplementation(indices, output, offset);
}

void generateTriangleStripIndicesInto(const Containers::StridedArrayView1D<const UnsignedShort>& indices, const Containers::StridedArrayView1D<UnsignedInt>& output, const UnsignedInt offset) {
    generateTriangleStripIndicesIntoImplementation(indices, output, offset);
}

void generateTriangleStripIndicesInto(const Containers::StridedArrayView1D<const UnsignedByte>& indices, const Containers::StridedArrayView1D<UnsignedInt>& output, const UnsignedInt offset) {
    generateTriangleStripIndicesIntoImplementation(indices, output, offset);
}

void generateTriangleStripIndicesInto(const Containers::StridedArrayView2D<const char>& indices, const Containers::StridedArrayView1D<UnsignedInt>& output, const UnsignedInt offset) {
    CORRADE_ASSERT(indices.isContiguous<1>(), "MeshTools::generateTriangleStripIndicesInto(): second index view dimension is not contiguous", );
    if(indices.size()[1] == 4)
        return generateTriangleStripIndicesInto(Containers::arrayCast<1, const UnsignedInt>(indices), output, offset);
    else if(indices.size()[1] == 2)
        return generateTriangleStripIndicesInto(Containers::arrayCast<1, const UnsignedShort>(indices), output, offset);
    else if(indices.size()[1] == 1)
        return generateTriangleStripIndicesInto(Containers::arrayCast<1, const UnsignedByte>(indices), output, offset);
    else CORRADE_ASSERT_UNREACHABLE("MeshTools::generateTriangleStripIndicesInto(): expected index type size 1, 2 or 4 but got" << indices.size()[1], );
}

Containers::Array<UnsignedInt> generateTriangleStripIndices(const UnsignedInt vertexCount, const UnsignedInt offset) {
    Containers::Array<UnsignedInt> output{NoInit, 3*(Math::max(vertexCount, 2u) - 2u)};
    generateTriangleStripIndicesInto(vertexCount, output, offset);
    return output;
}

Containers::Array<UnsignedInt> generateTriangleStripIndices(const Containers::StridedArrayView1D<const UnsignedInt>& indices, const UnsignedInt offset) {
    Containers::Array<UnsignedInt> output{NoInit, 3*(Math::max(indices.size(), std::size_t{2}) - 2)};
    generateTriangleStripIndicesInto(indices, output, offset);
    return output;
}

Containers::Array<UnsignedInt> generateTriangleStripIndices(const Containers::StridedArrayView1D<const UnsignedShort>& indices, const UnsignedInt offset) {
    Containers::Array<UnsignedInt> output{NoInit, 3*(Math::max(indices.size(), std::size_t{2}) - 2)};
    generateTriangleStripIndicesInto(indices, output, offset);
    return output;
}

Containers::Array<UnsignedInt> generateTriangleStripIndices(const Containers::StridedArrayView1D<const UnsignedByte>& indices, const UnsignedInt offset) {
    Containers::Array<UnsignedInt> output{NoInit, 3*(Math::max(indices.size(), std::size_t{2}) - 2)};
    generateTriangleStripIndicesInto(indices, output, offset);
    return output;
}

Containers::Array<UnsignedInt> generateTriangleStripIndices(const Containers::StridedArrayView2D<const char>& indices, const UnsignedInt offset) {
    Containers::Array<UnsignedInt> output{NoInit, 3*(Math::max(indices.size()[0], std::size_t{2}) - 2)};
    generateTriangleStripIndicesInto(indices, output, offset);
    return output;
}

void generateTriangleFanIndicesInto(const UnsignedInt vertexCount, const Containers::StridedArrayView1D<UnsignedInt>& output, const UnsignedInt offset) {
    CORRADE_ASSERT(vertexCount == 0 || vertexCount >= 3,
        "MeshTools::generateTriangleFanIndicesInto(): expected either zero or at least three vertices, got" << vertexCount, );

    const UnsignedInt iMax = Math::max(vertexCount, 2u) - 2;
    CORRADE_ASSERT(output.size() == 3*iMax,
        "MeshTools::generateTriangleFanIndicesInto(): bad output size, expected" << 3*iMax << "but got" << output.size(), );

    for(std::size_t i = 0; i != iMax; ++i) {
        output[i*3 + 0] = offset + 0;
        output[i*3 + 1] = offset + i + 1;
        output[i*3 + 2] = offset + i + 2;
    }
}

namespace {

template<class T> void generateTriangleFanIndicesIntoImplementation(const Containers::StridedArrayView1D<const T>& indices, const Containers::StridedArrayView1D<UnsignedInt>& output, const UnsignedInt offset) {
    CORRADE_ASSERT(indices.size() == 0 || indices.size() >= 3,
        "MeshTools::generateTriangleFanIndicesInto(): expected either zero or at least three indices, got" << indices.size(), );

    const UnsignedInt iMax = Math::max(indices.size(), std::size_t{2}) - 2;
    CORRADE_ASSERT(output.size() == 3*iMax,
        "MeshTools::generateTriangleFanIndicesInto(): bad output size, expected" << 3*iMax << "but got" << output.size(), );

    for(std::size_t i = 0; i != iMax; ++i) {
        output[i*3 + 0] = offset + indices[0];
        output[i*3 + 1] = offset + indices[i + 1];
        output[i*3 + 2] = offset + indices[i + 2];
    }
}

}

void generateTriangleFanIndicesInto(const Containers::StridedArrayView1D<const UnsignedInt>& indices, const Containers::StridedArrayView1D<UnsignedInt>& output, const UnsignedInt offset) {
    generateTriangleFanIndicesIntoImplementation(indices, output, offset);
}

void generateTriangleFanIndicesInto(const Containers::StridedArrayView1D<const UnsignedShort>& indices, const Containers::StridedArrayView1D<UnsignedInt>& output, const UnsignedInt offset) {
    generateTriangleFanIndicesIntoImplementation(indices, output, offset);
}

void generateTriangleFanIndicesInto(const Containers::StridedArrayView1D<const UnsignedByte>& indices, const Containers::StridedArrayView1D<UnsignedInt>& output, const UnsignedInt offset) {
    generateTriangleFanIndicesIntoImplementation(indices, output, offset);
}

void generateTriangleFanIndicesInto(const Containers::StridedArrayView2D<const char>& indices, const Containers::StridedArrayView1D<UnsignedInt>& output, const UnsignedInt offset) {
    CORRADE_ASSERT(indices.isContiguous<1>(), "MeshTools::generateTriangleFanIndicesInto(): second index view dimension is not contiguous", );
    if(indices.size()[1] == 4)
        return generateTriangleFanIndicesInto(Containers::arrayCast<1, const UnsignedInt>(indices), output, offset);
    else if(indices.size()[1] == 2)
        return generateTriangleFanIndicesInto(Containers::arrayCast<1, const UnsignedShort>(indices), output, offset);
    else if(indices.size()[1] == 1)
        return generateTriangleFanIndicesInto(Containers::arrayCast<1, const UnsignedByte>(indices), output, offset);
    else CORRADE_ASSERT_UNREACHABLE("MeshTools::generateTriangleFanIndicesInto(): expected index type size 1, 2 or 4 but got" << indices.size()[1], );
}

Containers::Array<UnsignedInt> generateTriangleFanIndices(const UnsignedInt vertexCount, const UnsignedInt offset) {
    Containers::Array<UnsignedInt> output{NoInit, 3*(Math::max(vertexCount, 2u) - 2)};
    generateTriangleFanIndicesInto(vertexCount, output, offset);
    return output;
}

Containers::Array<UnsignedInt> generateTriangleFanIndices(const Containers::StridedArrayView1D<const UnsignedInt>& indices, const UnsignedInt offset) {
    Containers::Array<UnsignedInt> output{NoInit, 3*(Math::max(indices.size(), std::size_t{2}) - 2)};
    generateTriangleFanIndicesInto(indices, output, offset);
    return output;
}

Containers::Array<UnsignedInt> generateTriangleFanIndices(const Containers::StridedArrayView1D<const UnsignedShort>& indices, const UnsignedInt offset) {
    Containers::Array<UnsignedInt> output{NoInit, 3*(Math::max(indices.size(), std::size_t{2}) - 2)};
    generateTriangleFanIndicesInto(indices, output, offset);
    return output;
}

Containers::Array<UnsignedInt> generateTriangleFanIndices(const Containers::StridedArrayView1D<const UnsignedByte>& indices, const UnsignedInt offset) {
    Containers::Array<UnsignedInt> output{NoInit, 3*(Math::max(indices.size(), std::size_t{2}) - 2)};
    generateTriangleFanIndicesInto(indices, output, offset);
    return output;
}

Containers::Array<UnsignedInt> generateTriangleFanIndices(const Containers::StridedArrayView2D<const char>& indices, const UnsignedInt offset) {
    Containers::Array<UnsignedInt> output{NoInit, 3*(Math::max(indices.size()[0], std::size_t{2}) - 2)};
    generateTriangleFanIndicesInto(indices, output, offset);
    return output;
}

namespace {

template<class T> inline void generateQuadIndicesIntoImplementation(const Containers::StridedArrayView1D<const Vector3>& positions, const Containers::StridedArrayView1D<const T>& quads, const Containers::StridedArrayView1D<T>& output, const UnsignedInt offset) {
    CORRADE_ASSERT(quads.size() % 4 == 0,
        "MeshTools::generateQuadIndicesInto(): quad index count" << quads.size() << "not divisible by 4", );
    CORRADE_ASSERT(quads.size()*6/4 == output.size(),
        "MeshTools::generateQuadIndicesInto(): bad output size, expected" << quads.size()*6/4 << "but got" << output.size(), );

    for(std::size_t i = 0, max = quads.size()/4; i != max; ++i) {
        auto get = [&](UnsignedInt j) -> const Vector3& {
            UnsignedInt index = quads[4*i + j];
            CORRADE_ASSERT(index < positions.size(),
                "MeshTools::generateQuadIndicesInto(): index" << index << "out of range for" << positions.size() << "elements", positions[0]);
            return positions[index];
        };
        const Vector3& a = get(0);
        const Vector3& b = get(1);
        const Vector3& c = get(2);
        const Vector3& d = get(3);

        constexpr UnsignedInt SplitAbcAcd[] { 0, 1, 2, 0, 2, 3 };
        constexpr UnsignedInt SplitDabDbc[] { 3, 0, 1, 3, 1, 2 };
        const UnsignedInt* split;
        const bool abcAcdOppositeDirection = Math::dot(Math::cross(c - b, a - b), Math::cross(d - c, a - c)) < 0.0f;
        const bool dabDbcOppositeDirection = Math::dot(Math::cross(d - b, a - b), Math::cross(c - b, d - b)) < 0.0f;

        if(abcAcdOppositeDirection != dabDbcOppositeDirection)
            split = abcAcdOppositeDirection ? SplitDabDbc : SplitAbcAcd;

        else split = (b - d).dot() < (c - a).dot() ? SplitDabDbc : SplitAbcAcd;

        for(std::size_t j = 0; j != 6; ++j)
            output[6*i + j] = offset + quads[4*i + split[j]];
    }
}

}

Containers::Array<UnsignedInt> generateQuadIndices(const Containers::StridedArrayView1D<const Vector3>& positions, const Containers::StridedArrayView1D<const UnsignedInt>& quads, const UnsignedInt offset) {
    Containers::Array<UnsignedInt> out{NoInit, quads.size()*6/4};
    generateQuadIndicesIntoImplementation(positions, quads, Containers::stridedArrayView(out), offset);
    return out;
}

Containers::Array<UnsignedInt> generateQuadIndices(const Containers::StridedArrayView1D<const Vector3>& positions, const Containers::StridedArrayView1D<const UnsignedShort>& quads, const UnsignedInt offset) {
    Containers::Array<UnsignedInt> out{ValueInit, quads.size()*6/4};
    generateQuadIndicesIntoImplementation(positions, quads,
        Containers::StridedArrayView1D<UnsignedShort>{
            Containers::arrayCast<UnsignedShort>(out),
            reinterpret_cast<UnsignedShort*>(out.data())
                #ifdef CORRADE_TARGET_BIG_ENDIAN
                + 1
                #endif
            , out.size(), 4},
        offset);
    return out;
}

Containers::Array<UnsignedInt> generateQuadIndices(const Containers::StridedArrayView1D<const Vector3>& positions, const Containers::StridedArrayView1D<const UnsignedByte>& quads, const UnsignedInt offset) {
    Containers::Array<UnsignedInt> out{ValueInit, quads.size()*6/4};
    generateQuadIndicesIntoImplementation(positions, quads,
        Containers::StridedArrayView1D<UnsignedByte>{
            Containers::arrayCast<UnsignedByte>(out),
            reinterpret_cast<UnsignedByte*>(out.data())
                #ifdef CORRADE_TARGET_BIG_ENDIAN
                + 3
                #endif
            , out.size(), 4},
        offset);
    return out;
}

void generateQuadIndicesInto(const Containers::StridedArrayView1D<const Vector3>& positions, const Containers::StridedArrayView1D<const UnsignedInt>& quads, const Containers::StridedArrayView1D<UnsignedInt>& output, const UnsignedInt offset) {
    return generateQuadIndicesIntoImplementation(positions, quads, output, offset);
}

void generateQuadIndicesInto(const Containers::StridedArrayView1D<const Vector3>& positions, const Containers::StridedArrayView1D<const UnsignedShort>& quads, const Containers::StridedArrayView1D<UnsignedShort>& output, const UnsignedInt offset) {
    return generateQuadIndicesIntoImplementation(positions, quads, output, offset);
}

void generateQuadIndicesInto(const Containers::StridedArrayView1D<const Vector3>& positions, const Containers::StridedArrayView1D<const UnsignedByte>& quads, const Containers::StridedArrayView1D<UnsignedByte>& output, const UnsignedInt offset) {
    return generateQuadIndicesIntoImplementation(positions, quads, output, offset);
}

}}
#endif
