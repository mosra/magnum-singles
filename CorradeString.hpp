/*
    Corrade::Containers::String
    Corrade::Containers::StringView
        — lightweight and optimized string (view) classes

    https://doc.magnum.graphics/corrade/classCorrade_1_1Containers_1_1BasicString.html
    https://doc.magnum.graphics/corrade/classCorrade_1_1Containers_1_1BasicStringView.html

    Depends on CorradeEnumSet.h, the implementation depends on CorradeCpu.hpp.

    This is a single-header library generated from the Corrade project. With
    the goal being easy integration, it's deliberately free of all comments
    to keep the file size small. More info, detailed changelogs and docs here:

    -   Project homepage — https://magnum.graphics/corrade/
    -   Documentation — https://doc.magnum.graphics/corrade/
    -   GitHub project page — https://github.com/mosra/corrade
    -   GitHub Singles repository — https://github.com/mosra/magnum-singles

    The library has a separate non-inline implementation part, enable it *just
    once* like this:

        #define CORRADE_STRING_IMPLEMENTATION
        #include <CorradeString.hpp>

    If you need the deinlined symbols to be exported from a shared library,
    `#define CORRADE_UTILITY_EXPORT` and `CORRADE_UTILITY_LOCAL` as
    appropriate. Runtime CPU dispatch for the implementation is enabled by
    default, you can disable it with `#define CORRADE_NO_CPU_RUNTIME_DISPATCH`
    before including the file in both the headers and the implementation. To
    enable the IFUNC functionality for CPU runtime dispatch,
    `#define CORRADE_CPU_USE_IFUNC`.

    The STL compatibility bits are included as well --- opt-in by specifying
    either `#define CORRADE_STRING_STL_COMPATIBILITY` or
    `#define CORRADE_STRING_STL_VIEW_COMPATIBILITY` before including the file.
    Including it multiple times with different macros defined works too.

    v2020.06-1864-g8b00 (2025-02-12)
    -   Removed dependency on Containers::Pair
    v2020.06-1846-gc4cdf (2025-01-07)
    -   Fixed embarrassing bugs in the NEON and WASM SIMD code paths for find()
    -   SFINAE is now done in template args as that's simpler for the compiler
    -   std::string STL compatibility is now inline, meaning the
        CORRADE_STRING_STL_COMPATIBILITY macro doesn't need to be defined also
        for CORRADE_STRING_IMPLEMENTATION anymore
    v2020.06-1687-g6b5f (2024-06-29)
    -   New, SIMD-optimized count() API
    -   Literals are now available in an inline Literals::StringLiterals
        namespace for finer control over which literals get actually used
    -   String copy construction and copy assignment now makes the copy a SSO
        only if the original instance was a SSO as well
    v2020.06-1502-g147e (2023-09-11)
    -   Initial release

    Generated from Corrade v2020.06-1864-g8b00 (2025-02-12), 2523 / 2179 LoC
*/

/*
    This file is part of Corrade.

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

#if !defined(CORRADE_CONSTEXPR_ASSERT) && !defined(NDEBUG)
#include <cassert>
#endif
#include <cstddef>
#include <type_traits>

#include "CorradeEnumSet.h"

#ifdef _MSC_VER
#define CORRADE_TARGET_MSVC
#endif
#if !defined(__x86_64) && !defined(_M_X64) && !defined(__aarch64__) && !defined(_M_ARM64) && !defined(__powerpc64__) && !defined(__wasm64__)
#define CORRADE_TARGET_32BIT
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

#ifndef CORRADE_NO_CPU_RUNTIME_DISPATCH
#define CORRADE_BUILD_CPU_RUNTIME_DISPATCH
#endif

#ifndef CorradeString_hpp
#define CorradeString_hpp
#ifndef CORRADE_UTILITY_EXPORT
#define CORRADE_UTILITY_EXPORT
#endif
#ifndef CORRADE_UTILITY_LOCAL
#define CORRADE_UTILITY_LOCAL
#endif
#if defined(CORRADE_BUILD_CPU_RUNTIME_DISPATCH) && !defined(CORRADE_CPU_USE_IFUNC)
    #define CORRADE_UTILITY_CPU_DISPATCHER_DECLARATION(name)                \
        CORRADE_UTILITY_EXPORT decltype(name) name ## Implementation(Cpu::Features);
    #define CORRADE_UTILITY_CPU_DISPATCHED_DECLARATION(name) (*name)
#else
    #define CORRADE_UTILITY_CPU_DISPATCHER_DECLARATION(name)
    #define CORRADE_UTILITY_CPU_DISPATCHED_DECLARATION(name) (name)
#endif

namespace Corrade { namespace Containers {

class String;
template<class> class BasicStringView;
typedef BasicStringView<const char> StringView;

}}
#endif
#ifndef Corrade_Corrade_h
#define Corrade_Corrade_h

namespace Corrade {

namespace Cpu {
    class Features;
}

}

#endif
#ifndef Corrade_Utility_Move_h
#define Corrade_Utility_Move_h

#ifdef CORRADE_MSVC2015_COMPATIBILITY
#include <utility>
#endif

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

#ifndef CORRADE_MSVC2015_COMPATIBILITY
template<class T> void swap(T& a, typename std::common_type<T>::type& b) noexcept(std::is_nothrow_move_constructible<T>::value && std::is_nothrow_move_assignable<T>::value) {
    T tmp = static_cast<T&&>(a);
    a = static_cast<T&&>(b);
    b = static_cast<T&&>(tmp);
}
#else
using std::swap;
#endif

#ifndef CORRADE_MSVC2015_COMPATIBILITY
template<std::size_t size, class T> void swap(T(&a)[size], typename std::common_type<T(&)[size]>::type b) noexcept(std::is_nothrow_move_constructible<T>::value && std::is_nothrow_move_assignable<T>::value) {
    for(std::size_t i = 0; i != size; ++i) {
        T tmp = static_cast<T&&>(a[i]);
        a[i] = static_cast<T&&>(b[i]);
        b[i] = static_cast<T&&>(tmp);
    }
}
#endif

}}

#endif
#ifndef CORRADE_CONSTEXPR_ASSERT
#ifdef NDEBUG
#define CORRADE_CONSTEXPR_ASSERT(condition, message) static_cast<void>(0)
#else
#define CORRADE_CONSTEXPR_ASSERT(condition, message)                        \
    static_cast<void>((condition) ? 0 : ([&]() {                            \
        assert(!#condition);                                                \
    }(), 0))
#endif
#endif
#ifndef CORRADE_CONSTEXPR_DEBUG_ASSERT
#define CORRADE_CONSTEXPR_DEBUG_ASSERT(condition, message)                  \
    CORRADE_CONSTEXPR_ASSERT(condition, message)
#endif
#ifndef Corrade_Containers_StringView_h
#define Corrade_Containers_StringView_h

namespace Corrade { namespace Containers {

namespace Implementation {
    template<class, class> struct StringViewConverter;
}

enum class StringViewFlag: std::size_t {
    Global = std::size_t{1} << (sizeof(std::size_t)*8 - 1),

    NullTerminated = std::size_t{1} << (sizeof(std::size_t)*8 - 2)
};

namespace Implementation {
    enum: std::size_t {
        StringViewSizeMask = std::size_t(StringViewFlag::NullTerminated)|std::size_t(StringViewFlag::Global)
    };
}

typedef EnumSet<StringViewFlag
    , Implementation::StringViewSizeMask
> StringViewFlags;

CORRADE_ENUMSET_OPERATORS(StringViewFlags)

template<class T> class
#ifndef CORRADE_TARGET_MSVC
CORRADE_UTILITY_EXPORT
#endif
BasicStringView {
    public:
        template<class U, class = typename std::enable_if<std::is_same<std::nullptr_t, U>::value>::type> constexpr /*implicit*/ BasicStringView(U) noexcept: _data{}, _sizePlusFlags{std::size_t(StringViewFlag::Global)} {}

        constexpr /*implicit*/ BasicStringView() noexcept: _data{}, _sizePlusFlags{std::size_t(StringViewFlag::Global)} {}

        constexpr /*implicit*/ BasicStringView(T* data, std::size_t size, StringViewFlags flags = {}) noexcept: _data{data}, _sizePlusFlags{(
            #ifdef CORRADE_TARGET_32BIT
            CORRADE_CONSTEXPR_DEBUG_ASSERT(size < std::size_t{1} << (sizeof(std::size_t)*8 - 2),
                "Containers::StringView: string expected to be smaller than 2^" << Utility::Debug::nospace << sizeof(std::size_t)*8 - 2 << "bytes, got" << size),
            #endif
            CORRADE_CONSTEXPR_DEBUG_ASSERT(data || !(flags & StringViewFlag::NullTerminated),
                "Containers::StringView: can't use StringViewFlag::NullTerminated with null data"),
            size|(std::size_t(flags) & Implementation::StringViewSizeMask))} {}

        /*implicit*/ BasicStringView(String& data) noexcept;

        template<class U = T, class = typename std::enable_if<std::is_const<U>::value>::type> /*implicit*/ BasicStringView(const String& data) noexcept;

        template<class U, class = typename std::enable_if<std::is_same<const U, T>::value>::type> constexpr /*implicit*/ BasicStringView(BasicStringView<U> mutable_) noexcept: _data{mutable_._data}, _sizePlusFlags{mutable_._sizePlusFlags} {}

        template<class U, class = typename std::enable_if<std::is_pointer<U>::value && std::is_convertible<const U&, T*>::value>::type> /*implicit*/ BasicStringView(U data, StringViewFlags extraFlags = {}) noexcept: BasicStringView{data, extraFlags, nullptr} {}

        template<class U, class = decltype(Implementation::StringViewConverter<T, typename std::decay<U&&>::type>::from(std::declval<U&&>()))> constexpr /*implicit*/ BasicStringView(U&& other) noexcept: BasicStringView{Implementation::StringViewConverter<T, typename std::decay<U&&>::type>::from(Utility::forward<U>(other))} {}

        template<class U, class = decltype(Implementation::StringViewConverter<T, U>::to(std::declval<BasicStringView<T>>()))> constexpr /*implicit*/ operator U() const {
            return Implementation::StringViewConverter<T, U>::to(*this);
        }

        constexpr explicit operator bool() const {
            return _data && (_sizePlusFlags & ~Implementation::StringViewSizeMask);
        }

        constexpr StringViewFlags flags() const {
            return StringViewFlag(_sizePlusFlags & Implementation::StringViewSizeMask);
        }

        constexpr T* data() const { return _data; }

        constexpr std::size_t size() const {
            return _sizePlusFlags & ~Implementation::StringViewSizeMask;
        }

        constexpr bool isEmpty() const {
            return !(_sizePlusFlags & ~Implementation::StringViewSizeMask);
        }

        constexpr T* begin() const { return _data; }
        constexpr T* cbegin() const { return _data; }

        constexpr T* end() const {
            return _data + (_sizePlusFlags & ~Implementation::StringViewSizeMask);
        }
        constexpr T* cend() const {
            return _data + (_sizePlusFlags & ~Implementation::StringViewSizeMask);
        }

        constexpr T& front() const;

        constexpr T& back() const;

        constexpr T& operator[](std::size_t i) const;

        constexpr BasicStringView<T> slice(T* begin, T* end) const;

        constexpr BasicStringView<T> slice(std::size_t begin, std::size_t end) const;

        template<class U, class = typename std::enable_if<std::is_convertible<U, T*>::value && !std::is_convertible<U, std::size_t>::value>::type> constexpr BasicStringView<T> sliceSize(U begin, std::size_t size) const {
            return slice(begin, begin + size);
        }

        constexpr BasicStringView<T> sliceSize(std::size_t begin, std::size_t size) const {
            return slice(begin, begin + size);
        }

        template<class U, class = typename std::enable_if<std::is_convertible<U, T*>::value && !std::is_convertible<U, std::size_t>::value>::type> constexpr BasicStringView<T> prefix(U end) const {
            return static_cast<T*>(end) ? slice(_data, end) : BasicStringView<T>{};
        }

        constexpr BasicStringView<T> suffix(T* begin) const {
            return _data && !begin ? BasicStringView<T>{} : slice(begin, _data + (_sizePlusFlags & ~Implementation::StringViewSizeMask));
        }

        constexpr BasicStringView<T> prefix(std::size_t size) const {
            return slice(0, size);
        }

        constexpr BasicStringView<T> exceptPrefix(std::size_t size) const {
            return slice(size, _sizePlusFlags & ~Implementation::StringViewSizeMask);
        }

        constexpr BasicStringView<T> exceptSuffix(std::size_t size) const {
            return slice(0, (_sizePlusFlags & ~Implementation::StringViewSizeMask) - size);
        }

        bool hasPrefix(StringView prefix) const;
        bool hasPrefix(char prefix) const;

        bool hasSuffix(StringView suffix) const;
        bool hasSuffix(char suffix) const;

        BasicStringView<T> exceptPrefix(StringView prefix) const;

        template<class = typename std::enable_if<std::is_same<typename std::decay<T>::type, char>::value>::type> BasicStringView<T> exceptPrefix(T&& prefix) const = delete;

        BasicStringView<T> exceptSuffix(StringView suffix) const;

        template<class = typename std::enable_if<std::is_same<typename std::decay<T>::type, char>::value>::type> BasicStringView<T> exceptSuffix(T&& suffix) const = delete;

        BasicStringView<T> trimmed(StringView characters) const {
            return trimmedPrefix(characters).trimmedSuffix(characters);
        }

        BasicStringView<T> trimmed() const;

        BasicStringView<T> trimmedPrefix(StringView characters) const;

        BasicStringView<T> trimmedPrefix() const;

        BasicStringView<T> trimmedSuffix(StringView characters) const;

        BasicStringView<T> trimmedSuffix() const;

        BasicStringView<T> find(StringView substring) const {
            return findOr(substring, nullptr);
        }

        BasicStringView<T> find(char character) const {
            return findOr(character, nullptr);
        }

        BasicStringView<T> findOr(StringView substring, T* fail) const;

        BasicStringView<T> findOr(char character, T* fail) const;

        BasicStringView<T> findLast(StringView substring) const {
            return findLastOr(substring, nullptr);
        }

        BasicStringView<T> findLast(char character) const {
            return findLastOr(character, nullptr);
        }

        BasicStringView<T> findLastOr(StringView substring, T* fail) const;

        BasicStringView<T> findLastOr(char character, T* fail) const;

        bool contains(StringView substring) const;

        bool contains(char character) const;

        BasicStringView<T> findAny(StringView characters) const {
            return findAnyOr(characters, nullptr);
        }

        BasicStringView<T> findAnyOr(StringView characters, T* fail) const;

        BasicStringView<T> findLastAny(StringView characters) const {
            return findLastAnyOr(characters, nullptr);
        }

        BasicStringView<T> findLastAnyOr(StringView characters, T* fail) const;

        bool containsAny(StringView substring) const;

        std::size_t count(char character) const;

    private:
        template<class> friend class BasicStringView;
        friend String;

        friend CORRADE_UTILITY_EXPORT bool operator==(StringView, StringView);
        friend CORRADE_UTILITY_EXPORT bool operator!=(StringView, StringView);
        friend CORRADE_UTILITY_EXPORT bool operator<(StringView, StringView);
        friend CORRADE_UTILITY_EXPORT bool operator<=(StringView, StringView);
        friend CORRADE_UTILITY_EXPORT bool operator>=(StringView, StringView);
        friend CORRADE_UTILITY_EXPORT bool operator>(StringView, StringView);
        friend CORRADE_UTILITY_EXPORT String operator+(StringView, StringView);
        friend CORRADE_UTILITY_EXPORT String operator*(StringView, std::size_t);

        explicit BasicStringView(T* data, StringViewFlags flags, std::nullptr_t) noexcept;

        constexpr explicit BasicStringView(T* data, std::size_t sizePlusFlags, std::nullptr_t) noexcept: _data{data}, _sizePlusFlags{sizePlusFlags} {}

        T* _data;
        std::size_t _sizePlusFlags;
};

