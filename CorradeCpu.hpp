/*
    Corrade::Cpu
        — compile-time and runtime CPU feature detection and dispatch

    https://doc.magnum.graphics/corrade/namespaceCorrade_1_1Cpu.html

    This is a single-header library generated from the Corrade project. With
    the goal being easy integration, it's deliberately free of all comments
    to keep the file size small. More info, detailed changelogs and docs here:

    -   Project homepage — https://magnum.graphics/corrade/
    -   Documentation — https://doc.magnum.graphics/corrade/
    -   GitHub project page — https://github.com/mosra/corrade
    -   GitHub Singles repository — https://github.com/mosra/magnum-singles

    The library has a separate non-inline implementation part, enable it *just
    once* like this:

        #define CORRADE_CPU_IMPLEMENTATION
        #include <CorradeCpu.hpp>

    If you need the deinlined symbols to be exported from a shared library,
    `#define CORRADE_UTILITY_EXPORT` as appropriate. To enable the IFUNC
    functionality, `#define CORRADE_CPU_USE_IFUNC` before including the file.

    v2020.06-1687-g6b5f (2024-06-29)
    -   FreeBSD and Emscripten compatibility fixes
    v2020.06-1454-gfc3b7 (2023-08-27)
    -   Added BMI2 detection on x86
    -   Fixed an issue on GCC 12+ and Clang, where only one of multiple
        CORRADE_ENABLE_ macro annotations would get used
    -   Fixed a potential build issue on x86 if none of the extra instruction
        sets are enabled at compile time
    -   Compatibility with C++20 which removes the <ciso646> header
    v2020.06-1040-g30cd2 (2022-09-05)
    -   Fixed a build issue on platforms that are neither x86, ARM nor WASM
    -   Renamed to CorradeCpu.hpp to imply the separate implementation part
        consistently with other header-only libraries
    v2020.06-1018-gef42a6 (2022-08-13)
    -   Properly checking XSAVE prerequisites for AVX-512
    v2020.06-1015-g8cbd6 (2022-08-02)
    -   Initial release

    Generated from Corrade v2020.06-1687-g6b5f (2024-06-29), 1716 / 1995 LoC
*/

