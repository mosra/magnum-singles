This repository contains single-header libraries from the Magnum engine.

[![Join the chat at https://gitter.im/mosra/magnum](https://badges.gitter.im/Join%20Chat.svg)](https://gitter.im/mosra/magnum?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)
[![Build Status](https://travis-ci.org/mosra/magnum-singles.svg?branch=master)](https://travis-ci.org/mosra/magnum-singles)
[![MIT License](https://img.shields.io/badge/license-MIT-green.svg)](https://opensource.org/licenses/MIT)

-   Project homepage — https://magnum.graphics/
-   Documentation — https://doc.magnum.graphics/
-   GitHub project page — https://github.com/mosra/magnum-singles

LIBRARIES
=========

There are the following single-header libraries at the moment. This list will
grow with more Magnum features being exposed this way.

Library         | LoC | PpLoC<sup>(*)</sup>     | Description
--------------- | --- | ----------------------- | -------------------
**[CorradeArrayView.h](CorradeArrayView.h)** | 558 | 2453   | [Containers::ArrayView](https://doc.magnum.graphics/corrade/classCorrade_1_1Containers_1_1ArrayView.html) and [Containers::StaticArrayView](https://doc.magnum.graphics/corrade/classCorrade_1_1Containers_1_1StaticArrayView.html), lightweight alternatives to [`std::span`](https://en.cppreference.com/w/cpp/container/span)
**[CorradeOptional.h](CorradeOptional.h)** | 328 | 2742   | [Containers::Optional](https://doc.magnum.graphics/corrade/classCorrade_1_1Containers_1_1Optional.html), a lightweight alternative to [`std::optional`](https://en.cppreference.com/w/cpp/utility/optional)
**[CorradePointer.h](CorradePointer.h)** | 259 | 2321     | [Containers::Pointer](https://doc.magnum.graphics/corrade/classCorrade_1_1Containers_1_1Pointer.html), a lightweight alternative to [`std::unique_ptr`](https://en.cppreference.com/w/cpp/memory/unique_ptr)
**[CorradeReference.h](CorradeReference.h)** | 115 | 1639 | [Containers::Reference](https://doc.magnum.graphics/corrade/classCorrade_1_1Containers_1_1Reference.html), a lightweight alternative to [`std::reference_wrapper`](https://en.cppreference.com/w/cpp/utility/functional/reference_wrapper)
**[CorradeScopeGuard.h](CorradeScopeGuard.h)** | 131 | 34 | [Containers::ScopeGuard](https://doc.magnum.graphics/corrade/classCorrade_1_1Containers_1_1ScopeGuard.html), a lightweight alternative to [`std::unique_ptr`](https://en.cppreference.com/w/cpp/memory/unique_ptr) with a custom deleter

<sup>(*)</sup> — lines of code after a preprocessor run, with system includes
expanded. Gathered using GCC 8.2 and libstdc++.

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
with a concrete Git revision it was generated from a changelog for a few
versions back for easier overview when updating.

For more information read the [single-header library docs](https://doc.magnum.graphics/corrade/corrade-singles.html). The libraries are generated
using `acme.py`, which is part of Corrade.
[See its documentation](https://doc.magnum.graphics/corrade/acme.html), if you
are interested. Particular libraries are introduced on the Magnum blog:

-   [Lightweight but still STL-compatible unique pointer](https://blog.magnum.graphics/backstage/lightweight-stl-compatible-unique-pointer/)

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
-   **Clang** 3.3 and newer (and equivalent AppleClang version), both
    `libstdc++` and `libc++`
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