typedef BasicStringView<const char> StringView;

typedef BasicStringView<char> MutableStringView;

CORRADE_UTILITY_EXPORT bool operator==(StringView a, StringView b);

CORRADE_UTILITY_EXPORT bool operator!=(StringView a, StringView b);

CORRADE_UTILITY_EXPORT bool operator<(StringView a, StringView b);

CORRADE_UTILITY_EXPORT bool operator<=(StringView a, StringView b);

CORRADE_UTILITY_EXPORT bool operator>=(StringView a, StringView b);

CORRADE_UTILITY_EXPORT bool operator>(StringView a, StringView b);

CORRADE_UTILITY_EXPORT String operator+(StringView a, StringView b);

CORRADE_UTILITY_EXPORT String operator*(StringView string, std::size_t count);

CORRADE_UTILITY_EXPORT String operator*(std::size_t count, StringView string);

namespace Literals {
    inline
    namespace StringLiterals {

#if defined(CORRADE_TARGET_CLANG) && __clang_major__ >= 17
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-literal-operator"
#endif
constexpr StringView operator"" _s(const char* data, std::size_t size) {
    return StringView{data, size, StringViewFlag(std::size_t(StringViewFlag::Global)|std::size_t(StringViewFlag::NullTerminated))};
}
#if defined(CORRADE_TARGET_CLANG) && __clang_major__ >= 17
#pragma clang diagnostic pop
#endif

}}

template<class T> constexpr T& BasicStringView<T>::operator[](const std::size_t i) const {
    return CORRADE_CONSTEXPR_DEBUG_ASSERT(i < size() + (flags() & StringViewFlag::NullTerminated ? 1 : 0),
        "Containers::StringView::operator[](): index" << i << "out of range for" << size() << (flags() & StringViewFlag::NullTerminated ? "null-terminated bytes" : "bytes")), _data[i];
}

template<class T> constexpr T& BasicStringView<T>::front() const {
    return CORRADE_CONSTEXPR_DEBUG_ASSERT(size(), "Containers::StringView::front(): view is empty"), _data[0];
}

template<class T> constexpr T& BasicStringView<T>::back() const {
    return CORRADE_CONSTEXPR_DEBUG_ASSERT(size(), "Containers::StringView::back(): view is empty"), _data[size() - 1];
}

template<class T> constexpr BasicStringView<T> BasicStringView<T>::slice(T* const begin, T* const end) const {
    return CORRADE_CONSTEXPR_DEBUG_ASSERT(_data <= begin && begin <= end && end <= _data + (_sizePlusFlags & ~Implementation::StringViewSizeMask),
            "Containers::StringView::slice(): slice ["
            << Utility::Debug::nospace << begin - _data
            << Utility::Debug::nospace << ":"
            << Utility::Debug::nospace << end - _data
            << Utility::Debug::nospace << "] out of range for"
            << (_sizePlusFlags & ~Implementation::StringViewSizeMask) << "elements"),
        BasicStringView<T>{begin, std::size_t(end - begin)|
            (_sizePlusFlags & std::size_t(StringViewFlag::Global))|
            ((_sizePlusFlags & std::size_t(StringViewFlag::NullTerminated))*(end == _data + (_sizePlusFlags & ~Implementation::StringViewSizeMask))),
            nullptr};
}

template<class T> constexpr BasicStringView<T> BasicStringView<T>::slice(const std::size_t begin, const std::size_t end) const {
    return CORRADE_CONSTEXPR_DEBUG_ASSERT(begin <= end && end <= (_sizePlusFlags & ~Implementation::StringViewSizeMask),
            "Containers::StringView::slice(): slice ["
            << Utility::Debug::nospace << begin
            << Utility::Debug::nospace << ":"
            << Utility::Debug::nospace << end
            << Utility::Debug::nospace << "] out of range for"
            << (_sizePlusFlags & ~Implementation::StringViewSizeMask) << "elements"),
        BasicStringView<T>{_data + begin, (end - begin)|
            (_sizePlusFlags & std::size_t(StringViewFlag::Global))|
            ((_sizePlusFlags & std::size_t(StringViewFlag::NullTerminated))*(end == (_sizePlusFlags & ~Implementation::StringViewSizeMask))),
            nullptr};
}

namespace Implementation {

CORRADE_UTILITY_EXPORT const char* stringFindString(const char* data, std::size_t size, const char* substring, std::size_t substringSize);
CORRADE_UTILITY_EXPORT const char* stringFindLastString(const char* data, std::size_t size, const char* substring, std::size_t substringSize);
CORRADE_UTILITY_EXPORT extern const char* CORRADE_UTILITY_CPU_DISPATCHED_DECLARATION(stringFindCharacter)(const char* data, std::size_t size, char character);
CORRADE_UTILITY_CPU_DISPATCHER_DECLARATION(stringFindCharacter)
CORRADE_UTILITY_EXPORT const char* stringFindLastCharacter(const char* data, std::size_t size, char character);
CORRADE_UTILITY_EXPORT const char* stringFindAny(const char* data, std::size_t size, const char* characters, std::size_t characterCount);
CORRADE_UTILITY_EXPORT const char* stringFindLastAny(const char* data, std::size_t size, const char* characters, std::size_t characterCount);
CORRADE_UTILITY_EXPORT const char* stringFindNotAny(const char* data, std::size_t size, const char* characters, std::size_t characterCount);
CORRADE_UTILITY_EXPORT const char* stringFindLastNotAny(const char* data, std::size_t size, const char* characters, std::size_t characterCount);
CORRADE_UTILITY_EXPORT extern std::size_t CORRADE_UTILITY_CPU_DISPATCHED_DECLARATION(stringCountCharacter)(const char* data, std::size_t size, char character);
CORRADE_UTILITY_CPU_DISPATCHER_DECLARATION(stringCountCharacter)

}

template<class T> inline BasicStringView<T> BasicStringView<T>::trimmedPrefix(const StringView characters) const {
    const std::size_t size = this->size();
    T* const found = const_cast<T*>(Implementation::stringFindNotAny(_data, size, characters._data, characters.size()));
    return suffix(found ? found : _data + size);
}

template<class T> inline BasicStringView<T> BasicStringView<T>::trimmedSuffix(const StringView characters) const {
    T* const found = const_cast<T*>(Implementation::stringFindLastNotAny(_data, size(), characters._data, characters.size()));
    return prefix(found ? found + 1 : _data);
}

template<class T> inline BasicStringView<T> BasicStringView<T>::findOr(const StringView substring, T* const fail) const {
    const std::size_t substringSize = substring.size();
    if(const char* const found = Implementation::stringFindString(_data, size(), substring._data, substringSize))
        return slice(const_cast<T*>(found), const_cast<T*>(found + substringSize));

    return BasicStringView<T>{fail, 0 /* empty, no flags */, nullptr};
}

template<class T> inline BasicStringView<T> BasicStringView<T>::findOr(const char character, T* const fail) const {
    if(const char* const found = Implementation::stringFindCharacter(_data, size(), character))
        return slice(const_cast<T*>(found), const_cast<T*>(found + 1));

    return BasicStringView<T>{fail, 0 /* empty, no flags */, nullptr};
}

template<class T> inline BasicStringView<T> BasicStringView<T>::findLastOr(const StringView substring, T* const fail) const {
    const std::size_t substringSize = substring.size();
    if(const char* const found = Implementation::stringFindLastString(_data, size(), substring._data, substringSize))
        return slice(const_cast<T*>(found), const_cast<T*>(found + substringSize));

    return BasicStringView<T>{fail, 0 /* empty, no flags */, nullptr};
}

template<class T> inline BasicStringView<T> BasicStringView<T>::findLastOr(const char character, T* const fail) const {
    if(const char* const found = Implementation::stringFindLastCharacter(_data, size(), character))
        return slice(const_cast<T*>(found), const_cast<T*>(found + 1));

    return BasicStringView<T>{fail, 0 /* empty, no flags */, nullptr};
}

template<class T> inline bool BasicStringView<T>::contains(const StringView substring) const {
    return Implementation::stringFindString(_data, size(), substring._data, substring.size());
}

template<class T> inline bool BasicStringView<T>::contains(const char character) const {
    return Implementation::stringFindCharacter(_data, size(), character);
}

template<class T> inline BasicStringView<T> BasicStringView<T>::findAnyOr(const StringView characters, T* const fail) const {
    if(const char* const found = Implementation::stringFindAny(_data, size(), characters._data, characters.size()))
        return slice(const_cast<T*>(found), const_cast<T*>(found + 1));

    return BasicStringView<T>{fail, 0 /* empty, no flags */, nullptr};
}

template<class T> inline BasicStringView<T> BasicStringView<T>::findLastAnyOr(const StringView characters, T* const fail) const {
    if(const char* const found = Implementation::stringFindLastAny(_data, size(), characters._data, characters.size()))
        return slice(const_cast<T*>(found), const_cast<T*>(found + 1));

    return BasicStringView<T>{fail, 0 /* empty, no flags */, nullptr};
}

template<class T> inline bool BasicStringView<T>::containsAny(const StringView characters) const {
    return Implementation::stringFindAny(_data, size(), characters._data, characters.size());
}

template<class T> inline std::size_t BasicStringView<T>::count(const char character) const {
    return Implementation::stringCountCharacter(_data, size(), character);
}

}}

#endif
#ifndef Corrade_Containers_String_h
#define Corrade_Containers_String_h

namespace Corrade { namespace Containers {

namespace Implementation {
    template<class> struct StringConverter;

    enum: std::size_t {
        SmallStringBit = 0x40,
        SmallStringSize = sizeof(std::size_t)*3 - 1
    };
}

struct AllocatedInitT {
    struct Init {};
    constexpr explicit AllocatedInitT(Init) {}
};

constexpr AllocatedInitT AllocatedInit{AllocatedInitT::Init{}};

class CORRADE_UTILITY_EXPORT String {
    public:
        typedef void(*Deleter)(char*, std::size_t);

        static String nullTerminatedView(StringView view);

        static String nullTerminatedView(AllocatedInitT, StringView view);

        static String nullTerminatedGlobalView(StringView view);

        static String nullTerminatedGlobalView(AllocatedInitT, StringView view);

        /*implicit*/ String() noexcept;

        /*implicit*/ String(StringView view);
        /*implicit*/ String(MutableStringView view);

        template<class T, class = typename std::enable_if<std::is_convertible<T, const char*>::value && !std::is_convertible<T, std::size_t>::value>::type> /*implicit*/ String(T data): String{nullptr, nullptr, nullptr, data} {}

        /*implicit*/ String(const char* data, std::size_t size);

        explicit String(AllocatedInitT, StringView view);
        explicit String(AllocatedInitT, MutableStringView view);

        explicit String(AllocatedInitT, String&& other);
        explicit String(AllocatedInitT, const String& other);

