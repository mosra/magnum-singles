This repository contains single-header libraries from the Magnum engine.

[![Join the chat at https://gitter.im/mosra/magnum](https://badges.gitter.im/Join%20Chat.svg)](https://gitter.im/mosra/magnum?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)
[![Build Status](https://circleci.com/gh/mosra/magnum-singles.svg?style=shield)](https://circleci.com/gh/mosra/magnum-singles)
[![MIT License](https://img.shields.io/badge/license-MIT-green.svg)](https://opensource.org/licenses/MIT)

-   Project homepage — https://magnum.graphics/
-   Documentation — https://doc.magnum.graphics/
-   GitHub project page — https://github.com/mosra/magnum-singles

LIBRARIES
=========

There are the following single-header libraries at the moment. This list will
grow with more Magnum features being exposed this way.

Library         | LoC | PpLoC<sup>[1]</sup>     | Description
--------------- | --- | ----------------------- | -------------------
**[CorradeArrayView.h](CorradeArrayView.h)** | 848 | 2025 | [Containers::ArrayView](https://doc.magnum.graphics/corrade/classCorrade_1_1Containers_1_1ArrayView.html) and [Containers::StaticArrayView](https://doc.magnum.graphics/corrade/classCorrade_1_1Containers_1_1StaticArrayView.html), lightweight alternatives to [`std::span`](https://en.cppreference.com/w/cpp/container/span)
**[CorradeStridedArrayView.h](CorradeStridedArrayView.h)** | 1280<sup>[2]</sup> | 2855 | [Containers::StridedArrayView](https://doc.magnum.graphics/corrade/classCorrade_1_1Containers_1_1StridedArrayView.html), multi-dimensional strided array view. Depends on `CorradeArrayView.h`.
**[CorradeArray.h](CorradeArray.h)** | 890<sup>[2]</sup> | 2669 | [Containers::Array](https://doc.magnum.graphics/corrade/classCorrade_1_1Containers_1_1Array.html) and [Containers::StaticArray](https://doc.magnum.graphics/corrade/classCorrade_1_1Containers_1_1StaticArray.html), lightweight alternatives to [`std::vector`](https://en.cppreference.com/w/cpp/container/vector) and [`std::array`](https://en.cppreference.com/w/cpp/container/span). Depends on `CorradeArrayView.h`.
**[CorradeEnumSet.h](CorradeEnumSet.h)** | 260 | 1705   | [Containers::EnumSet](https://doc.magnum.graphics/corrade/classCorrade_1_1Containers_1_1EnumSet.html), a type-safe set of bits
**[CorradeOptional.h](CorradeOptional.h)** | 456 | 1879   | [Containers::Optional](https://doc.magnum.graphics/corrade/classCorrade_1_1Containers_1_1Optional.html), a lightweight alternative to [`std::optional`](https://en.cppreference.com/w/cpp/utility/optional)
**[CorradePair.h](CorradePair.h)** | 303 | 1723 | [Containers::Pair](https://doc.magnum.graphics/corrade/classCorrade_1_1Containers_1_1Pair.html), a lightweight alternative to [`std::pair`](https://en.cppreference.com/w/cpp/utility/pair)
**[CorradePointer.h](CorradePointer.h)** | 361 | 1773     | [Containers::Pointer](https://doc.magnum.graphics/corrade/classCorrade_1_1Containers_1_1Pointer.html), a lightweight alternative to [`std::unique_ptr`](https://en.cppreference.com/w/cpp/memory/unique_ptr)
**[CorradeReference.h](CorradeReference.h)** | 119 | 1627 | [Containers::Reference](https://doc.magnum.graphics/corrade/classCorrade_1_1Containers_1_1Reference.html), a lightweight alternative to [`std::reference_wrapper`](https://en.cppreference.com/w/cpp/utility/functional/reference_wrapper)
**[CorradeScopeGuard.h](CorradeScopeGuard.h)** | 248 | 1704 | [Containers::ScopeGuard](https://doc.magnum.graphics/corrade/classCorrade_1_1Containers_1_1ScopeGuard.html), a lightweight alternative to [`std::unique_ptr`](https://en.cppreference.com/w/cpp/memory/unique_ptr) with a custom deleter
**[CorradeString.hpp](CorradeString.hpp)** | 2258<sup>[2]</sup> | 2183 | [Containers::String](https://doc.magnum.graphics/corrade/classCorrade_1_1Containers_1_1String.html) and [Containers::StringView](https://doc.magnum.graphics/corrade/classCorrade_1_1Containers_1_1BasicStringView.html), lightweight and optimized string (view) classes. Depends on `CorradeEnumSet.h`, the implementation depends on `CorradePair.h` and `CorradeCpu.hpp`.
**[CorradeTriple.h](CorradeTriple.h)** | 337 | 1740 | [Containers::Triple](https://doc.magnum.graphics/corrade/classCorrade_1_1Containers_1_1Triple.html), a lightweight alternative to a three-component [`std::tuple`](https://en.cppreference.com/w/cpp/utility/tuple)
**[CorradeCpu.hpp](CorradeCpu.hpp)** | 1717 | 1993 | [Cpu](https://doc.magnum.graphics/corrade/namespaceCorrade_1_1Cpu.html) library, compile-time and runtime CPU feature detection and dispatch
**[CorradeStlForwardArray.h](CorradeStlForwardArray.h)** | 88 | 2003<sup>[3]</sup> | [Corrade's forward declaration for `std::array`](https://doc.magnum.graphics/corrade/StlForwardArray_8h.html), a lightweight alternative to the full [`<array>`](https://en.cppreference.com/w/cpp/header/array) (15k PpLOC<sup>[1]</sup>) where supported
**[CorradeStlForwardString.h](CorradeStlForwardString.h)** | 89 | 68 | [Corrade's forward declaration for `std::string`](https://doc.magnum.graphics/corrade/StlForwardString_8h.html), a lightweight alternative to the full [`<string>`](https://en.cppreference.com/w/cpp/header/string) (11k PpLOC<sup>[1]</sup>) where supported
**[CorradeStlForwardTuple.h](CorradeStlForwardTuple.h)** | 102 | 2228 | [Corrade's forward declaration for `std::tuple`](https://doc.magnum.graphics/corrade/StlForwardTuple_8h.html), a lightweight alternative to the full [`<tuple>`](https://en.cppreference.com/w/cpp/header/tuple) (13k PpLOC<sup>[1]</sup>) where supported
**[CorradeStlForwardVector.h](CorradeStlForwardVector.h)** | 77 | 481<sup>[3]</sup> | [Corrade's forward declaration for `std::vector`](https://doc.magnum.graphics/corrade/StlForwardVector_8h.html), a lightweight alternative to the full [`<vector>`](https://en.cppreference.com/w/cpp/header/tuple) (9k PpLOC<sup>[1]</sup>) where supported
**[CorradeStlMath.h](CorradeStlMath.h)** | 73 | 3222<sup>[4]</sup> | [Corrade's optimized `<cmath>`](https://doc.magnum.graphics/corrade/StlMath_8h.html), without the heavy C++17 additions (which is otherwise 11k PpLOC<sup>[4]</sup>)
**[MagnumMath.hpp](MagnumMath.hpp)** | 7950 | 10096       | [Math](https://doc.magnum.graphics/magnum/namespaceMagnum_1_1Math.html) namespace, a full-featured graphics-oriented linear algebra library

<sup>[1]</sup> — lines of code after a preprocessor run, with system includes
expanded. Gathered using GCC 13.2 and libstdc++, unless said otherwise.

<sup>[2]</sup> — not a total size due to inter-library dependencies

<sup>[3]</sup> — gathered using Clang 15.0 and libc++, since libstdc++ doesn't
have a forward declaration for `std::array` / `std::vector`

<sup>[4]</sup> — gathered using GCC 13.2, libstdc++ and `-std=c++17`

Where is the documentation?
---------------------------

Single-header libraries provided here are *generated* from multi-file sources
in the Magnum project. This is done for two reasons — first, documentation and
test coverage is much easier to maintain in the setting of a bigger project,
avoiding any redundancy or duplicated efforts. Second, because the resulting
files are generated with non-essential parts stripped away, there's no need to
worry about bloating them due to original implementations having extensive
documentation or rarely used features.

With the goal being easy integration, the files are deliberately free of all
comments and documentation blocks to keep their size small. Documentation for
each library is provided in the official Magnum documentation, linked from the
table above. Each library file contains the same documentation link, together
with a concrete Git revision it was generated from and a changelog for a few
versions back for easier overview when updating.

For more information read the [single-header library docs](https://doc.magnum.graphics/corrade/corrade-singles.html). The libraries are generated
using `acme.py`, which is a part of Corrade.
[See its documentation](https://doc.magnum.graphics/corrade/acme.html), if you
are interested. Particular libraries are introduced on the Magnum blog:

-   [Lightweight but still STL-compatible unique pointer](https://blog.magnum.graphics/backstage/lightweight-stl-compatible-unique-pointer/)
    (Jan 16, 2019)
-   [Array view implementations in Magnum](https://blog.magnum.graphics/backstage/array-view-implementations/)
    (Feb 18, 2019)
-   [Forward-declaring STL container types](https://blog.magnum.graphics/backstage/forward-declaring-stl-container-types/)
    (Mar 28, 2019)
-   [Multi-dimensional strided array views in Magnum](https://blog.magnum.graphics/backstage/multidimensional-strided-array-views/)
    (Apr 30, 2019)
-   [Convenient CPU feature detection and dispatch](https://blog.magnum.graphics/backstage/cpu-feature-detection-dispatch/)
    (Aug 2, 2022)

What about test coverage?
-------------------------

Testing done in this repository is mainly to ensure the libraries are generated
correctly. Extensive testing on variety of compilers and OSes with > 99% test
coverage is done in the Magnum Project itself. See the
[Build Status](https://magnum.graphics/build-status/) page for more
information.

Reporting bugs and contributing
-------------------------------

As always, bug reports, feature requests and code contributions are very
welcome. However again please note the files in this repository are generated
from original sources in the [corrade](https://github.com/mosra/corrade) and
[magnum](https://github.com/mosra/magnum) repositories, meaning that ideally
all PRs should go there instead, as there's a better infrastructure for
documentation and testing. We don't enforce this rule though — if you have an
important bugfix, it's better if you submit it here than not at all 😉

SUPPORTED PLATFORMS
===================

All libraries are tested on these platforms:

-   **Linux** and embedded Linux
-   **Windows**, **Windows RT** (Store/Phone)
-   **macOS**, **iOS**
-   **Android**
-   **Web** ([asm.js](http://asmjs.org/) or [WebAssembly](http://webassembly.org/)),
    through [Emscripten](http://kripken.github.io/emscripten-site/)

And on these compilers:

-   **GCC** 4.8.1 and newer (and equivalent MinGW-w64 version)
-   **Clang** 6.0 and newer (or AppleClang 10.0 and newer), both `libstdc++`
    and `libc++`
-   **MSVC** 2015 and newer

CONTACT & SUPPORT
=================

-   Project homepage — https://magnum.graphics/
-   Documentation — https://doc.magnum.graphics/
-   GitHub — https://github.com/mosra/magnum-singles and the
    [#magnum](https://github.com/topics/magnum) topic
-   GitLab — https://gitlab.com/mosra/magnum-singles
-   Gitter community chat — https://gitter.im/mosra/magnum
-   E-mail — info@magnum.graphics
-   Google Groups mailing list — magnum-engine@googlegroups.com
    ([archive](https://groups.google.com/forum/#!forum/magnum-engine))
-   Twitter — https://twitter.com/czmosra and the
    [#MagnumEngine](https://twitter.com/hashtag/MagnumEngine) hashtag

See also the Magnum Project [Contact & Support page](https://magnum.graphics/contact/)
for further information.

Credits
-------

Libraries presented here are a result of a dedicated work by many community
members. List of all contributors to the Magnum Project can be found
[in the documentation](https://doc.magnum.graphics/magnum/credits-contributors.html).

License
-------

Magnum is licensed under the MIT/Expat license, see the [COPYING](COPYING) file
for details.