/*
    This file is part of Corrade.

    Copyright © 2007, 2008, 2009, 2010, 2011, 2012, 2013, 2014, 2015, 2016,
                2017, 2018, 2019, 2020, 2021, 2022, 2023
              Vladimír Vondruš <mosra@centrum.cz>
    Copyright © 2023 Robert Clausecker <fuz@FreeBSD.org>

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

#ifdef __ANDROID__
#define CORRADE_TARGET_ANDROID
#endif
#ifdef __EMSCRIPTEN__
#define CORRADE_TARGET_EMSCRIPTEN
#endif
#ifdef __APPLE__
#define CORRADE_TARGET_APPLE
#endif

#ifndef Corrade_configure_h
#define Corrade_configure_h

#if defined(__i386) || defined(__x86_64) || defined(_M_IX86) || defined(_M_X64)
#define CORRADE_TARGET_X86

#elif defined(__arm__) || defined(__aarch64__) || defined(_M_ARM) || defined(_M_ARM64)
#define CORRADE_TARGET_ARM

#elif defined(__powerpc__) || defined(__powerpc64__) || defined(_M_PPC)
#define CORRADE_TARGET_POWERPC

#elif defined(__wasm__)
#define CORRADE_TARGET_WASM

#endif

#if defined(CORRADE_TARGET_EMSCRIPTEN) && (defined(CORRADE_TARGET_X86) || defined(CORRADE_TARGET_ARM) || defined(CORRADE_TARGET_POWERPC))
#error CORRADE_TARGET_X86 / _ARM / _POWERPC defined on Emscripten
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

#ifdef __GNUC__
#define CORRADE_TARGET_GCC
#endif

#ifdef __clang__
#define CORRADE_TARGET_CLANG
#endif

#if defined(__clang__) && defined(_MSC_VER)
#define CORRADE_TARGET_CLANG_CL
#endif

#if defined(__clang__) && defined(__apple_build_version__)
#define  CORRADE_TARGET_APPLE_CLANG
#endif

#ifdef _MSC_VER
#define CORRADE_TARGET_MSVC
#endif

#ifdef __MINGW32__
#define CORRADE_TARGET_MINGW
#endif

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

#ifdef CORRADE_TARGET_X86

#ifdef CORRADE_TARGET_GCC
#ifdef __SSE2__
#define CORRADE_TARGET_SSE2
#endif
#ifdef __SSE3__
#define CORRADE_TARGET_SSE3
#endif
#ifdef __SSSE3__
#define CORRADE_TARGET_SSSE3
#endif
#ifdef __SSE4_1__
#define CORRADE_TARGET_SSE41
#endif
#ifdef __SSE4_2__
#define CORRADE_TARGET_SSE42
#endif

#elif defined(CORRADE_TARGET_MSVC)
#if (defined(_M_IX86_FP) && _M_IX86_FP == 2) || defined(_M_AMD64) || defined(_M_X64)
#define CORRADE_TARGET_SSE2
#endif
#ifdef __AVX__
#define CORRADE_TARGET_SSE3
#define CORRADE_TARGET_SSSE3
#define CORRADE_TARGET_SSE41
#define CORRADE_TARGET_SSE42
#endif
#endif

#if defined(CORRADE_TARGET_GCC) || defined(CORRADE_TARGET_MSVC)
#ifdef __AVX__
#define CORRADE_TARGET_AVX
#endif
#ifdef __AVX2__
#define CORRADE_TARGET_AVX2
#endif
#ifdef __AVX512F__
#define CORRADE_TARGET_AVX512F
#endif
#endif

#ifdef CORRADE_TARGET_GCC
#ifdef __POPCNT__
#define CORRADE_TARGET_POPCNT
#endif
#ifdef __LZCNT__
#define CORRADE_TARGET_LZCNT
#endif
#ifdef __BMI__
#define CORRADE_TARGET_BMI1
#endif
#ifdef __BMI2__
#define CORRADE_TARGET_BMI2
#endif

#elif defined(CORRADE_TARGET_MSVC)
#ifdef __AVX__
#if !defined(CORRADE_TARGET_CLANG_CL) || defined(__POPCNT__)
#define CORRADE_TARGET_POPCNT
#endif
#endif
#ifdef __AVX2__
#if !defined(CORRADE_TARGET_CLANG_CL) || defined(__LZCNT__)
#define CORRADE_TARGET_LZCNT
#endif
#if !defined(CORRADE_TARGET_CLANG_CL) || defined(__BMI__)
#define CORRADE_TARGET_BMI1
#endif
#if !defined(CORRADE_TARGET_CLANG_CL) || defined(__BMI2__)
#define CORRADE_TARGET_BMI2
#endif
#endif
#endif

#ifdef CORRADE_TARGET_GCC
#ifdef __F16C__
#define CORRADE_TARGET_AVX_F16C
#endif
#ifdef __FMA__
#define CORRADE_TARGET_AVX_FMA
#endif
#elif defined(CORRADE_TARGET_MSVC) && defined(__AVX2__)
#if !defined(CORRADE_TARGET_CLANG_CL) || defined(__F16C__)
#define CORRADE_TARGET_AVX_F16C
#endif
#if !defined(CORRADE_TARGET_CLANG_CL) || defined(__FMA__)
#define CORRADE_TARGET_AVX_FMA
#endif
#endif

#elif defined(CORRADE_TARGET_ARM)
#ifdef __ARM_NEON
#define CORRADE_TARGET_NEON
#if defined(__ARM_FEATURE_FMA) && (__ARM_NEON_FP || defined(__aarch64__))
#define CORRADE_TARGET_NEON_FMA
#endif
#ifdef __ARM_FEATURE_FP16_VECTOR_ARITHMETIC
#define CORRADE_TARGET_NEON_FP16
#endif
#endif

#elif defined(CORRADE_TARGET_WASM)
#if defined(CORRADE_TARGET_EMSCRIPTEN) && !defined(__EMSCRIPTEN_major__)
#include <emscripten/version.h>
#endif
#if defined(__wasm_simd128__) && __clang_major__ >= 13 && __EMSCRIPTEN_major__*10000 + __EMSCRIPTEN_minor__*100 + __EMSCRIPTEN_tiny__ >= 20018
#define CORRADE_TARGET_SIMD128
#endif
#endif

#ifdef CORRADE_CPU_USE_IFUNC
#ifdef __has_feature
#if __has_feature(address_sanitizer) || __has_feature(thread_sanitizer) || __has_feature(memory_sanitizer) || __has_feature(undefined_behavior_sanitizer)
#define _CORRADE_SANITIZER_IFUNC_DETECTED
#endif
#elif defined(__SANITIZE_ADDRESS__) || defined(__SANITIZE_THREAD__)
#define _CORRADE_SANITIZER_IFUNC_DETECTED
#endif
#ifdef _CORRADE_SANITIZER_IFUNC_DETECTED
#error Corrade was built with CORRADE_CPU_USE_IFUNC, which is incompatible with sanitizers. Rebuild without this option or disable sanitizers.
#endif
#endif

#endif // kate: hl c++

#ifndef CORRADE_UTILITY_EXPORT
#define CORRADE_UTILITY_EXPORT
#endif
#define _CORRADE_HELPER_PASTE2(a, b) a ## b

#ifdef CORRADE_TARGET_GCC
#define CORRADE_ALWAYS_INLINE __attribute__((always_inline)) inline
#elif defined(CORRADE_TARGET_MSVC)
#define CORRADE_ALWAYS_INLINE __forceinline
#else
#define CORRADE_ALWAYS_INLINE inline
#endif

#ifdef CORRADE_TARGET_GCC
#define CORRADE_NEVER_INLINE __attribute__((noinline))
#elif defined(CORRADE_TARGET_MSVC)
#define CORRADE_NEVER_INLINE __declspec(noinline)
#else
#define CORRADE_NEVER_INLINE
#endif
#ifndef Corrade_Cpu_h
#define Corrade_Cpu_h

#if defined(CORRADE_TARGET_MSVC) && !defined(CORRADE_TARGET_CLANG_CL) && defined(CORRADE_TARGET_X86)
extern "C" {
    void __cpuidex(int[4], int, int);
    unsigned __int64 __cdecl _xgetbv(unsigned int);
}
#endif

namespace Corrade {

namespace Cpu {

template<class> struct TypeTraits;

namespace Implementation {
    struct InitT {};
    constexpr InitT Init{};

    enum: unsigned int { ExtraTagBitOffset = 16 };
}

struct ScalarT {
    constexpr explicit ScalarT(Implementation::InitT) {}
};

template<> struct TypeTraits<ScalarT> {
    enum: unsigned int { Index = 0 };
    static const char* name() { return "Scalar"; }
};

#ifdef CORRADE_TARGET_X86
struct Sse2T: ScalarT {
    constexpr explicit Sse2T(Implementation::InitT): ScalarT{Implementation::Init} {}
};

struct Sse3T: Sse2T {
    constexpr explicit Sse3T(Implementation::InitT): Sse2T{Implementation::Init} {}
};

struct Ssse3T: Sse3T {
    constexpr explicit Ssse3T(Implementation::InitT): Sse3T{Implementation::Init} {}
};

struct Sse41T: Ssse3T {
    constexpr explicit Sse41T(Implementation::InitT): Ssse3T{Implementation::Init} {}
};

struct Sse42T: Sse41T {
    constexpr explicit Sse42T(Implementation::InitT): Sse41T{Implementation::Init} {}
};

struct PopcntT {
    constexpr explicit PopcntT(Implementation::InitT) {}
};

struct LzcntT {
    constexpr explicit LzcntT(Implementation::InitT) {}
};

struct Bmi1T {
    constexpr explicit Bmi1T(Implementation::InitT) {}
};

struct Bmi2T {
    constexpr explicit Bmi2T(Implementation::InitT) {}
};

struct AvxT: Sse42T {
    constexpr explicit AvxT(Implementation::InitT): Sse42T{Implementation::Init} {}
};

struct AvxF16cT {
    constexpr explicit AvxF16cT(Implementation::InitT) {}
};

struct AvxFmaT {
    constexpr explicit AvxFmaT(Implementation::InitT) {}
};

struct Avx2T: AvxT {
    constexpr explicit Avx2T(Implementation::InitT): AvxT{Implementation::Init} {}
};

struct Avx512fT: Avx2T {
    constexpr explicit Avx512fT(Implementation::InitT): Avx2T{Implementation::Init} {}
};

template<> struct TypeTraits<Sse2T> {
    enum: unsigned int { Index = 1 << 0 };
    static const char* name() { return "Sse2"; }
};
template<> struct TypeTraits<Sse3T> {
    enum: unsigned int { Index = 1 << 1 };
    static const char* name() { return "Sse3"; }
};
template<> struct TypeTraits<Ssse3T> {
    enum: unsigned int { Index = 1 << 2 };
    static const char* name() { return "Ssse3"; }
};
template<> struct TypeTraits<Sse41T> {
    enum: unsigned int { Index = 1 << 3 };
    static const char* name() { return "Sse41"; }
};
template<> struct TypeTraits<Sse42T> {
    enum: unsigned int { Index = 1 << 4 };
    static const char* name() { return "Sse42"; }
};
template<> struct TypeTraits<AvxT> {
    enum: unsigned int { Index = 1 << 5 };
    static const char* name() { return "Avx"; }
};
template<> struct TypeTraits<Avx2T> {
    enum: unsigned int { Index = 1 << 6 };
    static const char* name() { return "Avx2"; }
};
template<> struct TypeTraits<Avx512fT> {
    enum: unsigned int { Index = 1 << 7 };
    static const char* name() { return "Avx512f"; }
};

template<> struct TypeTraits<PopcntT> {
    enum: unsigned int { Index = 1 << (0 + Implementation::ExtraTagBitOffset) };
    static const char* name() { return "Popcnt"; }
};
template<> struct TypeTraits<LzcntT> {
    enum: unsigned int { Index = 1 << (1 + Implementation::ExtraTagBitOffset) };
    static const char* name() { return "Lzcnt"; }
};
template<> struct TypeTraits<Bmi1T> {
    enum: unsigned int { Index = 1 << (2 + Implementation::ExtraTagBitOffset) };
    static const char* name() { return "Bmi1"; }
};
template<> struct TypeTraits<Bmi2T> {
    enum: unsigned int { Index = 1 << (3 + Implementation::ExtraTagBitOffset) };
    static const char* name() { return "Bmi2"; }
};
template<> struct TypeTraits<AvxF16cT> {
    enum: unsigned int { Index = 1 << (4 + Implementation::ExtraTagBitOffset) };
    static const char* name() { return "AvxF16c"; }
};
template<> struct TypeTraits<AvxFmaT> {
    enum: unsigned int { Index = 1 << (5 + Implementation::ExtraTagBitOffset) };
    static const char* name() { return "AvxFma"; }
};
#endif

#ifdef CORRADE_TARGET_ARM
struct NeonT: ScalarT {
    constexpr explicit NeonT(Implementation::InitT): ScalarT{Implementation::Init} {}
};

struct NeonFmaT: NeonT {
    constexpr explicit NeonFmaT(Implementation::InitT): NeonT{Implementation::Init} {}
};

struct NeonFp16T: NeonFmaT {
    constexpr explicit NeonFp16T(Implementation::InitT): NeonFmaT{Implementation::Init} {}
};

template<> struct TypeTraits<NeonT> {
    enum: unsigned int { Index = 1 << 0 };
    static const char* name() { return "Neon"; }
};
template<> struct TypeTraits<NeonFmaT> {
    enum: unsigned int { Index = 1 << 1 };
    static const char* name() { return "NeonFma"; }
};
template<> struct TypeTraits<NeonFp16T> {
    enum: unsigned int { Index = 1 << 2 };
    static const char* name() { return "NeonFp16"; }
};
#endif

#ifdef CORRADE_TARGET_WASM
struct Simd128T: ScalarT {
    constexpr explicit Simd128T(Implementation::InitT): ScalarT{Implementation::Init} {}
};

template<> struct TypeTraits<Simd128T> {
    enum: unsigned int { Index = 1 << 0 };
    static const char* name() { return "Simd128"; }
};
#endif

constexpr ScalarT Scalar{Implementation::Init};

#ifdef CORRADE_TARGET_X86
constexpr Sse2T Sse2{Implementation::Init};

constexpr Sse3T Sse3{Implementation::Init};

constexpr Ssse3T Ssse3{Implementation::Init};

constexpr Sse41T Sse41{Implementation::Init};

constexpr Sse42T Sse42{Implementation::Init};

constexpr PopcntT Popcnt{Implementation::Init};

constexpr LzcntT Lzcnt{Implementation::Init};

constexpr Bmi1T Bmi1{Implementation::Init};

constexpr Bmi2T Bmi2{Implementation::Init};

constexpr AvxT Avx{Implementation::Init};

constexpr AvxF16cT AvxF16c{Implementation::Init};

constexpr AvxFmaT AvxFma{Implementation::Init};

constexpr Avx2T Avx2{Implementation::Init};

constexpr Avx512fT Avx512f{Implementation::Init};
#endif

#ifdef CORRADE_TARGET_ARM
constexpr NeonT Neon{Implementation::Init};

constexpr NeonFmaT NeonFma{Implementation::Init};

constexpr NeonFp16T NeonFp16{Implementation::Init};
#endif

#ifdef CORRADE_TARGET_WASM
constexpr Simd128T Simd128{Implementation::Init};
#endif

namespace Implementation {

template<unsigned i> struct Priority: Priority<i - 1> {};
template<> struct Priority<0> {};

enum: unsigned int {
    BaseTagMask = (1 << ExtraTagBitOffset) - 1,
    ExtraTagMask = 0xffffffffu & ~BaseTagMask,
    #ifdef CORRADE_TARGET_X86
    ExtraTagCount = 6,
    #else
    ExtraTagCount = 0,
    #endif
};

template<unsigned int value, unsigned int otherValue> struct IsTagConversionAllowed {
    enum: bool { Value =
        !((value & BaseTagMask) & ((value & BaseTagMask) - 1)) &&
        !((otherValue & BaseTagMask) & ((otherValue & BaseTagMask) - 1)) &&
        (otherValue & BaseTagMask) >= (value & BaseTagMask) &&
        ((otherValue & value) & ExtraTagMask) == (value & ExtraTagMask)
    };
};
template<unsigned int value, unsigned int otherIndex> struct IsSingleTagConversionAllowed {
    enum: bool { Value =
        !((value & BaseTagMask) & ((value & BaseTagMask) - 1)) &&
        (otherIndex & BaseTagMask) >= (value & BaseTagMask) &&
        ((otherIndex & value) & ExtraTagMask) == (value & ExtraTagMask)
    };
};

template<unsigned int value> struct Tags {
    enum: unsigned int { Value = value };

    constexpr explicit Tags(InitT) {}

    template<unsigned int otherValue> constexpr Tags(Tags<otherValue>, typename std::enable_if<IsTagConversionAllowed<value, otherValue>::Value>::type* = {}) {}

    template<class T> constexpr Tags(T, typename std::enable_if<IsSingleTagConversionAllowed<Value, TypeTraits<T>::Index>::Value>::type* = {}) {}

    template<unsigned int otherValue> constexpr Tags<value | otherValue> operator|(Tags<otherValue>) const {
        return Tags<value | otherValue>{Init};
    }
    template<class U> constexpr Tags<value | TypeTraits<U>::Index> operator|(U) const {
        return Tags<value | TypeTraits<U>::Index>{Init};
    }
    template<unsigned int otherValue> constexpr Tags<value & otherValue> operator&(Tags<otherValue>) const {
        return Tags<value & otherValue>{Init};
    }
    template<class U> constexpr Tags<value & TypeTraits<U>::Index> operator&(U) const {
        return Tags<value & TypeTraits<U>::Index>{Init};
    }
    template<unsigned int otherValue> constexpr Tags<value ^ otherValue> operator^(Tags<otherValue>) const {
        return Tags<value ^ otherValue>{Init};
    }
    template<class U> constexpr Tags<value ^ TypeTraits<U>::Index> operator^(U) const {
        return Tags<value ^ TypeTraits<U>::Index>{Init};
    }
    constexpr Tags<~value> operator~() const {
        return Tags<~value>{Init};
    }
    constexpr explicit operator bool() const {
        return bool(value);
    }
    constexpr operator unsigned int() const { return value; }
};

template<class T> constexpr Tags<TypeTraits<T>::Index> tags(T) {
    return Tags<TypeTraits<T>::Index>{Init};
}
template<unsigned int value> constexpr Tags<value> tags(Tags<value> tags) {
    return tags;
}

template<unsigned short A> struct BitIndex {
    enum: unsigned short { Value = 1 + BitIndex<(A >> 1)>::Value };
};
template<> struct BitIndex<0> {
    enum: unsigned short { Value = 0 };
};

template<unsigned short A> struct BitCount {
    enum: unsigned short {
        Bits1 = 0x5555,
        Bits2 = 0x3333,
        Bits4 = 0x0f0f,
        Bits8 = 0x00ff,

        B0 = (A >> 0) & Bits1,
        B1 = (A >> 1) & Bits1,
        C = B0 + B1,
        D0 = (C >> 0) & Bits2,
        D2 = (C >> 2) & Bits2,
        E = D0 + D2,
        F0 = (E >> 0) & Bits4,
        F4 = (E >> 4) & Bits4,
        G = F0 + F4,
        H0 = (G >> 0) & Bits8,
        H8 = (G >> 8) & Bits8,
        Value = H0 + H8
    };
};

template<class T> Priority<(static_cast<unsigned int>(TypeTraits<T>::Index) & ExtraTagMask ? 1 : BitIndex<static_cast<unsigned int>(TypeTraits<T>::Index) & BaseTagMask>::Value*(ExtraTagCount + 1))> constexpr priority(T) {
    return {};
}
template<unsigned int value> Priority<(BitIndex<value & BaseTagMask>::Value*(ExtraTagCount + 1) + BitCount<((value & ExtraTagMask) >> ExtraTagBitOffset)>::Value)> constexpr priority(Tags<value>) {
    static_assert(!((value & BaseTagMask) & ((value & BaseTagMask) - 1)), "more than one base tag used");
    static_assert(((value & ExtraTagMask) >> ExtraTagBitOffset) < (1 << static_cast<unsigned int>(ExtraTagCount)), "extra tag out of expected bounds");
    return {};
}

}

typedef
    #ifdef CORRADE_TARGET_X86
    #ifdef CORRADE_TARGET_AVX512F
    Avx512fT
    #elif defined(CORRADE_TARGET_AVX2)
    Avx2T
    #elif defined(CORRADE_TARGET_AVX)
    AvxT
    #elif defined(CORRADE_TARGET_SSE42)
    Sse42T
    #elif defined(CORRADE_TARGET_SSE41)
    Sse41T
    #elif defined(CORRADE_TARGET_SSSE3)
    Ssse3T
    #elif defined(CORRADE_TARGET_SSE3)
    Sse3T
    #elif defined(CORRADE_TARGET_SSE2)
    Sse2T
    #else
    ScalarT
    #endif

    #elif defined(CORRADE_TARGET_ARM)
    #ifdef CORRADE_TARGET_NEON_FP16
    NeonFp16T
    #elif defined(CORRADE_TARGET_NEON_FMA)
    NeonFmaT
    #elif defined(CORRADE_TARGET_NEON)
    NeonT
    #else
    ScalarT
    #endif

    #elif defined(CORRADE_TARGET_WASM)
    #ifdef CORRADE_TARGET_SIMD128
    Simd128T
    #else
    ScalarT
    #endif

    #else
    ScalarT
    #endif
    DefaultBaseT;

typedef Implementation::Tags<0
    #ifdef CORRADE_TARGET_X86
    #ifdef CORRADE_TARGET_POPCNT
    |TypeTraits<PopcntT>::Index
    #endif
    #ifdef CORRADE_TARGET_LZCNT
    |TypeTraits<LzcntT>::Index
    #endif
    #ifdef CORRADE_TARGET_BMI1
    |TypeTraits<Bmi1T>::Index
    #endif
    #ifdef CORRADE_TARGET_BMI2
    |TypeTraits<Bmi2T>::Index
    #endif
    #ifdef CORRADE_TARGET_AVX_FMA
    |TypeTraits<AvxFmaT>::Index
    #endif
    #ifdef CORRADE_TARGET_AVX_F16C
    |TypeTraits<AvxF16cT>::Index
    #endif
    #endif
    > DefaultExtraT;

typedef Implementation::Tags<static_cast<unsigned int>(TypeTraits<DefaultBaseT>::Index)|DefaultExtraT::Value> DefaultT;

constexpr DefaultBaseT DefaultBase{Implementation::Init};

constexpr DefaultExtraT DefaultExtra{Implementation::Init};

constexpr DefaultT Default{Implementation::Init};

template<class T> constexpr T tag() { return T{Implementation::Init}; }

#if defined(CORRADE_TARGET_ARM) && ((defined(__linux__) && !(defined(CORRADE_TARGET_ANDROID) && __ANDROID_API__ < 18)) || defined(__FreeBSD__))
namespace Implementation {
    Features runtimeFeatures(unsigned long caps);
}
#endif

class Features {
    public:
        constexpr explicit Features() noexcept: _data{} {}

        template<class T, class = decltype(TypeTraits<T>::Index)> constexpr /*implicit*/ Features(T) noexcept: _data{TypeTraits<T>::Index} {
            static_assert(((static_cast<unsigned int>(TypeTraits<T>::Index) & Implementation::ExtraTagMask) >> Implementation::ExtraTagBitOffset) < (1 << static_cast<unsigned int>(Implementation::ExtraTagCount)),
                "extra tag out of expected bounds");
        }

        template<unsigned int value> constexpr /*implicit*/ Features(Implementation::Tags<value>) noexcept: _data{value} {}

        constexpr bool operator==(Features other) const {
            return _data == other._data;
        }

        constexpr bool operator!=(Features other) const {
            return _data != other._data;
        }

        constexpr bool operator>=(Features other) const {
            return (_data & other._data) == other._data;
        }

        constexpr bool operator<=(Features other) const {
            return (_data & other._data) == _data;
        }

        constexpr Features operator|(Features other) const {
            return Features{_data | other._data};
        }

        Features& operator|=(Features other) {
            _data |= other._data;
            return *this;
        }

        constexpr Features operator&(Features other) const {
            return Features{_data & other._data};
        }

        Features& operator&=(Features other) {
            _data &= other._data;
            return *this;
        }

        constexpr Features operator^(Features other) const {
            return Features{_data ^ other._data};
        }

        Features& operator^=(Features other) {
            _data ^= other._data;
            return *this;
        }

        constexpr Features operator~() const {
            return Features{~_data};
        }

        constexpr explicit operator bool() const { return _data; }

        constexpr explicit operator unsigned int() const { return _data; }

    private:
        template<class> friend constexpr Features features();
        friend constexpr Features compiledFeatures();
        #if (defined(CORRADE_TARGET_X86) && (defined(CORRADE_TARGET_MSVC) || defined(CORRADE_TARGET_GCC))) || (defined(CORRADE_TARGET_ARM) && defined(CORRADE_TARGET_APPLE))
        friend
        #ifdef CORRADE_TARGET_ARM
        CORRADE_UTILITY_EXPORT
        #endif
        Features runtimeFeatures();
        #endif
        #if defined(CORRADE_TARGET_ARM) && ((defined(__linux__) && !(defined(CORRADE_TARGET_ANDROID) && __ANDROID_API__ < 18)) || defined(__FreeBSD__))
        friend Features Implementation::runtimeFeatures(unsigned long);
        #endif

        constexpr explicit Features(unsigned int data) noexcept: _data{data} {}

        unsigned int _data;
};