        explicit String(AllocatedInitT, const char* data);

        explicit String(AllocatedInitT, const char* data, std::size_t size);

        explicit String(char* data, std::size_t size, Deleter deleter) noexcept;

        template<class T, typename std::enable_if<std::is_convertible<T, Deleter>::value && !std::is_convertible<T, std::size_t>::value, int>::type = 0> String(char* data, T deleter) noexcept: String{deleter, nullptr, data} {}

        explicit String(const char* data, std::size_t size, Deleter deleter) noexcept: String{const_cast<char*>(data), size, deleter} {}

        template<class T, class = typename std::enable_if<std::is_convertible<T, Deleter>::value && !std::is_convertible<T, std::size_t>::value, const char*>::type> String(const char* data, T deleter) noexcept: String{deleter, nullptr, const_cast<char*>(data)} {}

        explicit String(std::nullptr_t, std::size_t size, Deleter deleter) = delete;

        template<class T, typename std::enable_if<std::is_convertible<T, Deleter>::value && !std::is_convertible<T, std::size_t>::value, int>::type = 0> String(std::nullptr_t, T) noexcept = delete;

        explicit String(Corrade::ValueInitT, std::size_t size);

        explicit String(Corrade::NoInitT, std::size_t size);

        explicit String(Corrade::DirectInitT, std::size_t size, char c);

        template<class T, class = decltype(Implementation::StringConverter<typename std::decay<T&&>::type>::from(std::declval<T&&>()))> /*implicit*/ String(T&& other) noexcept: String{Implementation::StringConverter<typename std::decay<T&&>::type>::from(Utility::forward<T>(other))} {}

        ~String();

        String(const String& other);

        String(String&& other) noexcept;

        String& operator=(const String& other);

        String& operator=(String&& other) noexcept;

        template<class T, class = decltype(Implementation::StringConverter<T>::to(std::declval<String>()))> /*implicit*/ operator T() const {
            return Implementation::StringConverter<T>::to(*this);
        }

        explicit operator bool() const;

        bool isSmall() const {
            return _small.size & Implementation::SmallStringBit;
        }

        StringViewFlags viewFlags() const;

        char* data();
        const char* data() const;

        Deleter deleter() const;

        bool isEmpty() const;

        std::size_t size() const;

        char* begin();
        const char* begin() const;
        const char* cbegin() const;

        char* end();
        const char* end() const;
        const char* cend() const;

        char& front();
        char front() const;

        char& back();
        char back() const;

        char& operator[](std::size_t i);
        char operator[](std::size_t i) const;

        MutableStringView slice(char* begin, char* end);
        StringView slice(const char* begin, const char* end) const;
        MutableStringView slice(std::size_t begin, std::size_t end);
        StringView slice(std::size_t begin, std::size_t end) const;

        template<class T, class = typename std::enable_if<std::is_convertible<T, char*>::value && !std::is_convertible<T, std::size_t>::value>::type> MutableStringView sliceSize(T begin, std::size_t size) {
            return sliceSizePointerInternal(begin, size);
        }
        template<class T, class = typename std::enable_if<std::is_convertible<T, const char*>::value && !std::is_convertible<T, std::size_t>::value>::type> StringView sliceSize(T begin, std::size_t size) const {
            return sliceSizePointerInternal(begin, size);
        }
        MutableStringView sliceSize(std::size_t begin, std::size_t size);
        StringView sliceSize(std::size_t begin, std::size_t size) const;

        template<class T, class = typename std::enable_if<std::is_convertible<T, char*>::value && !std::is_convertible<T, std::size_t>::value>::type> MutableStringView prefix(T end) {
            return prefixPointerInternal(end);
        }
        template<class T, class = typename std::enable_if<std::is_convertible<T, const char*>::value && !std::is_convertible<T, std::size_t>::value>::type> StringView prefix(T end) const {
            return prefixPointerInternal(end);
        }

        MutableStringView suffix(char* begin);
        StringView suffix(const char* begin) const;

        MutableStringView prefix(std::size_t size);
        StringView prefix(std::size_t size) const;

        MutableStringView exceptPrefix(std::size_t size);
        StringView exceptPrefix(std::size_t size) const;

        MutableStringView exceptSuffix(std::size_t size);
        StringView exceptSuffix(std::size_t size) const;

        bool hasPrefix(StringView prefix) const;
        bool hasPrefix(char prefix) const;

        bool hasSuffix(StringView suffix) const;
        bool hasSuffix(char suffix) const;

        MutableStringView exceptPrefix(StringView prefix);
        StringView exceptPrefix(StringView prefix) const;

        template<class T, class = typename std::enable_if<std::is_same<typename std::decay<T>::type, char>::value>::type> MutableStringView exceptPrefix(T&& prefix) = delete;
        template<class T, class = typename std::enable_if<std::is_same<typename std::decay<T>::type, char>::value>::type> StringView exceptPrefix(T&& prefix) const = delete;

        MutableStringView exceptSuffix(StringView suffix);
        StringView exceptSuffix(StringView suffix) const;

        template<class T, class = typename std::enable_if<std::is_same<typename std::decay<T>::type, char>::value>::type> MutableStringView exceptSuffix(T&& suffix) = delete;
        template<class T, class = typename std::enable_if<std::is_same<typename std::decay<T>::type, char>::value>::type> StringView exceptSuffix(T&& suffix) const = delete;

        MutableStringView trimmed(StringView characters);
        StringView trimmed(StringView characters) const;

        MutableStringView trimmed();
        StringView trimmed() const;

        MutableStringView trimmedPrefix(StringView characters);
        StringView trimmedPrefix(StringView characters) const;

        MutableStringView trimmedPrefix();
        StringView trimmedPrefix() const;

        MutableStringView trimmedSuffix(StringView characters);
        StringView trimmedSuffix(StringView characters) const;

        MutableStringView trimmedSuffix();
        StringView trimmedSuffix() const;

        MutableStringView find(StringView substring);
        StringView find(StringView substring) const;

        MutableStringView find(char character);
        StringView find(char character) const;

        MutableStringView findOr(StringView substring, char* fail);
        StringView findOr(StringView substring, const char* fail) const;

        MutableStringView findOr(char character, char* fail);
        StringView findOr(char character, const char* fail) const;

        MutableStringView findLast(StringView substring);
        StringView findLast(StringView substring) const;

        MutableStringView findLast(char character);
        StringView findLast(char character) const;

        MutableStringView findLastOr(StringView substring, char* fail);
        StringView findLastOr(StringView substring, const char* fail) const;

        MutableStringView findLastOr(char character, char* fail);
        StringView findLastOr(char character, const char* fail) const;

        bool contains(StringView substring) const;

        bool contains(char character) const;

        MutableStringView findAny(StringView characters);
        StringView findAny(StringView characters) const;

        MutableStringView findAnyOr(StringView characters, char* fail);
        StringView findAnyOr(StringView characters, const char* fail) const;

        MutableStringView findLastAny(StringView characters);
        StringView findLastAny(StringView characters) const;

        MutableStringView findLastAnyOr(StringView characters, char* fail);
        StringView findLastAnyOr(StringView characters, const char* fail) const;

        bool containsAny(StringView substring) const;

        std::size_t count(char character) const;

        char* release();

    private:
        explicit String(std::nullptr_t, std::nullptr_t, std::nullptr_t, const char* data);
        explicit String(Deleter deleter, std::nullptr_t, char* data) noexcept;

        CORRADE_UTILITY_LOCAL void construct(Corrade::NoInitT, std::size_t size);
        CORRADE_UTILITY_LOCAL void construct(const char* data, std::size_t size);
        CORRADE_UTILITY_LOCAL void copyConstruct(const String& other);
        CORRADE_UTILITY_LOCAL void destruct();
        struct Data {
            const char* data;
            std::size_t size;
        };
        CORRADE_UTILITY_LOCAL Data dataInternal() const;

        MutableStringView sliceSizePointerInternal(char* begin, std::size_t size);
        StringView sliceSizePointerInternal(const char* begin, std::size_t size) const;
        MutableStringView prefixPointerInternal(char* end);
        StringView prefixPointerInternal(const char* end) const;

        struct Small {
            #ifdef CORRADE_TARGET_BIG_ENDIAN
            unsigned char size;
            char data[Implementation::SmallStringSize];
            #else
            char data[Implementation::SmallStringSize];
            unsigned char size;
            #endif
        };
        struct Large {
            #ifdef CORRADE_TARGET_BIG_ENDIAN
            std::size_t size;
            char* data;
            void(*deleter)(char*, std::size_t);
            #else
            char* data;
            void(*deleter)(char*, std::size_t);
            std::size_t size;
            #endif
        };
        union {
            Small _small;
            Large _large;
        };
};

}}

#endif
#ifdef CORRADE_STRING_STL_COMPATIBILITY
#include <string>
#ifndef Corrade_Containers_StringStl_h
#define Corrade_Containers_StringStl_h

namespace Corrade { namespace Containers { namespace Implementation {

template<> struct
StringConverter<std::string> {
    static String from(const std::string& other);
    static std::string to(const String& other);
};

template<> struct
StringViewConverter<const char, std::string> {
    static StringView from(const std::string& other);
    static std::string to(StringView other);
};

template<> struct
StringViewConverter<char, std::string> {
    static MutableStringView from(std::string& other);
    static std::string to(MutableStringView other);
};

#define CORRADE_STRING_STL_INLINE inline
CORRADE_STRING_STL_INLINE StringView StringViewConverter<const char, std::string>::from(const std::string& other) {
    return StringView{other.data(), other.size(), StringViewFlag::NullTerminated};
}

CORRADE_STRING_STL_INLINE std::string StringViewConverter<const char, std::string>::to(StringView other) {
    return std::string{other.data(), other.size()};
}

CORRADE_STRING_STL_INLINE MutableStringView StringViewConverter<char, std::string>::from(std::string& other) {
    return MutableStringView{&other[0], other.size(), StringViewFlag::NullTerminated};
}

CORRADE_STRING_STL_INLINE std::string StringViewConverter<char, std::string>::to(MutableStringView other) {
    return std::string{other.data(), other.size()};
}

CORRADE_STRING_STL_INLINE String StringConverter<std::string>::from(const std::string& other) {
    return String{other.data(), other.size()};
}

CORRADE_STRING_STL_INLINE std::string StringConverter<std::string>::to(const String& other) {
    return std::string{other.data(), other.size()};
}

}}}

#endif
#endif
#ifdef CORRADE_STRING_STL_VIEW_COMPATIBILITY
#include <string_view>
#ifndef Corrade_Containers_StringStlView_h
#define Corrade_Containers_StringStlView_h

namespace Corrade { namespace Containers { namespace Implementation {

template<> struct StringConverter<std::string_view> {
    static String from(std::string_view other) {
        return String{other.data(), other.size()};
    }
    static std::string_view to(const String& other) {
        return std::string_view{other.data(), other.size()};
    }
};

template<> struct StringViewConverter<const char, std::string_view> {
    static StringView from(std::string_view other) {
        return StringView{other.data(), other.size()};
    }
    static std::string_view to(StringView other) {
        return std::string_view{other.data(), other.size()};
    }
};

template<> struct StringViewConverter<char, std::string_view> {
    static std::string_view to(MutableStringView other) {
        return std::string_view{other.data(), other.size()};
    }
};

}}}

#endif
#endif
#ifdef CORRADE_STRING_IMPLEMENTATION
#include <cstdint>
#include <cstdlib>
#include <cstring>
#if (!defined(CORRADE_ASSERT) || !defined(CORRADE_INTERNAL_ASSERT) || !defined(CORRADE_INTERNAL_ASSERT_UNREACHABLE)) && !defined(NDEBUG)
#include <cassert>
#endif

#ifndef CORRADE_ASSERT
#ifdef NDEBUG
#define CORRADE_ASSERT(condition, message, returnValue) do {} while(false)
#else
#define CORRADE_ASSERT(condition, message, returnValue) assert(condition)
#endif
#endif

#ifndef CORRADE_INTERNAL_ASSERT
#ifdef NDEBUG
#define CORRADE_INTERNAL_ASSERT(condition) do {} while(false)
#else
#define CORRADE_INTERNAL_ASSERT(condition) assert(condition)
#endif
#endif

#ifndef CORRADE_INTERNAL_ASSERT_UNREACHABLE
#ifdef NDEBUG
#ifdef CORRADE_TARGET_GCC
#define CORRADE_INTERNAL_ASSERT_UNREACHABLE() __builtin_unreachable()
#elif defined(CORRADE_TARGET_MSVC)
#define CORRADE_INTERNAL_ASSERT_UNREACHABLE() __assume(0)
#else
#define CORRADE_INTERNAL_ASSERT_UNREACHABLE() std::abort()
#endif
#else
#define CORRADE_INTERNAL_ASSERT_UNREACHABLE() assert(!"unreachable code")
#endif
#endif
#ifndef CORRADE_DEBUG_ASSERT
#define CORRADE_DEBUG_ASSERT(condition, message, returnValue)               \
    CORRADE_ASSERT(condition, message, returnValue)
#endif

#ifndef CORRADE_INTERNAL_DEBUG_ASSERT
#define CORRADE_INTERNAL_DEBUG_ASSERT(condition)                            \
    CORRADE_INTERNAL_ASSERT(condition)
#endif

