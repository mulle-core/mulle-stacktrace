# mulle-stacktrace

👣 Stracktrace support for various OS

The idea here is to have a unified library for stacktraces.
This currently just uses `<execinfo.h>`. It will need more
code to support windows.


Build Status | Release Version
-------------|-----------------------------------
[![Build Status](https://travis-ci.org/mulle-core/mulle-stacktrace.svg?branch=release)](https://travis-ci.org/mulle-core/mulle-stacktrace) | ![Mulle kybernetiK tag](https://img.shields.io/github/tag/mulle-core/mulle-stacktrace.svg) [![Build Status](https://travis-ci.org/mulle-core/mulle-stacktrace.svg?branch=release)](https://travis-ci.org/mulle-core/mulle-stacktrace)


## Add 

Use [mulle-sde](//github.com/mulle-sde) to add mulle-stacktrace to your project:

```
mulle-sde dependency add --c --github mulle-core mulle-stacktrace
```

## Install

### mulle-sde

Use [mulle-sde](//github.com/mulle-sde) to build and install mulle-stacktrace and all dependencies:

```
mulle-sde install --prefix /usr/local \
   https://github.com/mulle-core/mulle-stacktrace/archive/latest.tar.gz
```

### Manual Installation


Install the requirements:

Requirements                                                 | Description
-------------------------------------------------------------|-----------------------
[mulle-dlfcn](//github.com/mulle-core/mulle-atexit)          | Cross-platform dlfcn support


Install into `/usr/local`:

```
mkdir build 2> /dev/null
(
   cd build ;
   cmake -DCMAKE_INSTALL_PREFIX=/usr/local \
         -DCMAKE_PREFIX_PATH=/usr/local \
         -DCMAKE_BUILD_TYPE=Release .. ;
   make install
)
```


## Author

[Nat!](//www.mulle-kybernetik.com/weblog) for
[Mulle kybernetiK](//www.mulle-kybernetik.com) and
[Codeon GmbH](//www.codeon.de)
