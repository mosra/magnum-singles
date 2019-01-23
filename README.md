This repository contains single-header libraries from the Magnum engine.

[![Join the chat at https://gitter.im/mosra/magnum](https://badges.gitter.im/Join%20Chat.svg)](https://gitter.im/mosra/magnum?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)
[![Build Status](https://travis-ci.org/mosra/magnum-singles.svg?branch=master)](https://travis-ci.org/mosra/magnum-singles)
[![MIT License](https://img.shields.io/badge/license-MIT-green.svg)](https://opensource.org/licenses/MIT)

-   Project homepage — https://magnum.graphics/
-   Documentation — https://doc.magnum.graphics/
-   GitHub project page — https://github.com/mosra/magnum-singles

UNDER CONSTRUCTION
==================

This repository is not yet deemed stable and the level of documentation is
*way* below Magnum standards. Please be aware the repository might still get
force-pushed-to until things stabilize.

There are the following single-header libraries at the moment. This list will
grow with more Magnum features being exposed this way.

Library         | LoC | Pp LoC <sup>(*)</sup> | Description
--------------- | --- | ---------------- | -------------------
**[CorradeOptional.h](CorradeOptional.h)** | 253 | 2715   | [Containers::Optional](https://doc.magnum.graphics/corrade/classCorrade_1_1Containers_1_1Optional.html), a lightweight alternative to [`std::optional`](https://en.cppreference.com/w/cpp/utility/optional)
**[CorradePointer.h](CorradePointer.h)** | 243 | 2315     | [Containers::Pointer](https://doc.magnum.graphics/corrade/classCorrade_1_1Containers_1_1Pointer.html), a lightweight alternative to [`std::unique_ptr`](https://en.cppreference.com/w/cpp/memory/unique_ptr)
**[CorradeReference.h](CorradeReference.h)** | 109 | 1639 | [Containers::Reference](https://doc.magnum.graphics/corrade/classCorrade_1_1Containers_1_1Reference.html), a lightweight alternative to [`std::reference_wrapper`](https://en.cppreference.com/w/cpp/utility/functional/reference_wrapper)

<sup>(*)</sup> — lines of code after a preprocessor run, with system includes
expanded. Gathered using GCC 8.2 and libstdc++.

For more information read the
[single-header library docs](https://doc.magnum.graphics/corrade/corrade-singles.html)
or the intro article on the project blog:
https://blog.magnum.graphics/backstage/lightweight-stl-compatible-unique-pointer/
The libraries are generated using `acme.py`, which is part of Corrade.
[See its documentation](https://doc.magnum.graphics/corrade/acme.html), if you
are interested.

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

LICENSE
=======

Magnum is licensed under the MIT/Expat license, see the [COPYING](COPYING) file
for details.
