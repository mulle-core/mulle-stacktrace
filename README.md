# mulle-stacktrace

👣 Stracktrace support for various OS

The idea here is to have a unified library for stacktraces.
This currently just uses `<execinfo.h>`. It will need more
code to support windows.


Build Status | Release Version
-------------|-----------------------------------
[![Build Status](https://travis-ci.org/mulle-core/mulle-stacktrace.svg?branch=release)](https://travis-ci.org/mulle-core/mulle-stacktrace) | ![Mulle kybernetiK tag](https://img.shields.io/github/tag/mulle-core/mulle-stacktrace.svg) [![Build Status](https://travis-ci.org/mulle-core/mulle-stacktrace.svg?branch=release)](https://travis-ci.org/mulle-core/mulle-stacktrace)


## Install

Build and install with:

```
mkdir build 2> /dev/null
(
   cd build ;
   cmake .. ;
   make install
)
```

Or let [mulle-sde](//github.com/mulle-sde) do it all for you.


## Author

[Nat!](//www.mulle-kybernetik.com/weblog) for
[Mulle kybernetiK](//www.mulle-kybernetik.com) and
[Codeon GmbH](//www.codeon.de)