template<class T> constexpr Features features() {
    return Features{TypeTraits<T>::Index};
}

template<class T, class = decltype(TypeTraits<T>::Index)> constexpr bool operator==(T a, Features b) {
    return Features(a) == b;
}

template<class T, class U, class = decltype(TypeTraits<T>::Index), class = decltype(TypeTraits<U>::Index)> constexpr bool operator==(T, U) {
    return static_cast<unsigned int>(TypeTraits<T>::Index) == TypeTraits<U>::Index;
}

template<class T, class = decltype(TypeTraits<T>::Index)> constexpr bool operator!=(T a, Features b) {
    return Features(a) != b;
}

template<class T, class U, class = decltype(TypeTraits<T>::Index), class = decltype(TypeTraits<U>::Index)> constexpr bool operator!=(T, U) {
    return static_cast<unsigned int>(TypeTraits<T>::Index) != TypeTraits<U>::Index;
}

template<class T, class = decltype(TypeTraits<T>::Index)> constexpr bool operator>=(T a, Features b) {
    return Features(a) >= b;
}

template<class T, class = decltype(TypeTraits<T>::Index)> constexpr bool operator<=(T a, Features b) {
    return Features(a) <= b;
}

template<class T, class = decltype(TypeTraits<T>::Index)> constexpr Features operator|(T a, Features b) {
    return b | a;
}

template<class T, class U> constexpr Implementation::Tags<static_cast<unsigned int>(TypeTraits<T>::Index) | TypeTraits<U>::Index> operator|(T, U) {
    return Implementation::Tags<static_cast<unsigned int>(TypeTraits<T>::Index) | TypeTraits<U>::Index>{Implementation::Init};
}
template<class T, unsigned int value> constexpr Implementation::Tags<TypeTraits<T>::Index | value> operator|(T, Implementation::Tags<value>) {
    return Implementation::Tags<TypeTraits<T>::Index | value>{Implementation::Init};
}

template<class T, class = decltype(TypeTraits<T>::Index)> constexpr Features operator&(T a, Features b) {
    return b & a;
}

template<class T, class U> constexpr Implementation::Tags<static_cast<unsigned int>(TypeTraits<T>::Index) & TypeTraits<U>::Index> operator&(T, U) {
    return Implementation::Tags<static_cast<unsigned int>(TypeTraits<T>::Index) & TypeTraits<U>::Index>{Implementation::Init};
}
template<class T, unsigned int value> constexpr Implementation::Tags<TypeTraits<T>::Index & value> operator&(T, Implementation::Tags<value>) {
    return Implementation::Tags<TypeTraits<T>::Index & value>{Implementation::Init};
}

