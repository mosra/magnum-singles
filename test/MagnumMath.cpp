/*
    This file is part of Magnum.

    Copyright © 2010, 2011, 2012, 2013, 2014, 2015, 2016, 2017, 2018, 2019,
                2020, 2021, 2022, 2023, 2024, 2025
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

#define MAGNUM_MATH_IMPLEMENTATION
#include "../MagnumMath.hpp"
/* Including second time to verify the implementation (or other) symbols don't
   get accidentally defined twice. See comment in the input file for details
   how this can happen in practice. */
#include "../MagnumMath.hpp"

using namespace Magnum;

/*
    Does this look funny? That's DEFINITELY NOT how testing should look, right?
    True dat. The actual tests, with >99% code coverage, are done in the Magnum
    project itself, because there's much better tooling for that. Have a look:
    https://github.com/mosra/magnum/tree/master/src/Magnum/Math/Test
*/

int main() {
    Vector4i a{3, 42, 57, -1};

    /* The log2() call tests code from MAGNUM_MATH_IMPLEMENTATION */
    return a[1] - 40 - Math::log2(4);
}
