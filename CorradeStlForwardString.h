/*
    Corrade's forward declaration for std::string
        — a lightweight alternative to the full <string> where supported

    https://doc.magnum.graphics/corrade/StlForwardString_8h.html

    This is a single-header library generated from the Corrade project. With
    the goal being easy integration, it's deliberately free of all comments
    to keep the file size small. More info, detailed changelogs and docs here:

    -   Project homepage — https://magnum.graphics/corrade/
    -   Documentation — https://doc.magnum.graphics/corrade/
    -   GitHub project page — https://github.com/mosra/corrade
    -   GitHub Singles repository — https://github.com/mosra/magnum-singles

    v2019.01-115-ged348b26 (2019-03-27)
    -   Initial release

    Generated from Corrade v2020.06-0-g61d1b58c (2020-06-27), 74 / 48 LoC
*/

/*
    This file is part of Corrade.

    Copyright © 2007, 2008, 2009, 2010, 2011, 2012, 2013, 2014, 2015, 2016,
                2017, 2018, 2019, 2020 Vladimír Vondruš <mosra@centrum.cz>

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

#include <ciso646>

#ifdef _LIBCPP_VERSION
#define CORRADE_TARGET_LIBCXX
#elif defined(__GLIBCXX__)
#define CORRADE_TARGET_LIBSTDCXX
#elif defined(__has_include)
    #if __has_include(<bits/c++config.h>)
        #include <bits/c++config.h>
        #ifdef __GLIBCXX__
        #define CORRADE_TARGET_LIBSTDCXX
        #endif
    #endif
#else
#endif

#ifndef Corrade_Utility_StlForwardString_h
#define Corrade_Utility_StlForwardString_h

#ifdef CORRADE_TARGET_LIBCXX
#include <iosfwd>
#elif defined(CORRADE_TARGET_LIBSTDCXX)
#include <bits/stringfwd.h>
#else
#include <string>
#endif

#endif