template<class T, class = decltype(TypeTraits<T>::Index)> constexpr Features operator^(T a, Features b) {
    return b ^ a;
}

template<class T, class U> constexpr Implementation::Tags<static_cast<unsigned int>(TypeTraits<T>::Index) ^ TypeTraits<U>::Index> operator^(T, U) {
    return Implementation::Tags<static_cast<unsigned int>(TypeTraits<T>::Index) ^ TypeTraits<U>::Index>{Implementation::Init};
}
template<class T, unsigned int value> constexpr Implementation::Tags<TypeTraits<T>::Index ^ value> operator^(T, Implementation::Tags<value>) {
    return Implementation::Tags<TypeTraits<T>::Index ^ value>{Implementation::Init};
}

template<class T> constexpr Implementation::Tags<~TypeTraits<T>::Index> operator~(T) {
    return Implementation::Tags<~TypeTraits<T>::Index>{Implementation::Init};
}

constexpr Features compiledFeatures() {
    return Features{0
        #ifdef CORRADE_TARGET_X86
        #ifdef CORRADE_TARGET_SSE2
        |TypeTraits<Sse2T>::Index
        #endif
        #ifdef CORRADE_TARGET_SSE3
        |TypeTraits<Sse3T>::Index
        #endif
        #ifdef CORRADE_TARGET_SSSE3
        |TypeTraits<Ssse3T>::Index
        #endif
        #ifdef CORRADE_TARGET_SSE41
        |TypeTraits<Sse41T>::Index
        #endif
        #ifdef CORRADE_TARGET_SSE42
        |TypeTraits<Sse42T>::Index
        #endif
        #ifdef CORRADE_TARGET_POPCNT
        |TypeTraits<PopcntT>::Index
        #endif
        #ifdef CORRADE_TARGET_LZCNT
        |TypeTraits<LzcntT>::Index
        #endif
        #ifdef CORRADE_TARGET_BMI1
        |TypeTraits<Bmi1T>::Index
        #endif
        #ifdef CORRADE_TARGET_BMI2
        |TypeTraits<Bmi2T>::Index
        #endif
        #ifdef CORRADE_TARGET_AVX
        |TypeTraits<AvxT>::Index
        #endif
        #ifdef CORRADE_TARGET_AVX_FMA
        |TypeTraits<AvxFmaT>::Index
        #endif
        #ifdef CORRADE_TARGET_AVX_F16C
        |TypeTraits<AvxF16cT>::Index
        #endif
        #ifdef CORRADE_TARGET_AVX2
        |TypeTraits<Avx2T>::Index
        #endif
        #ifdef CORRADE_TARGET_AVX512F
        |TypeTraits<Avx512fT>::Index
        #endif

        #elif defined(CORRADE_TARGET_ARM)
        #ifdef CORRADE_TARGET_NEON
        |TypeTraits<NeonT>::Index
        #endif
        #ifdef CORRADE_TARGET_NEON_FMA
        |TypeTraits<NeonFmaT>::Index
        #endif
        #ifdef CORRADE_TARGET_NEON_FP16
        |TypeTraits<NeonFp16T>::Index
        #endif

        #elif defined(CORRADE_TARGET_WASM)
        #ifdef CORRADE_TARGET_SIMD128
        |TypeTraits<Simd128T>::Index
        #endif
        #endif
        };
}

#if (defined(CORRADE_TARGET_X86) && (defined(CORRADE_TARGET_MSVC) || defined(CORRADE_TARGET_GCC))) || (defined(CORRADE_TARGET_ARM) && ((defined(__linux__) && !(defined(CORRADE_TARGET_ANDROID) && __ANDROID_API__ < 18)) || defined(CORRADE_TARGET_APPLE) || defined(__FreeBSD__)))
#ifdef CORRADE_TARGET_ARM
CORRADE_UTILITY_EXPORT
#endif
Features runtimeFeatures();
#else
constexpr Features runtimeFeatures() { return compiledFeatures(); }
#endif

#define CORRADE_CPU_DECLARE(tag) decltype(Corrade::Cpu::Implementation::tags(tag)), decltype(Corrade::Cpu::Implementation::priority(tag))

#define CORRADE_CPU_SELECT(tag) tag, Corrade::Cpu::Implementation::priority(tag)

#define _CORRADE_HELPER_PICK(_0, _1, _2, _3, _4, _5, _6, _7, macroName, ...) macroName

#ifdef CORRADE_TARGET_X86
#define CORRADE_CPU_DISPATCHER_BASE(function)                               \
    decltype(function(Corrade::Cpu::Scalar)) function(Corrade::Cpu::Features features) { \
        if(features & Corrade::Cpu::Avx512f)                                \
            return function(Corrade::Cpu::Avx512f);                         \
        if(features & Corrade::Cpu::Avx2)                                   \
            return function(Corrade::Cpu::Avx2);                            \
        if(features & Corrade::Cpu::Avx)                                    \
            return function(Corrade::Cpu::Avx);                             \
        if(features & Corrade::Cpu::Sse42)                                  \
            return function(Corrade::Cpu::Sse42);                           \
        if(features & Corrade::Cpu::Sse41)                                  \
            return function(Corrade::Cpu::Sse41);                           \
        if(features & Corrade::Cpu::Ssse3)                                  \
            return function(Corrade::Cpu::Ssse3);                           \
        if(features & Corrade::Cpu::Sse3)                                   \
            return function(Corrade::Cpu::Sse3);                            \
        if(features & Corrade::Cpu::Sse2)                                   \
            return function(Corrade::Cpu::Sse2);                            \
        return function(Corrade::Cpu::Scalar);                              \
    }
#elif defined(CORRADE_TARGET_ARM)
#define CORRADE_CPU_DISPATCHER_BASE(function)                               \
    decltype(function(Corrade::Cpu::Scalar)) function(Corrade::Cpu::Features features) { \
        if(features & Corrade::Cpu::NeonFp16)                               \
            return function(Corrade::Cpu::NeonFp16);                        \
        if(features & Corrade::Cpu::NeonFma)                                \
            return function(Corrade::Cpu::NeonFma);                         \
        if(features & Corrade::Cpu::Neon)                                   \
            return function(Corrade::Cpu::Neon);                            \
        return function(Corrade::Cpu::Scalar);                              \
    }
#elif defined(CORRADE_TARGET_WASM)
#define CORRADE_CPU_DISPATCHER_BASE(function)                               \
    decltype(function(Corrade::Cpu::Scalar)) function(Corrade::Cpu::Features features) { \
        if(features & Corrade::Cpu::Simd128)                                \
            return function(Corrade::Cpu::Simd128);                         \
        return function(Corrade::Cpu::Scalar);                              \
    }
#else
#define CORRADE_CPU_DISPATCHER_BASE(function)                               \
    decltype(function(Corrade::Cpu::Scalar)) function(Corrade::Cpu::Features features) { \
        return function(Corrade::Cpu::Scalar);                              \
    }
#endif

#ifdef CORRADE_TARGET_X86
#define _CORRADE_CPU_DISPATCHER_IMPLEMENTATION(function, extra)             \
    if(features >= (Corrade::Cpu::Avx512f extra))                           \
        return function(CORRADE_CPU_SELECT(Corrade::Cpu::Avx512f extra));   \
    if(features >= (Corrade::Cpu::Avx2 extra))                              \
        return function(CORRADE_CPU_SELECT(Corrade::Cpu::Avx2 extra));      \
    if(features >= (Corrade::Cpu::Avx extra))                               \
        return function(CORRADE_CPU_SELECT(Corrade::Cpu::Avx extra));       \
    if(features >= (Corrade::Cpu::Sse42 extra))                             \
        return function(CORRADE_CPU_SELECT(Corrade::Cpu::Sse42 extra));     \
    if(features >= (Corrade::Cpu::Sse41 extra))                             \
        return function(CORRADE_CPU_SELECT(Corrade::Cpu::Sse41 extra));     \
    if(features >= (Corrade::Cpu::Ssse3 extra))                             \
        return function(CORRADE_CPU_SELECT(Corrade::Cpu::Ssse3 extra));     \
    if(features >= (Corrade::Cpu::Sse3 extra))                              \
        return function(CORRADE_CPU_SELECT(Corrade::Cpu::Sse3 extra));      \
    if(features >= (Corrade::Cpu::Sse2 extra))                              \
        return function(CORRADE_CPU_SELECT(Corrade::Cpu::Sse2 extra));      \
    return function(CORRADE_CPU_SELECT(Corrade::Cpu::Scalar extra));
#elif defined(CORRADE_TARGET_ARM)
#define _CORRADE_CPU_DISPATCHER_IMPLEMENTATION(function, extra)             \
    if(features >= (Corrade::Cpu::NeonFp16 extra))                          \
        return function(CORRADE_CPU_SELECT(Corrade::Cpu::NeonFp16 extra));  \
    if(features >= (Corrade::Cpu::NeonFma extra))                           \
        return function(CORRADE_CPU_SELECT(Corrade::Cpu::NeonFma extra));   \
    if(features >= (Corrade::Cpu::Neon extra))                              \
        return function(CORRADE_CPU_SELECT(Corrade::Cpu::Neon extra));      \
    return function(CORRADE_CPU_SELECT(Corrade::Cpu::Scalar extra));
#elif defined(CORRADE_TARGET_WASM)
#define _CORRADE_CPU_DISPATCHER_IMPLEMENTATION(function, extra)             \
    if(features >= (Corrade::Cpu::Simd128 extra))                           \
        return function(CORRADE_CPU_SELECT(Corrade::Cpu::Simd128 extra));   \
    return function(CORRADE_CPU_SELECT(Corrade::Cpu::Scalar extra));
#else
#define _CORRADE_CPU_DISPATCHER_IMPLEMENTATION(function, extra)             \
    return function(CORRADE_CPU_SELECT(Corrade::Cpu::Scalar extra));
#endif