#ifndef CORRADE_INTERNAL_DEBUG_ASSERT_UNREACHABLE
#define CORRADE_INTERNAL_DEBUG_ASSERT_UNREACHABLE()                         \
    CORRADE_INTERNAL_ASSERT_UNREACHABLE()
#endif

#include "CorradeCpu.hpp"

#if ((defined(CORRADE_ENABLE_SSE2) || defined(CORRADE_ENABLE_AVX)) && defined(CORRADE_ENABLE_BMI1)) || (defined(CORRADE_ENABLE_AVX) && defined(CORRADE_ENABLE_POPCNT))
#include <immintrin.h>
#elif defined(CORRADE_ENABLE_SSE2) && defined(CORRADE_ENABLE_POPCNT)
#include <smmintrin.h>
#include <nmmintrin.h>
#endif
#ifdef CORRADE_ENABLE_NEON
#include <arm_neon.h>
#endif
#ifdef CORRADE_ENABLE_SIMD128
#include <wasm_simd128.h>
#endif
#ifndef Corrade_Utility_Math_h
#define Corrade_Utility_Math_h

namespace Corrade { namespace Utility {

template<class T> constexpr T min(T value, T min) {
    return min < value ? min : value;
}

template<class T> constexpr T max(T value, T max) {
    return value < max ? max : value;
}

template<class T> constexpr T abs(T a) {
    return a < T{0} ? -a : a;
}

}}

#endif
#if defined(CORRADE_TARGET_GCC) && !defined(CORRADE_TARGET_CLANG) && __GNUC__ >= 10
#define CORRADE_UNUSED [[maybe_unused]]
#elif defined(CORRADE_TARGET_GCC) || defined(CORRADE_TARGET_CLANG_CL)
#define CORRADE_UNUSED __attribute__((__unused__))
#elif defined(CORRADE_TARGET_MSVC)
#define CORRADE_UNUSED __pragma(warning(suppress:4100))
#else
#define CORRADE_UNUSED
#endif

#if (defined(CORRADE_TARGET_MSVC) && _MSC_VER >= 1926 && CORRADE_CXX_STANDARD >= 201703) || (defined(CORRADE_TARGET_GCC) && !defined(CORRADE_TARGET_CLANG) && __GNUC__ >= 7)
#define CORRADE_FALLTHROUGH [[fallthrough]];
#elif defined(CORRADE_TARGET_CLANG)
#define CORRADE_FALLTHROUGH [[clang::fallthrough]];
#else
#define CORRADE_FALLTHROUGH
#endif

#define CORRADE_PASSTHROUGH(...) __VA_ARGS__

#define CORRADE_NOOP(...)
#ifndef Corrade_Utility_Implementation_cpu_h
#define Corrade_Utility_Implementation_cpu_h

#if (defined(CORRADE_BUILD_CPU_RUNTIME_DISPATCH) && !defined(CORRADE_CPU_USE_IFUNC))
    #define CORRADE_UTILITY_CPU_DISPATCHER(...) CORRADE_CPU_DISPATCHER(__VA_ARGS__)
    #define CORRADE_UTILITY_CPU_DISPATCHER_BASE(...) CORRADE_CPU_DISPATCHER_BASE(__VA_ARGS__)
    #define CORRADE_UTILITY_CPU_DISPATCHED(dispatcher, ...)                 \
        CORRADE_CPU_DISPATCHED_POINTER(dispatcher, __VA_ARGS__) CORRADE_NOOP
    #define CORRADE_UTILITY_CPU_MAYBE_UNUSED
#else
    #if defined(CORRADE_BUILD_CPU_RUNTIME_DISPATCH) && defined(CORRADE_CPU_USE_IFUNC)
        #define CORRADE_UTILITY_CPU_DISPATCHER(...)                         \
            namespace { CORRADE_CPU_DISPATCHER(__VA_ARGS__) }
        #define CORRADE_UTILITY_CPU_DISPATCHER_BASE(...)                    \
            namespace { CORRADE_CPU_DISPATCHER_BASE(__VA_ARGS__) }
        #define CORRADE_UTILITY_CPU_DISPATCHED(dispatcher, ...)             \
            CORRADE_CPU_DISPATCHED_IFUNC(dispatcher, __VA_ARGS__) CORRADE_NOOP
        #define CORRADE_UTILITY_CPU_MAYBE_UNUSED
    #elif !defined(CORRADE_BUILD_CPU_RUNTIME_DISPATCH)
        #define CORRADE_UTILITY_CPU_DISPATCHER(...)
        #define CORRADE_UTILITY_CPU_DISPATCHER_BASE(...)
        #define CORRADE_UTILITY_CPU_DISPATCHED(dispatcher, ...)             \
            __VA_ARGS__ CORRADE_PASSTHROUGH
        #define CORRADE_UTILITY_CPU_MAYBE_UNUSED CORRADE_UNUSED
    #else
    #error mosra messed up!
    #endif
#endif

#endif
namespace Corrade { namespace Containers {

template<class T> BasicStringView<T>::BasicStringView(T* const data, const StringViewFlags flags, std::nullptr_t) noexcept: BasicStringView{data,
    data ? std::strlen(data) : 0,
    flags|(data ? StringViewFlag::NullTerminated : StringViewFlag::Global)} {}

template<class T> BasicStringView<T>::BasicStringView(String& string) noexcept: BasicStringView{string.data(), string.size(), string.viewFlags()} {}

template<> template<> CORRADE_UTILITY_EXPORT BasicStringView<const char>::BasicStringView(const String& string) noexcept: BasicStringView{string.data(), string.size(), string.viewFlags()} {}

namespace Implementation {

const char* stringFindString(const char* data, const std::size_t size, const char* const substring, const std::size_t substringSize) {
    if(substringSize <= size) {
        if(!size) return data;

        for(const char* const max = data + size - substringSize; data <= max; ++data) {
            if(std::memcmp(data, substring, substringSize) == 0)
                return data;
        }
    }

    return {};
}

}

namespace Implementation {

const char* stringFindLastString(const char* const data, const std::size_t size, const char* const substring, const std::size_t substringSize) {
    if(substringSize <= size) {
        if(!size) return data;

        for(const char* i = data + size - substringSize; i >= data; --i) {
            if(std::memcmp(i, substring, substringSize) == 0)
                return i;
        }
    }

    return {};
}

namespace {

#if defined(CORRADE_ENABLE_SSE2) && defined(CORRADE_ENABLE_BMI1)
CORRADE_UTILITY_CPU_MAYBE_UNUSED CORRADE_ENABLE(SSE2,BMI1) typename std::decay<decltype(stringFindCharacter)>::type stringFindCharacterImplementation(CORRADE_CPU_DECLARE(Cpu::Sse2|Cpu::Bmi1)) {
  return [](const char* const data, const std::size_t size, const char character) CORRADE_ENABLE(SSE2,BMI1) {
    {
        const char* j = data;
        switch(size) {
            case 15: if(*j++ == character) return j - 1; CORRADE_FALLTHROUGH
            case 14: if(*j++ == character) return j - 1; CORRADE_FALLTHROUGH
            case 13: if(*j++ == character) return j - 1; CORRADE_FALLTHROUGH
            case 12: if(*j++ == character) return j - 1; CORRADE_FALLTHROUGH
            case 11: if(*j++ == character) return j - 1; CORRADE_FALLTHROUGH
            case 10: if(*j++ == character) return j - 1; CORRADE_FALLTHROUGH
            case  9: if(*j++ == character) return j - 1; CORRADE_FALLTHROUGH
            case  8: if(*j++ == character) return j - 1; CORRADE_FALLTHROUGH
            case  7: if(*j++ == character) return j - 1; CORRADE_FALLTHROUGH
            case  6: if(*j++ == character) return j - 1; CORRADE_FALLTHROUGH
            case  5: if(*j++ == character) return j - 1; CORRADE_FALLTHROUGH
            case  4: if(*j++ == character) return j - 1; CORRADE_FALLTHROUGH
            case  3: if(*j++ == character) return j - 1; CORRADE_FALLTHROUGH
            case  2: if(*j++ == character) return j - 1; CORRADE_FALLTHROUGH
            case  1: if(*j++ == character) return j - 1; CORRADE_FALLTHROUGH
            case  0: return static_cast<const char*>(nullptr);
        }
    }

    const __m128i vn1 = _mm_set1_epi8(character);

    {
        const __m128i chunk = _mm_loadu_si128(reinterpret_cast<const __m128i*>(data));
        if(const int mask = _mm_movemask_epi8(_mm_cmpeq_epi8(chunk, vn1)))
            return data + _tzcnt_u32(mask);
    }

    const char* i = reinterpret_cast<const char*>(reinterpret_cast<std::uintptr_t>(data + 16) & ~0xf);
    CORRADE_INTERNAL_DEBUG_ASSERT(i > data && reinterpret_cast<std::uintptr_t>(i) % 16 == 0);

    const char* const end = data + size;
    for(; i + 4*16 <= end; i += 4*16) {
        const __m128i a = _mm_load_si128(reinterpret_cast<const __m128i*>(i) + 0);
        const __m128i b = _mm_load_si128(reinterpret_cast<const __m128i*>(i) + 1);
        const __m128i c = _mm_load_si128(reinterpret_cast<const __m128i*>(i) + 2);
        const __m128i d = _mm_load_si128(reinterpret_cast<const __m128i*>(i) + 3);

        const __m128i eqA = _mm_cmpeq_epi8(vn1, a);
        const __m128i eqB = _mm_cmpeq_epi8(vn1, b);
        const __m128i eqC = _mm_cmpeq_epi8(vn1, c);
        const __m128i eqD = _mm_cmpeq_epi8(vn1, d);

        const __m128i or1 = _mm_or_si128(eqA, eqB);
        const __m128i or2 = _mm_or_si128(eqC, eqD);
        const __m128i or3 = _mm_or_si128(or1, or2);
        if(_mm_movemask_epi8(or3)) {
            if(const int mask = _mm_movemask_epi8(eqA))
                return i + 0*16 + _tzcnt_u32(mask);
            if(const int mask = _mm_movemask_epi8(eqB))
                return i + 1*16 + _tzcnt_u32(mask);
            if(const int mask = _mm_movemask_epi8(eqC))
                return i + 2*16 + _tzcnt_u32(mask);
            if(const int mask = _mm_movemask_epi8(eqD))
                return i + 3*16 + _tzcnt_u32(mask);
            CORRADE_INTERNAL_DEBUG_ASSERT_UNREACHABLE();
        }
    }

    for(; i + 16 <= end; i += 16) {
        const __m128i chunk = _mm_load_si128(reinterpret_cast<const __m128i*>(i));
        if(const int mask = _mm_movemask_epi8(_mm_cmpeq_epi8(chunk, vn1)))
            return i + _tzcnt_u32(mask);
    }

    if(i < end) {
        CORRADE_INTERNAL_DEBUG_ASSERT(i + 16 > end);
        i = end - 16;
        const __m128i chunk = _mm_loadu_si128(reinterpret_cast<const __m128i*>(i));
        if(const int mask = _mm_movemask_epi8(_mm_cmpeq_epi8(chunk, vn1)))
            return i + _tzcnt_u32(mask);
    }

    return static_cast<const char*>(nullptr);
  };
}
#endif

#if defined(CORRADE_ENABLE_AVX2) && defined(CORRADE_ENABLE_BMI1)
CORRADE_UTILITY_CPU_MAYBE_UNUSED CORRADE_ENABLE(AVX2,BMI1) typename std::decay<decltype(stringFindCharacter)>::type stringFindCharacterImplementation(CORRADE_CPU_DECLARE(Cpu::Avx2|Cpu::Bmi1)) {
  return [](const char* const data, const std::size_t size, const char character) CORRADE_ENABLE(AVX2,BMI1) {
    if(size < 32)
        return stringFindCharacterImplementation(CORRADE_CPU_SELECT(Cpu::Sse2|Cpu::Bmi1))(data, size, character);

    const __m256i vn1 = _mm256_set1_epi8(character);

    {
        const __m256i chunk = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(data));
        if(const int mask = _mm256_movemask_epi8(_mm256_cmpeq_epi8(chunk, vn1)))
            return data + _tzcnt_u32(mask);
    }

    const char* i = reinterpret_cast<const char*>(reinterpret_cast<std::uintptr_t>(data + 32) & ~0x1f);
    CORRADE_INTERNAL_DEBUG_ASSERT(i > data && reinterpret_cast<std::uintptr_t>(i) % 32 == 0);

    const char* const end = data + size;
    for(; i + 4*32 <= end; i += 4*32) {
        const __m256i a = _mm256_load_si256(reinterpret_cast<const __m256i*>(i) + 0);
        const __m256i b = _mm256_load_si256(reinterpret_cast<const __m256i*>(i) + 1);
        const __m256i c = _mm256_load_si256(reinterpret_cast<const __m256i*>(i) + 2);
        const __m256i d = _mm256_load_si256(reinterpret_cast<const __m256i*>(i) + 3);

        const __m256i eqA = _mm256_cmpeq_epi8(vn1, a);
        const __m256i eqB = _mm256_cmpeq_epi8(vn1, b);
        const __m256i eqC = _mm256_cmpeq_epi8(vn1, c);
        const __m256i eqD = _mm256_cmpeq_epi8(vn1, d);

        const __m256i or1 = _mm256_or_si256(eqA, eqB);
        const __m256i or2 = _mm256_or_si256(eqC, eqD);
        const __m256i or3 = _mm256_or_si256(or1, or2);
        if(_mm256_movemask_epi8(or3)) {
            if(const int mask = _mm256_movemask_epi8(eqA))
                return i + 0*32 + _tzcnt_u32(mask);
            if(const int mask = _mm256_movemask_epi8(eqB))
                return i + 1*32 + _tzcnt_u32(mask);
            if(const int mask = _mm256_movemask_epi8(eqC))
                return i + 2*32 + _tzcnt_u32(mask);
            if(const int mask = _mm256_movemask_epi8(eqD))
                return i + 3*32 + _tzcnt_u32(mask);
            CORRADE_INTERNAL_DEBUG_ASSERT_UNREACHABLE();
        }
    }

    for(; i + 32 <= end; i += 32) {
        const __m256i chunk = _mm256_load_si256(reinterpret_cast<const __m256i*>(i));
        if(const int mask = _mm256_movemask_epi8(_mm256_cmpeq_epi8(chunk, vn1)))
            return i + _tzcnt_u32(mask);
    }

    if(i < end) {
        CORRADE_INTERNAL_DEBUG_ASSERT(i + 32 > end);
        i = end - 32;
        const __m256i chunk = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(i));
        if(const int mask = _mm256_movemask_epi8(_mm256_cmpeq_epi8(chunk, vn1)))
            return i + _tzcnt_u32(mask);
    }

