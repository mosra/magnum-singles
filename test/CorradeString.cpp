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

#define CORRADE_CPU_IMPLEMENTATION
#define CORRADE_STRING_IMPLEMENTATION
#include "../CorradeString.hpp"
/* Including second time to verify the implementation (or other) symbols don't
   get accidentally defined twice. See comment in the input file for details
   how this can happen in practice. */
#include "../CorradeString.hpp"

using namespace Corrade;
using namespace Corrade::Containers::Literals;

/*
    Does this look funny? That's DEFINITELY NOT how testing should look, right?
    True dat. The actual tests, with >99% code coverage, are done in the Magnum
    project itself, because there's much better tooling for that. Have a look:
    https://github.com/mosra/corrade/blob/master/src/Corrade/Containers/Test/StringTest.cpp
    https://github.com/mosra/corrade/blob/master/src/Corrade/Containers/Test/StringStlTest.cpp
    https://github.com/mosra/corrade/blob/master/src/Corrade/Containers/Test/StringStlViewTest.cpp
    https://github.com/mosra/corrade/blob/master/src/Corrade/Containers/Test/StringViewTest.cpp
    https://github.com/mosra/corrade/blob/master/src/Corrade/Containers/Test/StringViewBenchmark.cpp
*/

int main() {
    /* The character lookup delegates to an AVX/SSE, NEON or WASM variant. This
       also tests the code from both CORRADE_CPU_IMPLEMENTATION and
       CORRADE_STRING_IMPLEMENTATION. */
    return Containers::String{"hello"_s}.contains('l') ? 0 : 1;
}
