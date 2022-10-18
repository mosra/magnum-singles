/*
    This file is part of Corrade.

    Copyright © 2007, 2008, 2009, 2010, 2011, 2012, 2013, 2014, 2015, 2016,
                2017, 2018, 2019, 2020, 2021, 2022
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

#include "../CorradeArrayView.h"

using namespace Corrade;

/*
    Does this look funny? That's DEFINITELY NOT how testing should look, right?
    True dat. The actual tests, with >99% code coverage, are done in the Magnum
    project itself, because there's much better tooling for that. Have a look:
    https://github.com/mosra/corrade/blob/master/src/Corrade/Containers/Test/ArrayViewTest.cpp
    https://github.com/mosra/corrade/blob/master/src/Corrade/Containers/Test/ArrayViewStlTest.cpp
    https://github.com/mosra/corrade/blob/master/src/Corrade/Containers/Test/ArrayViewStlSpanTest.cpp
    https://github.com/mosra/corrade/blob/master/src/Corrade/Containers/Test/StaticArrayViewTest.cpp
    https://github.com/mosra/corrade/blob/master/src/Corrade/Containers/Test/StaticArrayViewStlTest.cpp
    https://github.com/mosra/corrade/blob/master/src/Corrade/Containers/Test/StaticArrayViewStlSpanTest.cpp
*/

int main() {
    int data[]{1, 3, 42, 1337};

    auto a = Containers::arrayView(data);
    Containers::StaticArrayView<1, int> b = a.slice<1>(2);
    return *b - 42;
}