    return static_cast<const char*>(nullptr);
  };
}
#endif

#if defined(CORRADE_ENABLE_NEON) && !defined(CORRADE_TARGET_32BIT)
CORRADE_UTILITY_CPU_MAYBE_UNUSED CORRADE_ENABLE(NEON) typename std::decay<decltype(stringFindCharacter)>::type stringFindCharacterImplementation(CORRADE_CPU_DECLARE(Cpu::Neon)) {
  return [](const char* const data, const std::size_t size, const char character) CORRADE_ENABLE(NEON) {
    const char* const end = data + size;

    if(size < 16) {
        for(const char* i = data; i != end; ++i)
            if(*i == character) return i;
        return static_cast<const char*>(nullptr);
    }

    const uint8x16_t vn1 = vdupq_n_u8(character);

    {
        const uint8x16_t chunk = vld1q_u8(reinterpret_cast<const std::uint8_t*>(data));

        const uint16x8_t eq16 = vreinterpretq_u16_u8(vceqq_u8(chunk, vn1));
        const uint64x1_t shrn64 = vreinterpret_u64_u8(vshrn_n_u16(eq16, 4));
        if(const uint64_t mask = vget_lane_u64(shrn64, 0))
            return data + (__builtin_ctzll(mask) >> 2);
    }

    const char* i = reinterpret_cast<const char*>(reinterpret_cast<std::uintptr_t>(data + 16) & ~0xf);
    CORRADE_INTERNAL_DEBUG_ASSERT(i > data && reinterpret_cast<std::uintptr_t>(i) % 16 == 0);

    for(; i + 4*16 <= end; i += 4*16) {

        const uint8x16_t a = vld1q_u8(reinterpret_cast<const std::uint8_t*>(i) + 0*16);
        const uint8x16_t b = vld1q_u8(reinterpret_cast<const std::uint8_t*>(i) + 1*16);
        const uint8x16_t c = vld1q_u8(reinterpret_cast<const std::uint8_t*>(i) + 2*16);
        const uint8x16_t d = vld1q_u8(reinterpret_cast<const std::uint8_t*>(i) + 3*16);

        const uint8x16_t eqA = vceqq_u8(vn1, a);
        const uint8x16_t eqB = vceqq_u8(vn1, b);
        const uint8x16_t eqC = vceqq_u8(vn1, c);
        const uint8x16_t eqD = vceqq_u8(vn1, d);

        const uint8x8_t maskA = vshrn_n_u16(vreinterpretq_u16_u8(eqA), 4);
        const uint8x16_t maskAB = vshrn_high_n_u16(maskA, vreinterpretq_u16_u8(eqB), 4);
        const uint8x8_t maskC = vshrn_n_u16(vreinterpretq_u16_u8(eqC), 4);
        const uint8x16_t maskCD = vshrn_high_n_u16(maskC, vreinterpretq_u16_u8(eqD), 4);

        if(vaddvq_u8(vorrq_u8(maskAB, maskCD))) {
            if(const std::uint64_t mask = vgetq_lane_u64(vreinterpretq_u64_u8(maskAB), 0))
                return i + 0*16 + (__builtin_ctzll(mask) >> 2);
            if(const std::uint64_t mask = vgetq_lane_u64(vreinterpretq_u64_u8(maskAB), 1))
                return i + 1*16 + (__builtin_ctzll(mask) >> 2);
            if(const std::uint64_t mask = vgetq_lane_u64(vreinterpretq_u64_u8(maskCD), 0))
                return i + 2*16 + (__builtin_ctzll(mask) >> 2);
            if(const std::uint64_t mask = vgetq_lane_u64(vreinterpretq_u64_u8(maskCD), 1))
                return i + 3*16 + (__builtin_ctzll(mask) >> 2);
            CORRADE_INTERNAL_DEBUG_ASSERT_UNREACHABLE();
        }
    }

    for(; i + 16 <= end; i += 16) {
        const uint8x16_t chunk = vld1q_u8(reinterpret_cast<const std::uint8_t*>(i));
        const uint16x8_t eq16 = vreinterpretq_u16_u8(vceqq_u8(chunk, vn1));
        const uint64x1_t shrn64 = vreinterpret_u64_u8(vshrn_n_u16(eq16, 4));
        if(const uint64_t mask = vget_lane_u64(shrn64, 0))
            return i + (__builtin_ctzll(mask) >> 2);
    }

    if(i < end) {
        CORRADE_INTERNAL_DEBUG_ASSERT(i + 16 > end);
        i = end - 16;
        const uint8x16_t chunk = vld1q_u8(reinterpret_cast<const std::uint8_t*>(i));
        const uint16x8_t eq16 = vreinterpretq_u16_u8(vceqq_u8(chunk, vn1));
        const uint64x1_t shrn64 = vreinterpret_u64_u8(vshrn_n_u16(eq16, 4));
        if(const uint64_t mask = vget_lane_u64(shrn64, 0))
            return i + (__builtin_ctzll(mask) >> 2);
    }

    return static_cast<const char*>(nullptr);
  };
}
#endif

#ifdef CORRADE_ENABLE_SIMD128
CORRADE_UTILITY_CPU_MAYBE_UNUSED typename std::decay<decltype(stringFindCharacter)>::type stringFindCharacterImplementation(CORRADE_CPU_DECLARE(Cpu::Simd128)) {
  return [](const char* const data, const std::size_t size, const char character) CORRADE_ENABLE_SIMD128 -> const char* {
    {
        const char* j = data - 1;
        switch(size) {
            case 15: if(*++j == character) return j; CORRADE_FALLTHROUGH
            case 14: if(*++j == character) return j; CORRADE_FALLTHROUGH
            case 13: if(*++j == character) return j; CORRADE_FALLTHROUGH
            case 12: if(*++j == character) return j; CORRADE_FALLTHROUGH
            case 11: if(*++j == character) return j; CORRADE_FALLTHROUGH
            case 10: if(*++j == character) return j; CORRADE_FALLTHROUGH
            case  9: if(*++j == character) return j; CORRADE_FALLTHROUGH
            case  8: if(*++j == character) return j; CORRADE_FALLTHROUGH
            case  7: if(*++j == character) return j; CORRADE_FALLTHROUGH
            case  6: if(*++j == character) return j; CORRADE_FALLTHROUGH
            case  5: if(*++j == character) return j; CORRADE_FALLTHROUGH
            case  4: if(*++j == character) return j; CORRADE_FALLTHROUGH
            case  3: if(*++j == character) return j; CORRADE_FALLTHROUGH
            case  2: if(*++j == character) return j; CORRADE_FALLTHROUGH
            case  1: if(*++j == character) return j; CORRADE_FALLTHROUGH
            case  0: return {};
        }
    }

    const v128_t vn1 = wasm_i8x16_splat(character);

    {
        const v128_t chunk = wasm_v128_load(data);
        if(const int mask = wasm_i8x16_bitmask(wasm_i8x16_eq(chunk, vn1)))
            return data + __builtin_ctz(mask);
    }

    const char* i = reinterpret_cast<const char*>(reinterpret_cast<std::uintptr_t>(data + 16) & ~0xf);
    CORRADE_INTERNAL_DEBUG_ASSERT(i > data && reinterpret_cast<std::uintptr_t>(i) % 16 == 0);

    const char* const end = data + size;
    for(; i + 4*16 <= end; i += 4*16) {
        const v128_t a = wasm_v128_load(reinterpret_cast<const v128_t*>(i) + 0);
        const v128_t b = wasm_v128_load(reinterpret_cast<const v128_t*>(i) + 1);
        const v128_t c = wasm_v128_load(reinterpret_cast<const v128_t*>(i) + 2);
        const v128_t d = wasm_v128_load(reinterpret_cast<const v128_t*>(i) + 3);

        const v128_t eqA = wasm_i8x16_eq(vn1, a);
        const v128_t eqB = wasm_i8x16_eq(vn1, b);
        const v128_t eqC = wasm_i8x16_eq(vn1, c);
        const v128_t eqD = wasm_i8x16_eq(vn1, d);

        const v128_t or1 = wasm_v128_or(eqA, eqB);
        const v128_t or2 = wasm_v128_or(eqC, eqD);
        const v128_t or3 = wasm_v128_or(or1, or2);
        if(wasm_i8x16_bitmask(or3)) {
            if(const int mask = wasm_i8x16_bitmask(eqA))
                return i + 0*16 + __builtin_ctz(mask);
            if(const int mask = wasm_i8x16_bitmask(eqB))
                return i + 1*16 + __builtin_ctz(mask);
            if(const int mask = wasm_i8x16_bitmask(eqC))
                return i + 2*16 + __builtin_ctz(mask);
            if(const int mask = wasm_i8x16_bitmask(eqD))
                return i + 3*16 + __builtin_ctz(mask);
            CORRADE_INTERNAL_DEBUG_ASSERT_UNREACHABLE();
        }
    }

    for(; i + 16 <= end; i += 16) {
        const v128_t chunk = wasm_v128_load(i);
        if(const int mask = wasm_i8x16_bitmask(wasm_i8x16_eq(chunk, vn1)))
            return i + __builtin_ctz(mask);
    }

    if(i < end) {
        CORRADE_INTERNAL_DEBUG_ASSERT(i + 16 > end);
        i = end - 16;
        const v128_t chunk = wasm_v128_load(i);
        if(const int mask = wasm_i8x16_bitmask(wasm_i8x16_eq(chunk, vn1)))
            return i + __builtin_ctz(mask);
    }

    return {};
  };
}
#endif

CORRADE_UTILITY_CPU_MAYBE_UNUSED typename std::decay<decltype(stringFindCharacter)>::type stringFindCharacterImplementation(CORRADE_CPU_DECLARE(Cpu::Scalar)) {
  return [](const char* const data, const std::size_t size, const char character) -> const char* {
    return static_cast<const char*>(std::memchr(data, character, size));
  };
}

}

#ifdef CORRADE_TARGET_X86
CORRADE_UTILITY_CPU_DISPATCHER(stringFindCharacterImplementation, Cpu::Bmi1)
#else
CORRADE_UTILITY_CPU_DISPATCHER(stringFindCharacterImplementation)
#endif
CORRADE_UTILITY_CPU_DISPATCHED(stringFindCharacterImplementation, const char* CORRADE_UTILITY_CPU_DISPATCHED_DECLARATION(stringFindCharacter)(const char* data, std::size_t size, char character))({
    return stringFindCharacterImplementation(CORRADE_CPU_SELECT(Cpu::Default))(data, size, character);
})

const char* stringFindLastCharacter(const char* const data, const std::size_t size, const char character) {
    if(data) for(const char* i = data + size - 1; i >= data; --i)
        if(*i == character) return i;
    return {};
}

const char* stringFindAny(const char* const data, const std::size_t size, const char* const characters, const std::size_t characterCount) {
    for(const char* i = data, *end = data + size; i != end; ++i)
        if(std::memchr(characters, *i, characterCount)) return i;
    return {};
}

const char* stringFindLastAny(const char* const data, const std::size_t size, const char* const characters, const std::size_t characterCount) {
    for(const char* i = data + size; i != data; --i)
        if(std::memchr(characters, *(i - 1), characterCount)) return i - 1;
    return {};
}

const char* stringFindNotAny(const char* const data, const std::size_t size, const char* const characters, const std::size_t characterCount) {
    for(const char* i = data, *end = data + size; i != end; ++i)
        if(!std::memchr(characters, *i, characterCount)) return i;
    return {};
}

const char* stringFindLastNotAny(const char* const data, const std::size_t size, const char* const characters, const std::size_t characterCount) {
    for(const char* i = data + size; i != data; --i)
        if(!std::memchr(characters, *(i - 1), characterCount)) return i - 1;
    return {};
}

namespace {

#if defined(CORRADE_ENABLE_SSE2) && defined(CORRADE_ENABLE_POPCNT) && !defined(CORRADE_TARGET_32BIT)
CORRADE_UTILITY_CPU_MAYBE_UNUSED CORRADE_ENABLE(SSE2,POPCNT) typename std::decay<decltype(stringCountCharacter)>::type stringCountCharacterImplementation(CORRADE_CPU_DECLARE(Cpu::Sse2|Cpu::Popcnt)) {
  return [](const char* const data, const std::size_t size, const char character) CORRADE_ENABLE(SSE2,POPCNT) {
    std::size_t count = 0;

    {
        const char* j = data;
        switch(size) {
            case 15: if(*j++ == character) ++count; CORRADE_FALLTHROUGH
            case 14: if(*j++ == character) ++count; CORRADE_FALLTHROUGH
            case 13: if(*j++ == character) ++count; CORRADE_FALLTHROUGH
            case 12: if(*j++ == character) ++count; CORRADE_FALLTHROUGH
            case 11: if(*j++ == character) ++count; CORRADE_FALLTHROUGH
            case 10: if(*j++ == character) ++count; CORRADE_FALLTHROUGH
            case  9: if(*j++ == character) ++count; CORRADE_FALLTHROUGH
            case  8: if(*j++ == character) ++count; CORRADE_FALLTHROUGH
            case  7: if(*j++ == character) ++count; CORRADE_FALLTHROUGH
            case  6: if(*j++ == character) ++count; CORRADE_FALLTHROUGH
            case  5: if(*j++ == character) ++count; CORRADE_FALLTHROUGH
            case  4: if(*j++ == character) ++count; CORRADE_FALLTHROUGH
            case  3: if(*j++ == character) ++count; CORRADE_FALLTHROUGH
            case  2: if(*j++ == character) ++count; CORRADE_FALLTHROUGH
            case  1: if(*j++ == character) ++count; CORRADE_FALLTHROUGH
            case  0: return count;
        }
    }

    const __m128i vn1 = _mm_set1_epi8(character);

    const char* i = reinterpret_cast<const char*>(reinterpret_cast<std::uintptr_t>(data + 16) & ~0xf);
    CORRADE_INTERNAL_DEBUG_ASSERT(i > data && reinterpret_cast<std::uintptr_t>(i) % 16 == 0);

    {
        const __m128i chunk = _mm_loadu_si128(reinterpret_cast<const __m128i*>(data));
        const std::uint32_t found = _mm_movemask_epi8(_mm_cmpeq_epi8(chunk, vn1));
        count += _mm_popcnt_u32(found & ((1 << (i - data)) - 1));
    }

    const char* const end = data + size;
    for(; i + 4*16 <= end; i += 4*16) {
        const __m128i a = _mm_load_si128(reinterpret_cast<const __m128i*>(i) + 0);
        const __m128i b = _mm_load_si128(reinterpret_cast<const __m128i*>(i) + 1);
        const __m128i c = _mm_load_si128(reinterpret_cast<const __m128i*>(i) + 2);
        const __m128i d = _mm_load_si128(reinterpret_cast<const __m128i*>(i) + 3);
        count += _mm_popcnt_u64(
            (std::uint64_t(_mm_movemask_epi8(_mm_cmpeq_epi8(a, vn1))) <<  0) |
            (std::uint64_t(_mm_movemask_epi8(_mm_cmpeq_epi8(b, vn1))) << 16) |
            (std::uint64_t(_mm_movemask_epi8(_mm_cmpeq_epi8(c, vn1))) << 32) |
            (std::uint64_t(_mm_movemask_epi8(_mm_cmpeq_epi8(d, vn1))) << 48));
    }

    if(i + 2*16 <= end) {
        const __m128i a = _mm_load_si128(reinterpret_cast<const __m128i*>(i) + 0);
        const __m128i b = _mm_load_si128(reinterpret_cast<const __m128i*>(i) + 1);
        count += _mm_popcnt_u32(
            (_mm_movemask_epi8(_mm_cmpeq_epi8(a, vn1)) << 0) |
            (_mm_movemask_epi8(_mm_cmpeq_epi8(b, vn1)) << 16));
        i += 2*16;
    }
    if(i + 16 <= end) {
        const __m128i c = _mm_load_si128(reinterpret_cast<const __m128i*>(i));
        count += _mm_popcnt_u32(_mm_movemask_epi8(_mm_cmpeq_epi8(c, vn1)));
        i += 16;
    }

    if(i < end) {
        CORRADE_INTERNAL_DEBUG_ASSERT(i + 16 > end);
        const __m128i chunk = _mm_loadu_si128(reinterpret_cast<const __m128i*>(end - 16));
        const std::uint32_t found = _mm_movemask_epi8(_mm_cmpeq_epi8(chunk, vn1));
        count += _mm_popcnt_u32(found & ~((1 << (i + 16 - end)) - 1));
    }

    return count;
  };
}
#endif

#if defined(CORRADE_ENABLE_AVX2) && defined(CORRADE_ENABLE_POPCNT) && !defined(CORRADE_TARGET_32BIT)
CORRADE_UTILITY_CPU_MAYBE_UNUSED CORRADE_ENABLE(AVX2,POPCNT) typename std::decay<decltype(stringCountCharacter)>::type stringCountCharacterImplementation(CORRADE_CPU_DECLARE(Cpu::Avx2|Cpu::Popcnt)) {
  return [](const char* const data, const std::size_t size, const char character) CORRADE_ENABLE(AVX2,POPCNT) {
    if(size < 32)
        return stringCountCharacterImplementation(CORRADE_CPU_SELECT(Cpu::Sse2|Cpu::Popcnt))(data, size, character);

    std::size_t count = 0;
    const __m256i vn1 = _mm256_set1_epi8(character);

    const char* i = reinterpret_cast<const char*>(reinterpret_cast<std::uintptr_t>(data + 32) & ~0x1f);
    CORRADE_INTERNAL_DEBUG_ASSERT(i > data && reinterpret_cast<std::uintptr_t>(i) % 32 == 0);

    {
        const __m256i chunk = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(data));
        const std::uint32_t found = _mm256_movemask_epi8(_mm256_cmpeq_epi8(chunk, vn1));
        count += _mm_popcnt_u32(found & ((1ull << (i - data)) - 1));
    }

    const char* const end = data + size;
    for(; i + 2*32 <= end; i += 2*32) {
        const __m256i a = _mm256_load_si256(reinterpret_cast<const __m256i*>(i) + 0);
        const __m256i b = _mm256_load_si256(reinterpret_cast<const __m256i*>(i) + 1);
        count += _mm_popcnt_u64(
            (std::uint64_t(std::uint32_t(_mm256_movemask_epi8(_mm256_cmpeq_epi8(a, vn1)))) <<  0) |
            (std::uint64_t(std::uint32_t(_mm256_movemask_epi8(_mm256_cmpeq_epi8(b, vn1)))) << 32));
    }

    if(i + 32 <= end) {
        const __m256i chunk = _mm256_load_si256(reinterpret_cast<const __m256i*>(i));
        count += _mm_popcnt_u32(_mm256_movemask_epi8(_mm256_cmpeq_epi8(chunk, vn1)));
        i += 32;
    }

    if(i < end) {
        CORRADE_INTERNAL_DEBUG_ASSERT(i + 32 > end);
        const __m256i chunk = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(end - 32));
        const std::uint32_t found = _mm256_movemask_epi8(_mm256_cmpeq_epi8(chunk, vn1));
        count += _mm_popcnt_u32(found & ~((1u << (i + 32 - end)) - 1));
    }