#define _CORRADE_CPU_DISPATCHER0(function)                                  \
    decltype(function(CORRADE_CPU_SELECT(Corrade::Cpu::Scalar))) function(Corrade::Cpu::Features features) { \
        _CORRADE_CPU_DISPATCHER_IMPLEMENTATION(function, )                  \
    }

#define _CORRADE_CPU_DISPATCHERn(function, ...)                             \
    template<unsigned int value> CORRADE_ALWAYS_INLINE decltype(function(CORRADE_CPU_SELECT(Corrade::Cpu::Scalar))) function ## Internal(Corrade::Cpu::Features features, Corrade::Cpu::Implementation::Tags<value>) { \
        _CORRADE_CPU_DISPATCHER_IMPLEMENTATION(function, |Corrade::Cpu::Implementation::Tags<value>{Corrade::Cpu::Implementation::Init}) \
    }                                                                       \
    template<unsigned int value, class First, class ...Next> CORRADE_ALWAYS_INLINE decltype(function(CORRADE_CPU_SELECT(Corrade::Cpu::Scalar))) function ## Internal(Corrade::Cpu::Features features, Corrade::Cpu::Implementation::Tags<value> extra, First first, Next... next) { \
        static_assert(!(static_cast<unsigned int>(Corrade::Cpu::Implementation::tags(First{Corrade::Cpu::Implementation::Init})) & Corrade::Cpu::Implementation::BaseTagMask), \
            "only extra instruction set tags should be explicitly listed"); \
        if(features & first)                                                \
            return function ## Internal(features, extra|first, next...);    \
        else                                                                \
            return function ## Internal(features, extra, next...);          \
    }                                                                       \
    decltype(function(CORRADE_CPU_SELECT(Corrade::Cpu::Scalar))) function(Corrade::Cpu::Features features) { \
        return function ## Internal(features, Corrade::Cpu::Implementation::Tags<0>{Corrade::Cpu::Implementation::Init}, __VA_ARGS__); \
    }

#if !defined(CORRADE_TARGET_MSVC) || defined(CORRADE_TARGET_CLANG_CL)
#define CORRADE_CPU_DISPATCHER(...)                               \
    _CORRADE_HELPER_PICK(__VA_ARGS__, _CORRADE_CPU_DISPATCHERn, _CORRADE_CPU_DISPATCHERn, _CORRADE_CPU_DISPATCHERn, _CORRADE_CPU_DISPATCHERn, _CORRADE_CPU_DISPATCHERn, _CORRADE_CPU_DISPATCHERn, _CORRADE_CPU_DISPATCHERn, _CORRADE_CPU_DISPATCHER0, )(__VA_ARGS__)
#else
#define _CORRADE_CPU_DISPATCHER_FFS_MSVC_EXPAND_THIS(x) x
#define CORRADE_CPU_DISPATCHER(...)                               \
    _CORRADE_CPU_DISPATCHER_FFS_MSVC_EXPAND_THIS( _CORRADE_HELPER_PICK(__VA_ARGS__, _CORRADE_CPU_DISPATCHERn, _CORRADE_CPU_DISPATCHERn, _CORRADE_CPU_DISPATCHERn, _CORRADE_CPU_DISPATCHERn, _CORRADE_CPU_DISPATCHERn, _CORRADE_CPU_DISPATCHERn, _CORRADE_CPU_DISPATCHERn, _CORRADE_CPU_DISPATCHER0, )(__VA_ARGS__))
#endif

#define CORRADE_CPU_DISPATCHED_POINTER(dispatcher, ...)              \
    __VA_ARGS__ = dispatcher(Corrade::Cpu::runtimeFeatures());

