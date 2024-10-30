# mulle-stacktrace

#### 👣 Stracktrace support for various OS

The idea here is to have a unified library for stacktraces.
This currently just uses `<execinfo.h>`. It will need more code to support
windows. This is not doing much at the moment and not doing it very well.




| Release Version                                       | Release Notes
|-------------------------------------------------------|--------------
| ![Mulle kybernetiK tag](https://img.shields.io/github/tag/mulle-core/mulle-stacktrace.svg?branch=release) [![Build Status](https://github.com/mulle-core/mulle-stacktrace/workflows/CI/badge.svg?branch=release)](//github.com/mulle-core/mulle-stacktrace/actions) | [RELEASENOTES](RELEASENOTES.md) |






### You are here

![Overview](overview.dot.svg)





## Add

**This project is a component of the [mulle-core](//github.com/mulle-core/mulle-core) library. As such you usually will *not* add or install it
individually, unless you specifically do not want to link against
`mulle-core`.**


### Add as an individual component

Use [mulle-sde](//github.com/mulle-sde) to add mulle-stacktrace to your project:

``` sh
mulle-sde add github:mulle-core/mulle-stacktrace
```

To only add the sources of mulle-stacktrace with dependency
sources use [clib](https://github.com/clibs/clib):


``` sh
clib install --out src/mulle-core mulle-core/mulle-stacktrace
```

Add `-isystem src/mulle-core` to your `CFLAGS` and compile all the sources that were downloaded with your project.


## Install

Use [mulle-sde](//github.com/mulle-sde) to build and install mulle-stacktrace and all dependencies:

``` sh
mulle-sde install --prefix /usr/local \
   https://github.com/mulle-core/mulle-stacktrace/archive/latest.tar.gz
```

### Legacy Installation

Install the requirements:

| Requirements                                 | Description
|----------------------------------------------|-----------------------
| [mulle-dlfcn](https://github.com/mulle-core/mulle-dlfcn)             | ♿️ Shared library helper

Download the latest [tar](https://github.com/mulle-core/mulle-stacktrace/archive/refs/tags/latest.tar.gz) or [zip](https://github.com/mulle-core/mulle-stacktrace/archive/refs/tags/latest.zip) archive and unpack it.

Install **mulle-stacktrace** into `/usr/local` with [cmake](https://cmake.org):

``` sh
cmake -B build \
      -DCMAKE_INSTALL_PREFIX=/usr/local \
      -DCMAKE_PREFIX_PATH=/usr/local \
      -DCMAKE_BUILD_TYPE=Release &&
cmake --build build --config Release &&
cmake --install build --config Release
```


## Author

[Nat!](https://mulle-kybernetik.com/weblog) for Mulle kybernetiK  