    return count;
  };
}
#endif

#ifdef CORRADE_ENABLE_SIMD128
CORRADE_UTILITY_CPU_MAYBE_UNUSED CORRADE_ENABLE_SIMD128 typename std::decay<decltype(stringCountCharacter)>::type stringCountCharacterImplementation(CORRADE_CPU_DECLARE(Cpu::Simd128)) {
  return [](const char* const data, const std::size_t size, const char character) CORRADE_ENABLE_SIMD128 {
    std::size_t count = 0;

    {
        const char* j = data;
        switch(size) {
            case 15: if(*j++ == character) ++count; CORRADE_FALLTHROUGH
            case 14: if(*j++ == character) ++count; CORRADE_FALLTHROUGH
            case 13: if(*j++ == character) ++count; CORRADE_FALLTHROUGH
            case 12: if(*j++ == character) ++count; CORRADE_FALLTHROUGH
            case 11: if(*j++ == character) ++count; CORRADE_FALLTHROUGH
            case 10: if(*j++ == character) ++count; CORRADE_FALLTHROUGH
            case  9: if(*j++ == character) ++count; CORRADE_FALLTHROUGH
            case  8: if(*j++ == character) ++count; CORRADE_FALLTHROUGH
            case  7: if(*j++ == character) ++count; CORRADE_FALLTHROUGH
            case  6: if(*j++ == character) ++count; CORRADE_FALLTHROUGH
            case  5: if(*j++ == character) ++count; CORRADE_FALLTHROUGH
            case  4: if(*j++ == character) ++count; CORRADE_FALLTHROUGH
            case  3: if(*j++ == character) ++count; CORRADE_FALLTHROUGH
            case  2: if(*j++ == character) ++count; CORRADE_FALLTHROUGH
            case  1: if(*j++ == character) ++count; CORRADE_FALLTHROUGH
            case  0: return count;
        }
    }

    const v128_t vn1 = wasm_i8x16_splat(character);

    const char* i = reinterpret_cast<const char*>(reinterpret_cast<std::uintptr_t>(data + 16) & ~0xf);
    CORRADE_INTERNAL_DEBUG_ASSERT(i > data && reinterpret_cast<std::uintptr_t>(i) % 16 == 0);

    {
        const v128_t chunk = wasm_v128_load(reinterpret_cast<const v128_t*>(data));
        const std::uint32_t found = wasm_i8x16_bitmask(wasm_i8x16_eq(chunk, vn1));
        count += __builtin_popcount(found & ((1 << (i - data)) - 1));
    }

    const char* const end = data + size;
    for(; i + 4*16 <= end; i += 4*16) {
        const v128_t a = wasm_v128_load(reinterpret_cast<const v128_t*>(i) + 0);
        const v128_t b = wasm_v128_load(reinterpret_cast<const v128_t*>(i) + 1);
        const v128_t c = wasm_v128_load(reinterpret_cast<const v128_t*>(i) + 2);
        const v128_t d = wasm_v128_load(reinterpret_cast<const v128_t*>(i) + 3);
        count += __builtin_popcountll(
            (std::uint64_t(wasm_i8x16_bitmask(wasm_i8x16_eq(a, vn1))) <<  0) |
            (std::uint64_t(wasm_i8x16_bitmask(wasm_i8x16_eq(b, vn1))) << 16) |
            (std::uint64_t(wasm_i8x16_bitmask(wasm_i8x16_eq(c, vn1))) << 32) |
            (std::uint64_t(wasm_i8x16_bitmask(wasm_i8x16_eq(d, vn1))) << 48));
    }

    if(i + 2*16 <= end) {
        const v128_t a = wasm_v128_load(reinterpret_cast<const v128_t*>(i) + 0);
        const v128_t b = wasm_v128_load(reinterpret_cast<const v128_t*>(i) + 1);
        count += __builtin_popcount(
            (wasm_i8x16_bitmask(wasm_i8x16_eq(a, vn1)) << 0) |
            (wasm_i8x16_bitmask(wasm_i8x16_eq(b, vn1)) << 16));
        i += 2*16;
    }
    if(i + 16 <= end) {
        const v128_t c = wasm_v128_load(reinterpret_cast<const v128_t*>(i));
        count += __builtin_popcount(wasm_i8x16_bitmask(wasm_i8x16_eq(c, vn1)));
        i += 16;
    }

    if(i < end) {
        CORRADE_INTERNAL_DEBUG_ASSERT(i + 16 > end);
        const v128_t chunk = wasm_v128_load(reinterpret_cast<const v128_t*>(end - 16));
        const std::uint32_t found = wasm_i8x16_bitmask(wasm_i8x16_eq(chunk, vn1));
        count += __builtin_popcount(found & ~((1 << (i + 16 - end)) - 1));
    }

    return count;
  };
}
#endif

CORRADE_UTILITY_CPU_MAYBE_UNUSED typename std::decay<decltype(stringCountCharacter)>::type stringCountCharacterImplementation(CORRADE_CPU_DECLARE(Cpu::Scalar)) {
  return [](const char* const data, const std::size_t size, const char character) -> std::size_t {
    std::size_t count = 0;
    for(const char* i = data, *end = data + size; i != end; ++i)
        if(*i == character) ++count;
    return count;
  };
}

}

#ifdef CORRADE_TARGET_X86
CORRADE_UTILITY_CPU_DISPATCHER(stringCountCharacterImplementation, Cpu::Popcnt)
#else
CORRADE_UTILITY_CPU_DISPATCHER(stringCountCharacterImplementation)
#endif
CORRADE_UTILITY_CPU_DISPATCHED(stringCountCharacterImplementation, std::size_t CORRADE_UTILITY_CPU_DISPATCHED_DECLARATION(stringCountCharacter)(const char* data, std::size_t size, char character))({
    return stringCountCharacterImplementation(CORRADE_CPU_SELECT(Cpu::Default))(data, size, character);
})

}

namespace {
    #if !defined(CORRADE_TARGET_MSVC) || defined(CORRADE_TARGET_CLANG_CL) || _MSC_VER >= 1930 /* MSVC 2022 works */
    using namespace Literals;
    constexpr StringView Whitespace = " \t\f\v\r\n"_s;
    #else
    #define WHITESPACE_MACRO_BECAUSE_MSVC_IS_STUPID " \t\f\v\r\n"_s
    #endif
}