#ifdef CORRADE_CPU_USE_IFUNC
#ifdef CORRADE_TARGET_CLANG
#ifndef CORRADE_TARGET_ARM
#define CORRADE_CPU_DISPATCHED_IFUNC(dispatcher, ...)                       \
    _Pragma("GCC diagnostic push")                                          \
    _Pragma("GCC diagnostic ignored \"-Wmissing-prototypes\"")              \
    extern "C" decltype(dispatcher(std::declval<Corrade::Cpu::Features>())) dispatcher() { \
        return dispatcher(Corrade::Cpu::runtimeFeatures());                 \
    }                                                                       \
    __VA_ARGS__ __attribute__((ifunc(#dispatcher)));                        \
    _Pragma("GCC diagnostic pop")
#else
#define CORRADE_CPU_DISPATCHED_IFUNC(dispatcher, ...)                       \
    _Pragma("GCC diagnostic push")                                          \
    _Pragma("GCC diagnostic ignored \"-Wmissing-prototypes\"")              \
    extern "C" decltype(dispatcher(std::declval<Corrade::Cpu::Features>())) dispatcher(unsigned long caps) { \
        return dispatcher(Corrade::Cpu::Implementation::runtimeFeatures(caps)); \
    }                                                                       \
    __VA_ARGS__ __attribute__((ifunc(#dispatcher)));                        \
    _Pragma("GCC diagnostic pop")
#endif
#elif defined(CORRADE_TARGET_GCC) && __GNUC__*100 + __GNUC_MINOR__ < 409
#ifndef CORRADE_TARGET_ARM
#define CORRADE_CPU_DISPATCHED_IFUNC(dispatcher, ...)                       \
    extern "C" { CORRADE_NEVER_INLINE static decltype(dispatcher(std::declval<Corrade::Cpu::Features>())) dispatcher ## Ifunc() {   \
        return dispatcher(Corrade::Cpu::runtimeFeatures());                 \
    }}                                                                      \
    __VA_ARGS__ __attribute__((ifunc(#dispatcher "Ifunc")));
#else
#define CORRADE_CPU_DISPATCHED_IFUNC(dispatcher, ...)                       \
    extern "C" { CORRADE_NEVER_INLINE static decltype(dispatcher(std::declval<Corrade::Cpu::Features>())) dispatcher ## Ifunc(unsigned long caps) { \
        return dispatcher(Corrade::Cpu::Implementation::runtimeFeatures(caps)); \
    }}                                                                      \
    __VA_ARGS__ __attribute__((ifunc(#dispatcher "Ifunc")));
#endif
#else
#ifndef CORRADE_TARGET_ARM
#define CORRADE_CPU_DISPATCHED_IFUNC(dispatcher, ...)                       \
    extern "C" { static decltype(dispatcher(std::declval<Corrade::Cpu::Features>())) dispatcher() { \
        return dispatcher(Corrade::Cpu::runtimeFeatures());                 \
    }}                                                                      \
    __VA_ARGS__ __attribute__((ifunc(#dispatcher)));
#else
#define CORRADE_CPU_DISPATCHED_IFUNC(dispatcher, ...)                       \
    extern "C" { static decltype(dispatcher(std::declval<Corrade::Cpu::Features>())) dispatcher(unsigned long caps) { \
        return dispatcher(Corrade::Cpu::Implementation::runtimeFeatures(caps)); \
    }}                                                                      \
    __VA_ARGS__ __attribute__((ifunc(#dispatcher)));
#endif
#endif
#endif

#ifdef CORRADE_TARGET_X86
#ifdef CORRADE_TARGET_SSE2
#define CORRADE_ENABLE_SSE2
#if (defined(CORRADE_TARGET_GCC) && __GNUC__ < 12) || defined(CORRADE_TARGET_CLANG)
#define _CORRADE_ENABLE_SSE2
#endif
#elif defined(CORRADE_TARGET_GCC) || defined(CORRADE_TARGET_CLANG_CL)
#define CORRADE_ENABLE_SSE2 __attribute__((__target__("sse2")))
#if (defined(CORRADE_TARGET_GCC) && __GNUC__ < 12) || defined(CORRADE_TARGET_CLANG)
#define _CORRADE_ENABLE_SSE2 "sse2",
#endif
#elif defined(CORRADE_TARGET_MSVC)
#define CORRADE_ENABLE_SSE2
#endif

#ifdef CORRADE_TARGET_SSE3
#define CORRADE_ENABLE_SSE3
#if (defined(CORRADE_TARGET_GCC) && __GNUC__ < 12) || defined(CORRADE_TARGET_CLANG)
#define _CORRADE_ENABLE_SSE3
#endif
#elif defined(CORRADE_TARGET_GCC) || defined(CORRADE_TARGET_CLANG_CL)
#define CORRADE_ENABLE_SSE3 __attribute__((__target__("sse3")))
#if (defined(CORRADE_TARGET_GCC) && __GNUC__ < 12) || defined(CORRADE_TARGET_CLANG)
#define _CORRADE_ENABLE_SSE3 "sse3",
#endif
#elif defined(CORRADE_TARGET_MSVC)
#define CORRADE_ENABLE_SSE3
#endif

#ifdef CORRADE_TARGET_SSSE3
#define CORRADE_ENABLE_SSSE3
#if (defined(CORRADE_TARGET_GCC) && __GNUC__ < 12) || defined(CORRADE_TARGET_CLANG)
#define _CORRADE_ENABLE_SSSE3
#endif
#elif defined(CORRADE_TARGET_GCC) || defined(CORRADE_TARGET_CLANG_CL)
#define CORRADE_ENABLE_SSSE3 __attribute__((__target__("ssse3")))
#if (defined(CORRADE_TARGET_GCC) && __GNUC__ < 12) || defined(CORRADE_TARGET_CLANG)
#define _CORRADE_ENABLE_SSSE3 "ssse3",
#endif
#elif defined(CORRADE_TARGET_MSVC)
#define CORRADE_ENABLE_SSSE3
#endif

#ifdef CORRADE_TARGET_SSE41
#define CORRADE_ENABLE_SSE41
#if (defined(CORRADE_TARGET_GCC) && __GNUC__ < 12) || defined(CORRADE_TARGET_CLANG)
#define _CORRADE_ENABLE_SSE41
#endif
#elif (defined(CORRADE_TARGET_GCC) && __GNUC__*100 + __GNUC_MINOR__ >= 409) || defined(CORRADE_TARGET_CLANG) /* also matches clang-cl */
#define CORRADE_ENABLE_SSE41 __attribute__((__target__("sse4.1")))
#if (defined(CORRADE_TARGET_GCC) && __GNUC__ < 12) || defined(CORRADE_TARGET_CLANG)
#define _CORRADE_ENABLE_SSE41 "sse4.1",
#endif
#elif defined(CORRADE_TARGET_MSVC)
#define CORRADE_ENABLE_SSE41
#endif

#ifdef CORRADE_TARGET_SSE42
#define CORRADE_ENABLE_SSE42
#if (defined(CORRADE_TARGET_GCC) && __GNUC__ < 12) || defined(CORRADE_TARGET_CLANG)
#define _CORRADE_ENABLE_SSE42
#endif
#elif defined(CORRADE_TARGET_GCC) || defined(CORRADE_TARGET_CLANG_CL)
#define CORRADE_ENABLE_SSE42 __attribute__((__target__("sse4.2")))
#if (defined(CORRADE_TARGET_GCC) && __GNUC__ < 12) || defined(CORRADE_TARGET_CLANG)
#define _CORRADE_ENABLE_SSE42 "sse4.2",
#endif
#elif defined(CORRADE_TARGET_MSVC)
#define CORRADE_ENABLE_SSE42
#endif

#ifdef CORRADE_TARGET_POPCNT
#define CORRADE_ENABLE_POPCNT
#if (defined(CORRADE_TARGET_GCC) && __GNUC__ < 12) || defined(CORRADE_TARGET_CLANG)
#define _CORRADE_ENABLE_POPCNT
#endif
#elif (defined(CORRADE_TARGET_GCC) && __GNUC__*100 + __GNUC_MINOR__ >= 409) || defined(CORRADE_TARGET_CLANG) /* matches clang-cl */
#define CORRADE_ENABLE_POPCNT __attribute__((__target__("popcnt")))
#if (defined(CORRADE_TARGET_GCC) && __GNUC__ < 12) || defined(CORRADE_TARGET_CLANG)
#define _CORRADE_ENABLE_POPCNT "popcnt",
#endif
#elif defined(CORRADE_TARGET_MSVC)
#define CORRADE_ENABLE_POPCNT
#endif

#ifdef CORRADE_TARGET_LZCNT
#define CORRADE_ENABLE_LZCNT
#if (defined(CORRADE_TARGET_GCC) && __GNUC__ < 12) || defined(CORRADE_TARGET_CLANG)
#define _CORRADE_ENABLE_LZCNT
#endif
#elif defined(CORRADE_TARGET_GCC) && (__GNUC__*100 + __GNUC_MINOR__ >= 409 || defined(CORRADE_TARGET_CLANG)) /* does not match clang-cl */
#define CORRADE_ENABLE_LZCNT __attribute__((__target__("lzcnt")))
#if (defined(CORRADE_TARGET_GCC) && __GNUC__ < 12) || defined(CORRADE_TARGET_CLANG)
#define _CORRADE_ENABLE_LZCNT "lzcnt",
#endif
#elif defined(CORRADE_TARGET_MSVC) && !defined(CORRADE_TARGET_CLANG_CL)
#define CORRADE_ENABLE_LZCNT
#endif

#ifdef CORRADE_TARGET_BMI1
#define CORRADE_ENABLE_BMI1
#if (defined(CORRADE_TARGET_GCC) && __GNUC__ < 12) || defined(CORRADE_TARGET_CLANG)
#define _CORRADE_ENABLE_BMI1
#endif
#elif defined(CORRADE_TARGET_GCC) && (__GNUC__*100 + __GNUC_MINOR__ >= 409 || defined(CORRADE_TARGET_CLANG)) /* does not match clang-cl */
#define CORRADE_ENABLE_BMI1 __attribute__((__target__("bmi")))
#if (defined(CORRADE_TARGET_GCC) && __GNUC__ < 12) || defined(CORRADE_TARGET_CLANG)
#define _CORRADE_ENABLE_BMI1 "bmi",
#endif
#elif defined(CORRADE_TARGET_MSVC) && !defined(CORRADE_TARGET_CLANG_CL)
#define CORRADE_ENABLE_BMI1
#endif

#ifdef CORRADE_TARGET_BMI2
#define CORRADE_ENABLE_BMI2
#if (defined(CORRADE_TARGET_GCC) && __GNUC__ < 12) || defined(CORRADE_TARGET_CLANG)
#define _CORRADE_ENABLE_BMI2
#endif
#elif defined(CORRADE_TARGET_GCC) && (__GNUC__*100 + __GNUC_MINOR__ >= 409 || defined(CORRADE_TARGET_CLANG)) /* does not match clang-cl */
#define CORRADE_ENABLE_BMI2 __attribute__((__target__("bmi2")))
#if (defined(CORRADE_TARGET_GCC) && __GNUC__ < 12) || defined(CORRADE_TARGET_CLANG)
#define _CORRADE_ENABLE_BMI2 "bmi2",
#endif
#elif defined(CORRADE_TARGET_MSVC) && !defined(CORRADE_TARGET_CLANG_CL)
#define CORRADE_ENABLE_BMI2
#endif

#ifdef CORRADE_TARGET_AVX
#define CORRADE_ENABLE_AVX
#if (defined(CORRADE_TARGET_GCC) && __GNUC__ < 12) || defined(CORRADE_TARGET_CLANG)
#define _CORRADE_ENABLE_AVX
#endif
#elif defined(CORRADE_TARGET_GCC) /* does not match clang-cl */
#define CORRADE_ENABLE_AVX __attribute__((__target__("avx")))
#if (defined(CORRADE_TARGET_GCC) && __GNUC__ < 12) || defined(CORRADE_TARGET_CLANG)
#define _CORRADE_ENABLE_AVX "avx",
#endif
#elif defined(CORRADE_TARGET_MSVC) && !defined(CORRADE_TARGET_CLANG_CL)
#define CORRADE_ENABLE_AVX
#endif

#ifdef CORRADE_TARGET_AVX_F16C
#define CORRADE_ENABLE_AVX_F16C
#if (defined(CORRADE_TARGET_GCC) && __GNUC__ < 12) || defined(CORRADE_TARGET_CLANG)
#define _CORRADE_ENABLE_AVX_F16C
#endif
#elif defined(CORRADE_TARGET_GCC) && (__GNUC__*100 + __GNUC_MINOR__ >= 409 || defined(CORRADE_TARGET_CLANG)) /* does not match clang-cl */
#define CORRADE_ENABLE_AVX_F16C __attribute__((__target__("f16c")))
#if (defined(CORRADE_TARGET_GCC) && __GNUC__ < 12) || defined(CORRADE_TARGET_CLANG)
#define _CORRADE_ENABLE_AVX_F16C "f16c",
#endif
#elif defined(CORRADE_TARGET_MSVC) && !defined(CORRADE_TARGET_CLANG_CL)
#define CORRADE_ENABLE_AVX_F16C
#endif

#ifdef CORRADE_TARGET_AVX_FMA
#define CORRADE_ENABLE_AVX_FMA
#if (defined(CORRADE_TARGET_GCC) && __GNUC__ < 12) || defined(CORRADE_TARGET_CLANG)
#define _CORRADE_ENABLE_AVX_FMA
#endif
#elif defined(CORRADE_TARGET_GCC) && (__GNUC__*100 + __GNUC_MINOR__ >= 409 || defined(CORRADE_TARGET_CLANG)) /* does not match clang-cl */
#define CORRADE_ENABLE_AVX_FMA __attribute__((__target__("fma")))
#if (defined(CORRADE_TARGET_GCC) && __GNUC__ < 12) || defined(CORRADE_TARGET_CLANG)
#define _CORRADE_ENABLE_AVX_FMA "fma",
#endif
#elif defined(CORRADE_TARGET_MSVC) && !defined(CORRADE_TARGET_CLANG_CL)
#define CORRADE_ENABLE_AVX_FMA
#endif

#ifdef CORRADE_TARGET_AVX2
#define CORRADE_ENABLE_AVX2
#if (defined(CORRADE_TARGET_GCC) && __GNUC__ < 12) || defined(CORRADE_TARGET_CLANG)
#define _CORRADE_ENABLE_AVX2
#endif
#elif defined(CORRADE_TARGET_GCC) /* does not match clang-cl */
#define CORRADE_ENABLE_AVX2 __attribute__((__target__("avx2")))
#if (defined(CORRADE_TARGET_GCC) && __GNUC__ < 12) || defined(CORRADE_TARGET_CLANG)
#define _CORRADE_ENABLE_AVX2 "avx2",
#endif
#elif defined(CORRADE_TARGET_MSVC) && !defined(CORRADE_TARGET_CLANG_CL)
#define CORRADE_ENABLE_AVX2
#endif

#ifdef CORRADE_TARGET_AVX512F
#define CORRADE_ENABLE_AVX512F
#if (defined(CORRADE_TARGET_GCC) && __GNUC__ < 12) || defined(CORRADE_TARGET_CLANG)
#define _CORRADE_ENABLE_AVX512F
#endif
#elif defined(CORRADE_TARGET_GCC) && (__GNUC__*100 + __GNUC_MINOR__ >= 409 || defined(CORRADE_TARGET_CLANG)) /* does not match clang-cl */
#define CORRADE_ENABLE_AVX512F __attribute__((__target__("avx512f")))
#if (defined(CORRADE_TARGET_GCC) && __GNUC__ < 12) || defined(CORRADE_TARGET_CLANG)
#define _CORRADE_ENABLE_AVX512F "avx512f",
#endif
#elif defined(CORRADE_TARGET_MSVC) && _MSC_VER >= 1911 && !defined(CORRADE_TARGET_CLANG_CL)
#define CORRADE_ENABLE_AVX512F
#endif
#endif

#ifdef CORRADE_TARGET_ARM
#ifdef CORRADE_TARGET_NEON
#define CORRADE_ENABLE_NEON
#if (defined(CORRADE_TARGET_GCC) && __GNUC__ < 12) || defined(CORRADE_TARGET_CLANG)
#define _CORRADE_ENABLE_NEON
#endif
#elif defined(CORRADE_TARGET_GCC) && !defined(CORRADE_TARGET_CLANG)
#define CORRADE_ENABLE_NEON __attribute__((__target__("fpu=neon")))
#if (defined(CORRADE_TARGET_GCC) && __GNUC__ < 12) || defined(CORRADE_TARGET_CLANG)
#define _CORRADE_ENABLE_NEON "fpu=neon",
#endif
#elif defined(CORRADE_TARGET_MSVC)
#define CORRADE_ENABLE_NEON
#endif

#ifdef CORRADE_TARGET_NEON_FMA
#define CORRADE_ENABLE_NEON_FMA
#if (defined(CORRADE_TARGET_GCC) && __GNUC__ < 12) || defined(CORRADE_TARGET_CLANG)
#define _CORRADE_ENABLE_NEON_FMA
#endif
#elif defined(CORRADE_TARGET_GCC) && !defined(CORRADE_TARGET_CLANG)
#define CORRADE_ENABLE_NEON_FMA __attribute__((__target__("fpu=neon-vfpv4")))
#if (defined(CORRADE_TARGET_GCC) && __GNUC__ < 12) || defined(CORRADE_TARGET_CLANG)
#define _CORRADE_ENABLE_NEON_FMA "fpu=neon-vfpv4",
#endif
#elif defined(CORRADE_TARGET_MSVC)
#define CORRADE_ENABLE_NEON_FMA
#endif

#ifdef CORRADE_TARGET_NEON_FP16
#define CORRADE_ENABLE_NEON_FP16
#if (defined(CORRADE_TARGET_GCC) && __GNUC__ < 12) || defined(CORRADE_TARGET_CLANG)
#define _CORRADE_ENABLE_NEON_FP16
#endif
#elif defined(CORRADE_TARGET_GCC) && !defined(CORRADE_TARGET_CLANG)
#define CORRADE_ENABLE_NEON_FP16 __attribute__((__target__("arch=armv8.2-a+fp16")))
#if (defined(CORRADE_TARGET_GCC) && __GNUC__ < 12) || defined(CORRADE_TARGET_CLANG)
#define _CORRADE_ENABLE_NEON_FP16 "arch=armv8.2-a+fp16",
#endif
#elif defined(CORRADE_TARGET_MSVC)
#define CORRADE_ENABLE_NEON_FP16
#endif
#endif

#ifdef CORRADE_TARGET_WASM
#ifdef CORRADE_TARGET_SIMD128
#define CORRADE_ENABLE_SIMD128
#if (defined(CORRADE_TARGET_GCC) && __GNUC__ < 12) || defined(CORRADE_TARGET_CLANG)
#define _CORRADE_ENABLE_SIMD128
#endif
#endif
#endif

#if (defined(CORRADE_TARGET_GCC) && __GNUC__ < 12) || defined(CORRADE_TARGET_CLANG)
#define _CORRADE_ENABLE_CONCATENATE0(unused)
#define _CORRADE_ENABLE_CONCATENATE1(v0, unused)                            \
    __attribute__((__target__(v0)))
#define _CORRADE_ENABLE_CONCATENATE2(v0, v1, unused)                        \
    __attribute__((__target__(v0 "," v1)))
#define _CORRADE_ENABLE_CONCATENATE3(v0, v1, v2, unused)                    \
    __attribute__((__target__(v0 "," v1 "," v2)))
#define _CORRADE_ENABLE_CONCATENATE4(v0, v1, v2, v3, unused)                \
    __attribute__((__target__(v0 "," v1 "," v2 "," v3)))
#define _CORRADE_ENABLE_CONCATENATE5(v0, v1, v2, v3, v4, unused)            \
    __attribute__((__target__(v0 "," v1 "," v2 "," v3 "," v4)))
#define _CORRADE_ENABLE_CONCATENATE6(v0, v1, v2, v3, v4, v5, unused)        \
    __attribute__((__target__(v0 "," v1 "," v2 "," v3 "," v4 "," v5)))
#define _CORRADE_ENABLE_CONCATENATE7(v0, v1, v2, v3, v4, v5, v6, unused)    \
    __attribute__((__target__(v0 "," v1 "," v2 "," v3 "," v4 "," v5 "," v6)))
#define _CORRADE_ENABLE_CONCATENATE(...)                                    \
    _CORRADE_HELPER_PICK(__VA_ARGS__, _CORRADE_ENABLE_CONCATENATE7, _CORRADE_ENABLE_CONCATENATE6, _CORRADE_ENABLE_CONCATENATE5, _CORRADE_ENABLE_CONCATENATE4, _CORRADE_ENABLE_CONCATENATE3, _CORRADE_ENABLE_CONCATENATE2, _CORRADE_ENABLE_CONCATENATE1, _CORRADE_ENABLE_CONCATENATE0, )(__VA_ARGS__)
#define _CORRADE_ENABLE1(v0)                                                \
    _CORRADE_ENABLE_CONCATENATE(                                            \
        _CORRADE_ENABLE_ ## v0                                              \
    )
#define _CORRADE_ENABLE2(v0, v1)                                            \
    _CORRADE_ENABLE_CONCATENATE(                                            \
        _CORRADE_ENABLE_ ## v0                                              \
        _CORRADE_ENABLE_ ## v1                                              \
    )
#define _CORRADE_ENABLE3(v0, v1, v2)                                        \
    _CORRADE_ENABLE_CONCATENATE(                                            \
        _CORRADE_ENABLE_ ## v0                                              \
        _CORRADE_ENABLE_ ## v1                                              \
        _CORRADE_ENABLE_ ## v2                                              \
    )
#define _CORRADE_ENABLE4(v0, v1, v2, v3)                                    \
    _CORRADE_ENABLE_CONCATENATE(                                            \
        _CORRADE_ENABLE_ ## v0                                              \
        _CORRADE_ENABLE_ ## v1                                              \
        _CORRADE_ENABLE_ ## v2                                              \
        _CORRADE_ENABLE_ ## v3                                              \
    )
#define _CORRADE_ENABLE5(v0, v1, v2, v3, v4)                                \
    _CORRADE_ENABLE_CONCATENATE(                                            \
        _CORRADE_ENABLE_ ## v0                                              \
        _CORRADE_ENABLE_ ## v1                                              \
        _CORRADE_ENABLE_ ## v2                                              \
        _CORRADE_ENABLE_ ## v3                                              \
        _CORRADE_ENABLE_ ## v4                                              \
    )
#define _CORRADE_ENABLE6(v0, v1, v2, v3, v4, v5)                            \
    _CORRADE_ENABLE_CONCATENATE(                                            \
        _CORRADE_ENABLE_ ## v0                                              \
        _CORRADE_ENABLE_ ## v1                                              \
        _CORRADE_ENABLE_ ## v2                                              \
        _CORRADE_ENABLE_ ## v3                                              \
        _CORRADE_ENABLE_ ## v4                                              \
        _CORRADE_ENABLE_ ## v5                                              \
    )
#define _CORRADE_ENABLE7(v0, v1, v2, v3, v4, v5, v6)                        \
    _CORRADE_ENABLE_CONCATENATE(                                            \
        _CORRADE_ENABLE_ ## v0                                              \
        _CORRADE_ENABLE_ ## v1                                              \
        _CORRADE_ENABLE_ ## v2                                              \
        _CORRADE_ENABLE_ ## v3                                              \
        _CORRADE_ENABLE_ ## v4                                              \
        _CORRADE_ENABLE_ ## v5                                              \
        _CORRADE_ENABLE_ ## v6                                              \
    )
#elif defined(CORRADE_TARGET_GCC) || !defined(CORRADE_TARGET_MSVC)
#define _CORRADE_ENABLE1(v0)                                                \
    _CORRADE_HELPER_PASTE2(CORRADE_ENABLE_, v0)
#define _CORRADE_ENABLE2(v0, v1)                                            \
    _CORRADE_HELPER_PASTE2(CORRADE_ENABLE_, v0)                             \
    _CORRADE_HELPER_PASTE2(CORRADE_ENABLE_, v1)
#define _CORRADE_ENABLE3(v0, v1, v2)                                        \
    _CORRADE_HELPER_PASTE2(CORRADE_ENABLE_, v0)                             \
    _CORRADE_HELPER_PASTE2(CORRADE_ENABLE_, v1)                             \
    _CORRADE_HELPER_PASTE2(CORRADE_ENABLE_, v2)
#define _CORRADE_ENABLE4(v0, v1, v2, v3)                                    \
    _CORRADE_HELPER_PASTE2(CORRADE_ENABLE_, v0)                             \
    _CORRADE_HELPER_PASTE2(CORRADE_ENABLE_, v1)                             \
    _CORRADE_HELPER_PASTE2(CORRADE_ENABLE_, v2)                             \
    _CORRADE_HELPER_PASTE2(CORRADE_ENABLE_, v3)
#define _CORRADE_ENABLE5(v0, v1, v2, v3, v4)                                \
    _CORRADE_HELPER_PASTE2(CORRADE_ENABLE_, v0)                             \
    _CORRADE_HELPER_PASTE2(CORRADE_ENABLE_, v1)                             \
    _CORRADE_HELPER_PASTE2(CORRADE_ENABLE_, v2)                             \
    _CORRADE_HELPER_PASTE2(CORRADE_ENABLE_, v3)                             \
    _CORRADE_HELPER_PASTE2(CORRADE_ENABLE_, v4)
#define _CORRADE_ENABLE6(v0, v1, v2, v3, v4, v5)                            \
    _CORRADE_HELPER_PASTE2(CORRADE_ENABLE_, v0)                             \
    _CORRADE_HELPER_PASTE2(CORRADE_ENABLE_, v1)                             \
    _CORRADE_HELPER_PASTE2(CORRADE_ENABLE_, v2)                             \
    _CORRADE_HELPER_PASTE2(CORRADE_ENABLE_, v3)                             \
    _CORRADE_HELPER_PASTE2(CORRADE_ENABLE_, v4)                             \
    _CORRADE_HELPER_PASTE2(CORRADE_ENABLE_, v5)
#define _CORRADE_ENABLE7(v0, v1, v2, v3, v4, v5, v6)                        \
    _CORRADE_HELPER_PASTE2(CORRADE_ENABLE_, v0)                             \
    _CORRADE_HELPER_PASTE2(CORRADE_ENABLE_, v1)                             \
    _CORRADE_HELPER_PASTE2(CORRADE_ENABLE_, v2)                             \
    _CORRADE_HELPER_PASTE2(CORRADE_ENABLE_, v3)                             \
    _CORRADE_HELPER_PASTE2(CORRADE_ENABLE_, v4)                             \
    _CORRADE_HELPER_PASTE2(CORRADE_ENABLE_, v5)                             \
    _CORRADE_HELPER_PASTE2(CORRADE_ENABLE_, v6)
#define _CORRADE_ENABLE8(v0, v1, v2, v3, v4, v5, v6, v7)                    \
    _CORRADE_HELPER_PASTE2(CORRADE_ENABLE_, v0)                             \
    _CORRADE_HELPER_PASTE2(CORRADE_ENABLE_, v1)                             \
    _CORRADE_HELPER_PASTE2(CORRADE_ENABLE_, v2)                             \
    _CORRADE_HELPER_PASTE2(CORRADE_ENABLE_, v3)                             \
    _CORRADE_HELPER_PASTE2(CORRADE_ENABLE_, v4)                             \
    _CORRADE_HELPER_PASTE2(CORRADE_ENABLE_, v5)                             \
    _CORRADE_HELPER_PASTE2(CORRADE_ENABLE_, v6)                             \
    _CORRADE_HELPER_PASTE2(CORRADE_ENABLE_, v7)
#endif

#if !defined(CORRADE_TARGET_MSVC) || defined(CORRADE_TARGET_CLANG_CL)
#define CORRADE_ENABLE(...) _CORRADE_HELPER_PICK(__VA_ARGS__, _CORRADE_ENABLE8, _CORRADE_ENABLE7, _CORRADE_ENABLE6, _CORRADE_ENABLE5, _CORRADE_ENABLE4, _CORRADE_ENABLE3, _CORRADE_ENABLE2, _CORRADE_ENABLE1, )(__VA_ARGS__)
#else
#define CORRADE_ENABLE(...)
#endif

#if defined(CORRADE_TARGET_X86) && (defined(CORRADE_TARGET_MSVC) || defined(CORRADE_TARGET_GCC))
namespace Implementation {
    inline void cpuid(int data[4], int leaf, int count) {
        #if defined(CORRADE_TARGET_GCC) || defined(CORRADE_TARGET_CLANG_CL)
        #ifdef CORRADE_TARGET_32BIT
        asm("cpuid":       \
            "=a"(data[0]), "=b"(data[1]), "=c"(data[2]), "=d"(data[3]): \
            "0"(leaf), "2"(count));
        #else
        asm("xchgq %%rbx,%q1\n" \
            "cpuid\n" \
            "xchgq %%rbx,%q1": \
            "=a"(data[0]), "=b"(data[1]), "=c"(data[2]), "=d"(data[3]): \
            "0"(leaf), "2"(count));
        #endif

        #elif defined(CORRADE_TARGET_MSVC)
        __cpuidex(data, leaf, count);
        #else
        #error
        #endif
    }
}

inline Features runtimeFeatures() {
    union {
        struct {
            unsigned int ax, bx, cx, dx;
        } e;
        int data[4];
    } cpuid{};

    Implementation::cpuid(cpuid.data, 1, 0);

    unsigned int out = 0;
    if(cpuid.e.dx & (1 << 26)) out |= TypeTraits<Sse2T>::Index;
    if(cpuid.e.cx & (1 <<  0)) out |= TypeTraits<Sse3T>::Index;
    if(cpuid.e.cx & (1 <<  9)) out |= TypeTraits<Ssse3T>::Index;
    if(cpuid.e.cx & (1 << 19)) out |= TypeTraits<Sse41T>::Index;
    if(cpuid.e.cx & (1 << 20)) out |= TypeTraits<Sse42T>::Index;

    if(cpuid.e.cx & (1 << 23)) out |= TypeTraits<PopcntT>::Index;

    if((cpuid.e.cx & (1 << 27)) &&
       (cpuid.e.cx & (1 << 28)))
    {

        #if defined(CORRADE_TARGET_GCC) || defined(CORRADE_TARGET_CLANG_CL)
        unsigned int a, d;
        __asm("xgetbv": "=a"(a), "=d"(d): "c"(0): );
        const unsigned long long xgetbv = a|(static_cast<unsigned long long>(d) << 32);

        #elif defined(CORRADE_TARGET_MSVC)
        const unsigned long long xgetbv = _xgetbv(0);
        #else
        #error
        #endif

        if((xgetbv & 0x06) == 0x06 /* XSTATE_SSE|XSTATE_YMM */) {
            out |= TypeTraits<AvxT>::Index;

            if(cpuid.e.cx & (1 << 29)) out |= TypeTraits<AvxF16cT>::Index;
            if(cpuid.e.cx & (1 << 12)) out |= TypeTraits<AvxFmaT>::Index;

            Implementation::cpuid(cpuid.data, 7, 0);
            if(cpuid.e.bx & (1 << 3)) out |= TypeTraits<Bmi1T>::Index;
            if(cpuid.e.bx & (1 << 5)) out |= TypeTraits<Avx2T>::Index;
            if(cpuid.e.bx & (1 << 8)) out |= TypeTraits<Bmi2T>::Index;
        }

        if((cpuid.e.bx & (1 << 16)) &&
           (xgetbv & 0xe6) == 0xe6)
        {
            out |= TypeTraits<Avx512fT>::Index;
        }
    }

    Implementation::cpuid(cpuid.data, 0x80000001, 0);
    if(cpuid.e.cx & (1 << 5)) out |= TypeTraits<LzcntT>::Index;

    return Features{out};
}
#endif

#if defined(CORRADE_TARGET_ARM) && ((defined(__linux__) && !(defined(CORRADE_TARGET_ANDROID) && __ANDROID_API__ < 18)) || defined(__FreeBSD__))
namespace Implementation {
    inline Features runtimeFeatures(const unsigned long caps) {
        unsigned int out = 0;
        #ifdef CORRADE_TARGET_32BIT
        if(caps & (1 << 12) /*HWCAP_NEON*/) out |= TypeTraits<NeonT>::Index;
        if(caps & (1 << 16) /*HWCAP_VFPv4*/) out |= TypeTraits<NeonFmaT>::Index;
        #else
        out |=
            #ifdef CORRADE_TARGET_NEON
            TypeTraits<NeonT>::Index|
            #endif
            #ifdef CORRADE_TARGET_NEON_FMA
            TypeTraits<NeonFmaT>::Index|
            #endif
            0;
        if(caps & (1 << 10) /*HWCAP_ASIMDHP*/) out |= TypeTraits<NeonFp16T>::Index;
        #endif
        return Features{out};
    }
}
#endif

}

}

#endif
#ifdef CORRADE_CPU_IMPLEMENTATION
#if defined(CORRADE_TARGET_ARM) && ((defined(__linux__) && !(defined(CORRADE_TARGET_ANDROID) && __ANDROID_API__ < 18)) || defined(__FreeBSD__))
    #include <sys/auxv.h>
#elif defined(CORRADE_TARGET_ARM) && defined(CORRADE_TARGET_APPLE)
    #include <sys/sysctl.h>
#endif

namespace Corrade { namespace Cpu {

static_assert(sizeof(Cpu::Scalar) == 1, "");
#ifdef CORRADE_TARGET_X86
static_assert(sizeof(Cpu::Sse2) == 1, "");
static_assert(sizeof(Cpu::Sse3) == 1, "");
static_assert(sizeof(Cpu::Ssse3) == 1, "");
static_assert(sizeof(Cpu::Sse41) == 1, "");
static_assert(sizeof(Cpu::Sse42) == 1, "");
static_assert(sizeof(Cpu::Avx) == 1, "");
static_assert(sizeof(Cpu::AvxF16c) == 1, "");
static_assert(sizeof(Cpu::AvxFma) == 1, "");
static_assert(sizeof(Cpu::Avx2) == 1, "");
static_assert(sizeof(Cpu::Avx512f) == 1, "");
#elif defined(CORRADE_TARGET_ARM)
static_assert(sizeof(Cpu::Neon) == 1, "");
static_assert(sizeof(Cpu::NeonFma) == 1, "");
static_assert(sizeof(Cpu::NeonFp16) == 1, "");
#elif defined(CORRADE_TARGET_WASM)
static_assert(sizeof(Cpu::Simd128) == 1, "");
#endif

#if defined(CORRADE_TARGET_ARM) && defined(CORRADE_TARGET_APPLE)
namespace {

int appleSysctlByName(const char* name) {
  int value;
  std::size_t size = sizeof(value);
  return sysctlbyname(name, &value, &size, nullptr, 0) ? 0 : value;
}

}
#endif

#if defined(CORRADE_TARGET_ARM) && ((defined(__linux__) && !(defined(CORRADE_TARGET_ANDROID) && __ANDROID_API__ < 18)) || defined(CORRADE_TARGET_APPLE) || defined(__FreeBSD__))
Features runtimeFeatures() {
    #if defined(CORRADE_TARGET_ARM) && defined(__linux__) && !(defined(CORRADE_TARGET_ANDROID) && __ANDROID_API__ < 18)
    return Implementation::runtimeFeatures(getauxval(AT_HWCAP));

    #elif defined(CORRADE_TARGET_ARM) && defined(CORRADE_TARGET_APPLE)
    unsigned int out = 0;
    #ifdef CORRADE_TARGET_32BIT
    if(appleSysctlByName("hw.optional.neon")) out |= TypeTraits<NeonT>::Index;
    #else
    out |=
        #ifdef CORRADE_TARGET_NEON
        TypeTraits<NeonT>::Index|
        #endif
        #ifdef CORRADE_TARGET_NEON_FMA
        TypeTraits<NeonFmaT>::Index|
        #endif
        0;
    #endif
    if(appleSysctlByName("hw.optional.neon_fp16")) {
        #ifdef CORRADE_TARGET_32BIT
        out |= TypeTraits<NeonFmaT>::Index;
        #endif
        out |= TypeTraits<NeonFp16T>::Index;
    }

    return Features{out};

    #elif defined(CORRADE_TARGET_ARM) && defined(__FreeBSD__)
    unsigned long hwcap = 0;
    elf_aux_info(AT_HWCAP, &hwcap, sizeof(hwcap));
    return Implementation::runtimeFeatures(hwcap);

    #else
    #error
    #endif
}
#endif

}}
#endif