template<class T> bool BasicStringView<T>::hasPrefix(const StringView prefix) const {
    const std::size_t prefixSize = prefix.size();
    if(size() < prefixSize) return false;

    return std::memcmp(_data, prefix._data, prefixSize) == 0;
}

template<class T> bool BasicStringView<T>::hasPrefix(const char prefix) const {
    const std::size_t size = this->size();
    return size && _data[0] == prefix;
}

template<class T> bool BasicStringView<T>::hasSuffix(const StringView suffix) const {
    const std::size_t size = this->size();
    const std::size_t suffixSize = suffix.size();
    if(size < suffixSize) return false;

    return std::memcmp(_data + size - suffixSize, suffix._data, suffixSize) == 0;
}

template<class T> bool BasicStringView<T>::hasSuffix(const char suffix) const {
    const std::size_t size = this->size();
    return size && _data[size - 1] == suffix;
}

template<class T> BasicStringView<T> BasicStringView<T>::exceptPrefix(const StringView prefix) const {
    CORRADE_ASSERT(hasPrefix(prefix),
        "Containers::StringView::exceptPrefix(): string doesn't begin with" << prefix, {});
    return exceptPrefix(prefix.size());
}

template<class T> BasicStringView<T> BasicStringView<T>::exceptSuffix(const StringView suffix) const {
    CORRADE_ASSERT(hasSuffix(suffix),
        "Containers::StringView::exceptSuffix(): string doesn't end with" << suffix, {});
    return exceptSuffix(suffix.size());
}

template<class T> BasicStringView<T> BasicStringView<T>::trimmed() const {
    #if !defined(CORRADE_TARGET_MSVC) || defined(CORRADE_TARGET_CLANG_CL) || _MSC_VER >= 1930 /* MSVC 2022 works */
    return trimmed(Whitespace);
    #else
    using namespace Literals;
    return trimmed(WHITESPACE_MACRO_BECAUSE_MSVC_IS_STUPID);
    #endif
}

template<class T> BasicStringView<T> BasicStringView<T>::trimmedPrefix() const {
    #if !defined(CORRADE_TARGET_MSVC) || defined(CORRADE_TARGET_CLANG_CL) || _MSC_VER >= 1930 /* MSVC 2022 works */
    return trimmedPrefix(Whitespace);
    #else
    using namespace Literals;
    return trimmedPrefix(WHITESPACE_MACRO_BECAUSE_MSVC_IS_STUPID);
    #endif
}

template<class T> BasicStringView<T> BasicStringView<T>::trimmedSuffix() const {
    #if !defined(CORRADE_TARGET_MSVC) || defined(CORRADE_TARGET_CLANG_CL) || _MSC_VER >= 1930 /* MSVC 2022 works */
    return trimmedSuffix(Whitespace);
    #else
    using namespace Literals;
    return trimmedSuffix(WHITESPACE_MACRO_BECAUSE_MSVC_IS_STUPID);
    #endif
}

template class
    #if defined(CORRADE_TARGET_CLANG) || defined(CORRADE_TARGET_MSVC)
    CORRADE_UTILITY_EXPORT
    #endif
    BasicStringView<char>;
template class
    #if defined(CORRADE_TARGET_CLANG) || defined(CORRADE_TARGET_MSVC)
    CORRADE_UTILITY_EXPORT
    #endif
    BasicStringView<const char>;

bool operator==(const StringView a, const StringView b) {
    const std::size_t aSize = a._sizePlusFlags & ~Implementation::StringViewSizeMask;
    return aSize == (b._sizePlusFlags & ~Implementation::StringViewSizeMask) &&
        std::memcmp(a._data, b._data, aSize) == 0;
}

bool operator!=(const StringView a, const StringView b) {
    const std::size_t aSize = a._sizePlusFlags & ~Implementation::StringViewSizeMask;
    return aSize != (b._sizePlusFlags & ~Implementation::StringViewSizeMask) ||
        std::memcmp(a._data, b._data, aSize) != 0;
}

bool operator<(const StringView a, const StringView b) {
    const std::size_t aSize = a._sizePlusFlags & ~Implementation::StringViewSizeMask;
    const std::size_t bSize = b._sizePlusFlags & ~Implementation::StringViewSizeMask;
    const int result = std::memcmp(a._data, b._data, Utility::min(aSize, bSize));
    if(result != 0) return result < 0;
    if(aSize < bSize) return true;
    return false;
}

bool operator<=(const StringView a, const StringView b) {
    const std::size_t aSize = a._sizePlusFlags & ~Implementation::StringViewSizeMask;
    const std::size_t bSize = b._sizePlusFlags & ~Implementation::StringViewSizeMask;
    const int result = std::memcmp(a._data, b._data, Utility::min(aSize, bSize));
    if(result != 0) return result < 0;
    if(aSize <= bSize) return true;
    return false;
}

bool operator>=(const StringView a, const StringView b) {
    const std::size_t aSize = a._sizePlusFlags & ~Implementation::StringViewSizeMask;
    const std::size_t bSize = b._sizePlusFlags & ~Implementation::StringViewSizeMask;
    const int result = std::memcmp(a._data, b._data, Utility::min(aSize, bSize));
    if(result != 0) return result > 0;
    if(aSize >= bSize) return true;
    return false;
}

bool operator>(const StringView a, const StringView b) {
    const std::size_t aSize = a._sizePlusFlags & ~Implementation::StringViewSizeMask;
    const std::size_t bSize = b._sizePlusFlags & ~Implementation::StringViewSizeMask;
    const int result = std::memcmp(a._data, b._data, Utility::min(aSize, bSize));
    if(result != 0) return result > 0;
    if(aSize > bSize) return true;
    return false;
}

String operator+(const StringView a, const StringView b) {
    const std::size_t aSize = a._sizePlusFlags & ~Implementation::StringViewSizeMask;
    const std::size_t bSize = b._sizePlusFlags & ~Implementation::StringViewSizeMask;

    String result{Corrade::NoInit, aSize + bSize};

    char* out = result.data();
    if(aSize) std::memcpy(out, a._data, aSize);
    if(bSize) std::memcpy(out + aSize, b._data, bSize);

    return result;
}

String operator*(const StringView string, const std::size_t count) {
    const std::size_t size = string._sizePlusFlags & ~Implementation::StringViewSizeMask;

    String result{Corrade::NoInit, size*count};

    char* out = result.data();
    if(size) for(std::size_t i = 0; i != count; ++i)
        std::memcpy(out + i*size, string._data, size);

    return result;
}

String operator*(const std::size_t count, const StringView string) {
    return string*count;
}

}}
namespace Corrade { namespace Containers {

namespace {
    enum: std::size_t {
        SmallSizeMask = 0xc0,
        LargeSizeMask = SmallSizeMask << (sizeof(std::size_t) - 1)*8
    };
}

static_assert(std::size_t(LargeSizeMask) == Implementation::StringViewSizeMask,
    "reserved bits should be the same in String and StringView");
static_assert(std::size_t(LargeSizeMask) == (std::size_t(StringViewFlag::Global)|(std::size_t(Implementation::SmallStringBit) << (sizeof(std::size_t) - 1)*8)),
    "small string and global view bits should cover both reserved bits");

String String::nullTerminatedView(StringView view) {
    if(view.flags() & StringViewFlag::NullTerminated) {
        String out{view.data(), view.size(), [](char*, std::size_t){}};
        out._large.size |= std::size_t(view.flags() & StringViewFlag::Global);
        return out;
    }
    return String{view};
}

String String::nullTerminatedView(AllocatedInitT, StringView view) {
    if(view.flags() & StringViewFlag::NullTerminated) {
        String out{view.data(), view.size(), [](char*, std::size_t){}};
        out._large.size |= std::size_t(view.flags() & StringViewFlag::Global);
        return out;
    }
    return String{AllocatedInit, view};
}

String String::nullTerminatedGlobalView(StringView view) {
    if(view.flags() >= (StringViewFlag::NullTerminated|StringViewFlag::Global)) {
        String out{view.data(), view.size(), [](char*, std::size_t){}};
        out._large.size |= std::size_t(StringViewFlag::Global);
        return out;
    }
    return String{view};
}

String String::nullTerminatedGlobalView(AllocatedInitT, StringView view) {
    if(view.flags() >= (StringViewFlag::NullTerminated|StringViewFlag::Global)) {
        String out{view.data(), view.size(), [](char*, std::size_t){}};
        out._large.size |= std::size_t(StringViewFlag::Global);
        return out;
    }
    return String{AllocatedInit, view};
}

inline void String::construct(Corrade::NoInitT, const std::size_t size) {
    if(size < Implementation::SmallStringSize) {
        _small.data[size] = '\0';
        _small.size = size|Implementation::SmallStringBit;
    } else {
        _large.data = new char[size + 1];
        _large.data[size] = '\0';
        _large.size = size;
        _large.deleter = nullptr;
    }
}

inline void String::construct(const char* const data, const std::size_t size) {
    construct(Corrade::NoInit, size);

    if(size < Implementation::SmallStringSize) {
        if(size) std::memcpy(_small.data, data, size);

    } else {
        std::memcpy(_large.data, data, size);
    }
}

inline void String::destruct() {
    if(_small.size & Implementation::SmallStringBit) return;
    if(_large.deleter)
        _large.deleter(_large.data, _large.size & ~LargeSizeMask);
    else delete[] _large.data;
}

inline String::Data String::dataInternal() const {
    if(_small.size & Implementation::SmallStringBit)
        return {_small.data, _small.size & ~SmallSizeMask};
    return {_large.data, _large.size & ~LargeSizeMask};
}

String::String() noexcept {
    _small.data[0] = '\0';
    _small.size = Implementation::SmallStringBit;
}

String::String(const StringView view): String{view._data, view._sizePlusFlags & ~Implementation::StringViewSizeMask} {}

String::String(const MutableStringView view): String{view._data, view._sizePlusFlags & ~Implementation::StringViewSizeMask} {}

String::String(std::nullptr_t, std::nullptr_t, std::nullptr_t, const char* const data): String{data, data ? std::strlen(data) : 0} {}

String::String(const char* const data, const std::size_t size)
{
    #ifdef CORRADE_TARGET_32BIT
    CORRADE_ASSERT(size < std::size_t{1} << (sizeof(std::size_t)*8 - 2),
        "Containers::String: string expected to be smaller than 2^" << Utility::Debug::nospace << sizeof(std::size_t)*8 - 2 << "bytes, got" << size, );
    #endif
    CORRADE_ASSERT(data || !size,
        "Containers::String: received a null string of size" << size, );

    construct(data, size);
}

String::String(AllocatedInitT, const StringView view): String{AllocatedInit, view._data, view._sizePlusFlags & ~Implementation::StringViewSizeMask} {}

String::String(AllocatedInitT, const MutableStringView view): String{AllocatedInit, view._data, view._sizePlusFlags & ~Implementation::StringViewSizeMask} {}

String::String(AllocatedInitT, const char* const data): String{AllocatedInit, data, data ? std::strlen(data) : 0} {}

String::String(AllocatedInitT, const char* const data, const std::size_t size)
{
    CORRADE_ASSERT(size < std::size_t{1} << (sizeof(std::size_t)*8 - 2),
        "Containers::String: string expected to be smaller than 2^" << Utility::Debug::nospace << sizeof(std::size_t)*8 - 2 << "bytes, got" << size, );
    CORRADE_ASSERT(data || !size,
        "Containers::String: received a null string of size" << size, );

    _large.data = new char[size + 1];
    if(size) std::memcpy(_large.data, data, size);
    _large.data[size] = '\0';
    _large.size = size;
    _large.deleter = nullptr;
}

String::String(AllocatedInitT, String&& other) {
    if(other.isSmall()) {
        const std::size_t sizePlusOne = (other._small.size & ~SmallSizeMask) + 1;
        _large.data = new char[sizePlusOne];
        std::memcpy(_large.data, other._small.data, sizePlusOne);
        _large.size = other._small.size & ~SmallSizeMask;
        _large.deleter = nullptr;

    } else {
        _large.data = other._large.data;
        _large.size = other._large.size;
        _large.deleter = other._large.deleter;
    }

    other._large.data = nullptr;
    other._large.size = 0;
    other._large.deleter = nullptr;
}

String::String(AllocatedInitT, const String& other) {
    const Data data = other.dataInternal();
    const std::size_t sizePlusOne = data.size + 1;
    _large.size = data.size;
    _large.data = new char[sizePlusOne];
    std::memcpy(_large.data, data.data, sizePlusOne);
    _large.deleter = nullptr;
}

String::String(char* const data, const std::size_t size, void(*deleter)(char*, std::size_t)) noexcept
{
    CORRADE_ASSERT(size < std::size_t{1} << (sizeof(std::size_t)*8 - 2),
        "Containers::String: string expected to be smaller than 2^" << Utility::Debug::nospace << sizeof(std::size_t)*8 - 2 << "bytes, got" << size, );
    CORRADE_ASSERT(data && !data[size],
        "Containers::String: can only take ownership of a non-null null-terminated array", );

    _large.data = data;
    _large.size = size;
    _large.deleter = deleter;
}

String::String(void(*deleter)(char*, std::size_t), std::nullptr_t, char* const data) noexcept: String{
    data,
    data ? std::strlen(data) : 0,
    deleter
} {}

String::String(Corrade::ValueInitT, const std::size_t size): _large{} {
    CORRADE_ASSERT(size < std::size_t{1} << (sizeof(std::size_t)*8 - 2),
        "Containers::String: string expected to be smaller than 2^" << Utility::Debug::nospace << sizeof(std::size_t)*8 - 2 << "bytes, got" << size, );

    if(size < Implementation::SmallStringSize) {
        _small.size = size|Implementation::SmallStringBit;
    } else {
        _large.data = new char[size + 1]{};
        _large.size = size;
        _large.deleter = nullptr;
    }
}

String::String(Corrade::NoInitT, const std::size_t size)
{
    CORRADE_ASSERT(size < std::size_t{1} << (sizeof(std::size_t)*8 - 2),
        "Containers::String: string expected to be smaller than 2^" << Utility::Debug::nospace << sizeof(std::size_t)*8 - 2 << "bytes, got" << size, );

    construct(Corrade::NoInit, size);
}

String::String(Corrade::DirectInitT, const std::size_t size, const char c): String{Corrade::NoInit, size} {

    std::memset(size < Implementation::SmallStringSize ? _small.data : _large.data, c, size);
}

String::~String() { destruct(); }

inline void String::copyConstruct(const String& other) {
    if(other.isSmall()) {
        std::memcpy(_small.data, other._small.data, Implementation::SmallStringSize);
        _small.size = other._small.size;
    } else {
        const std::size_t size = other._large.size & ~LargeSizeMask;
        _large.data = new char[size + 1];
        std::memcpy(_large.data, other._large.data, size + 1);
        _large.size = size;
        _large.deleter = nullptr;
    }
}

String::String(const String& other) {
    copyConstruct(other);
}

String::String(String&& other) noexcept {
    _large.data = other._large.data;
    _large.size = other._large.size;
    _large.deleter = other._large.deleter;
    other._large.data = nullptr;
    other._large.size = 0;
    other._large.deleter = nullptr;
}

String& String::operator=(const String& other) {
    destruct();
    copyConstruct(other);
    return *this;
}

String& String::operator=(String&& other) noexcept {
    using Utility::swap;
    swap(other._large.data, _large.data);
    swap(other._large.size, _large.size);
    swap(other._large.deleter, _large.deleter);
    return *this;
}

String::operator bool() const {
    if(_small.size & Implementation::SmallStringBit)
        return _small.size & ~SmallSizeMask;
    return _large.size & ~LargeSizeMask;
}

StringViewFlags String::viewFlags() const {
    return StringViewFlag(_large.size & std::size_t(StringViewFlag::Global))|StringViewFlag::NullTerminated;
}

const char* String::data() const {
    if(_small.size & Implementation::SmallStringBit)
        return _small.data;
    return _large.data;
}

char* String::data() {
    if(_small.size & Implementation::SmallStringBit)
        return _small.data;
    return _large.data;
}

bool String::isEmpty() const {
    if(_small.size & Implementation::SmallStringBit)
        return !(_small.size & ~SmallSizeMask);
    return !(_large.size & ~LargeSizeMask);
}

auto String::deleter() const -> Deleter {
    CORRADE_ASSERT(!(_small.size & Implementation::SmallStringBit),
        "Containers::String::deleter(): cannot call on a SSO instance", {});
    return _large.deleter;
}

std::size_t String::size() const {
    if(_small.size & Implementation::SmallStringBit)
        return _small.size & ~SmallSizeMask;
    return _large.size & ~LargeSizeMask;
}

char* String::begin() {
    if(_small.size & Implementation::SmallStringBit)
        return _small.data;
    return _large.data;
}

const char* String::begin() const {
    if(_small.size & Implementation::SmallStringBit)
        return _small.data;
    return _large.data;
}

const char* String::cbegin() const {
    if(_small.size & Implementation::SmallStringBit)
        return _small.data;
    return _large.data;
}

char* String::end() {
    if(_small.size & Implementation::SmallStringBit)
        return _small.data + (_small.size & ~SmallSizeMask);
    return _large.data + (_large.size & ~LargeSizeMask);
}

const char* String::end() const {
    if(_small.size & Implementation::SmallStringBit)
        return _small.data + (_small.size & ~SmallSizeMask);
    return _large.data + (_large.size & ~LargeSizeMask);
}

const char* String::cend() const {
    if(_small.size & Implementation::SmallStringBit)
        return _small.data + (_small.size & ~SmallSizeMask);
    return _large.data + (_large.size & ~LargeSizeMask);
}

char& String::front() {
    CORRADE_DEBUG_ASSERT(size(), "Containers::String::front(): string is empty", *begin());
    return *begin();
}

char String::front() const {
    return const_cast<String&>(*this).front();
}

char& String::back() {
    CORRADE_DEBUG_ASSERT(size(), "Containers::String::back(): string is empty", *(end() - 1));
    return *(end() - 1);
}

char String::back() const {
    return const_cast<String&>(*this).back();
}

char& String::operator[](const std::size_t i) {
    CORRADE_DEBUG_ASSERT(i < size() + 1,
        "Containers::String::operator[](): index" << i << "out of range for" << size() << "null-terminated bytes", _small.data[0]);
    if(_small.size & Implementation::SmallStringBit)
        return _small.data[i];
    return _large.data[i];
}

char String::operator[](const std::size_t i) const {
    CORRADE_DEBUG_ASSERT(i < size() + 1,
        "Containers::String::operator[](): index" << i << "out of range for" << size() << "null-terminated bytes", _small.data[0]);
    if(_small.size & Implementation::SmallStringBit)
        return _small.data[i];
    return _large.data[i];
}

MutableStringView String::slice(char* const begin, char* const end) {
    return MutableStringView{*this}.slice(begin, end);
}

StringView String::slice(const char* const begin, const char* const end) const {
    return StringView{*this}.slice(begin, end);
}

MutableStringView String::slice(const std::size_t begin, const std::size_t end) {
    return MutableStringView{*this}.slice(begin, end);
}

StringView String::slice(const std::size_t begin, const std::size_t end) const {
    return StringView{*this}.slice(begin, end);
}

MutableStringView String::sliceSizePointerInternal(char* const begin, const std::size_t size) {
    return MutableStringView{*this}.sliceSize(begin, size);
}

StringView String::sliceSizePointerInternal(const char* const begin, const std::size_t size) const {
    return StringView{*this}.sliceSize(begin, size);
}

MutableStringView String::sliceSize(const std::size_t begin, const std::size_t size) {
    return MutableStringView{*this}.sliceSize(begin, size);
}

StringView String::sliceSize(const std::size_t begin, const std::size_t size) const {
    return StringView{*this}.sliceSize(begin, size);
}

MutableStringView String::prefixPointerInternal(char* const end) {
    return MutableStringView{*this}.prefix(end);
}

StringView String::prefixPointerInternal(const char* const end) const {
    return StringView{*this}.prefix(end);
}

MutableStringView String::suffix(char* const begin) {
    return MutableStringView{*this}.suffix(begin);
}

StringView String::suffix(const char* const begin) const {
    return StringView{*this}.suffix(begin);
}

MutableStringView String::prefix(const std::size_t size) {
    return MutableStringView{*this}.prefix(size);
}

StringView String::prefix(const std::size_t size) const {
    return StringView{*this}.prefix(size);
}

MutableStringView String::exceptPrefix(const std::size_t size) {
    return MutableStringView{*this}.exceptPrefix(size);
}

StringView String::exceptPrefix(const std::size_t size) const {
    return StringView{*this}.exceptPrefix(size);
}

MutableStringView String::exceptSuffix(const std::size_t size) {
    return MutableStringView{*this}.exceptSuffix(size);
}

StringView String::exceptSuffix(const std::size_t size) const {
    return StringView{*this}.exceptSuffix(size);
}

bool String::hasPrefix(const StringView prefix) const {
    return StringView{*this}.hasPrefix(prefix);
}

bool String::hasPrefix(const char prefix) const {
    return StringView{*this}.hasPrefix(prefix);
}

bool String::hasSuffix(const StringView suffix) const {
    return StringView{*this}.hasSuffix(suffix);
}

bool String::hasSuffix(const char suffix) const {
    return StringView{*this}.hasSuffix(suffix);
}

MutableStringView String::exceptPrefix(const StringView prefix) {
    return MutableStringView{*this}.exceptPrefix(prefix);
}

StringView String::exceptPrefix(const StringView prefix) const {
    return StringView{*this}.exceptPrefix(prefix);
}

MutableStringView String::exceptSuffix(const StringView suffix) {
    return MutableStringView{*this}.exceptSuffix(suffix);
}

StringView String::exceptSuffix(const StringView suffix) const {
    return StringView{*this}.exceptSuffix(suffix);
}

MutableStringView String::trimmed(const StringView characters) {
    return MutableStringView{*this}.trimmed(characters);
}

StringView String::trimmed(const StringView characters) const {
    return StringView{*this}.trimmed(characters);
}

MutableStringView String::trimmed() {
    return MutableStringView{*this}.trimmed();
}

StringView String::trimmed() const {
    return StringView{*this}.trimmed();
}

MutableStringView String::trimmedPrefix(const StringView characters) {
    return MutableStringView{*this}.trimmedPrefix(characters);
}

StringView String::trimmedPrefix(const StringView characters) const {
    return StringView{*this}.trimmedPrefix(characters);
}

MutableStringView String::trimmedPrefix() {
    return MutableStringView{*this}.trimmedPrefix();
}

StringView String::trimmedPrefix() const {
    return StringView{*this}.trimmedPrefix();
}

MutableStringView String::trimmedSuffix(const StringView characters) {
    return MutableStringView{*this}.trimmedSuffix(characters);
}

StringView String::trimmedSuffix(const StringView characters) const {
    return StringView{*this}.trimmedSuffix(characters);
}

MutableStringView String::trimmedSuffix() {
    return MutableStringView{*this}.trimmedSuffix();
}

StringView String::trimmedSuffix() const {
    return StringView{*this}.trimmedSuffix();
}

MutableStringView String::find(const StringView substring) {
    return MutableStringView{*this}.findOr(substring, nullptr);
}

StringView String::find(const StringView substring) const {
    return StringView{*this}.findOr(substring, nullptr);
}

MutableStringView String::find(const char character) {
    return MutableStringView{*this}.findOr(character, nullptr);
}

StringView String::find(const char character) const {
    return StringView{*this}.findOr(character, nullptr);
}

MutableStringView String::findOr(const StringView substring, char* const fail) {
    return MutableStringView{*this}.findOr(substring, fail);
}

StringView String::findOr(const StringView substring, const char* const fail) const {
    return StringView{*this}.findOr(substring, fail);
}

MutableStringView String::findOr(const char character, char* const fail) {
    return MutableStringView{*this}.findOr(character, fail);
}

StringView String::findOr(const char character, const char* const fail) const {
    return StringView{*this}.findOr(character, fail);
}

MutableStringView String::findLast(const StringView substring) {
    return MutableStringView{*this}.findLastOr(substring, nullptr);
}

StringView String::findLast(const StringView substring) const {
    return StringView{*this}.findLastOr(substring, nullptr);
}

MutableStringView String::findLast(const char character) {
    return MutableStringView{*this}.findLastOr(character, nullptr);
}

StringView String::findLast(const char character) const {
    return StringView{*this}.findLastOr(character, nullptr);
}

MutableStringView String::findLastOr(const StringView substring, char* const fail) {
    return MutableStringView{*this}.findLastOr(substring, fail);
}

StringView String::findLastOr(const StringView substring, const char* const fail) const {
    return StringView{*this}.findLastOr(substring, fail);
}

MutableStringView String::findLastOr(const char character, char* const fail) {
    return MutableStringView{*this}.findLastOr(character, fail);
}

StringView String::findLastOr(const char character, const char* const fail) const {
    return StringView{*this}.findLastOr(character, fail);
}

bool String::contains(const StringView substring) const {
    return StringView{*this}.contains(substring);
}

bool String::contains(const char character) const {
    return StringView{*this}.contains(character);
}

MutableStringView String::findAny(const StringView characters) {
    return MutableStringView{*this}.findAny(characters);
}

StringView String::findAny(const StringView characters) const {
    return StringView{*this}.findAny(characters);
}

MutableStringView String::findAnyOr(const StringView characters, char* fail) {
    return MutableStringView{*this}.findAnyOr(characters, fail);
}

StringView String::findAnyOr(const StringView characters, const char* fail) const {
    return StringView{*this}.findAnyOr(characters, fail);
}

MutableStringView String::findLastAny(const StringView characters) {
    return MutableStringView{*this}.findLastAny(characters);
}

StringView String::findLastAny(const StringView characters) const {
    return StringView{*this}.findLastAny(characters);
}

MutableStringView String::findLastAnyOr(const StringView characters, char* fail) {
    return MutableStringView{*this}.findLastAnyOr(characters, fail);
}

StringView String::findLastAnyOr(const StringView characters, const char* fail) const {
    return StringView{*this}.findLastAnyOr(characters, fail);
}

bool String::containsAny(const StringView substring) const {
    return StringView{*this}.containsAny(substring);
}

std::size_t String::count(const char character) const {
    return StringView{*this}.count(character);
}

char* String::release() {
    CORRADE_ASSERT(!(_small.size & Implementation::SmallStringBit),
        "Containers::String::release(): cannot call on a SSO instance", {});
    char* data = _large.data;

    _small.data[0] = '\0';
    _small.size = Implementation::SmallStringBit;
    return data;
}

}}
#endif
