# mulle-stacktrace

#### 👣 Stracktrace support for various OS

The idea here is to have a unified library for stacktraces.
This currently just uses `<execinfo.h>`. It will need more code to support
windows. This is not doing much at the moment and not doing it very well.


| Release Version
|-----------------------------------
| [![Build Status](https://github.com/mulle-core/mulle-stacktrace/workflows/CI/badge.svg?branch=release)](//github.com/mulle-core/mulle-stacktrace)  ![Mulle kybernetiK tag](https://img.shields.io/github/tag/mulle-core/mulle-stacktrace/workflows/CI/badge.svg?branch=release)

### You are here

![Overview](overview.dot.svg)


## Add

Use [mulle-sde](//github.com/mulle-sde) to add mulle-stacktrace to your project:

``` sh
mulle-sde dependency add --c --marks no-actual-link --github mulle-core mulle-stacktrace
```

## Install

### mulle-sde

Use [mulle-sde](//github.com/mulle-sde) to build and install mulle-stacktrace and all dependencies:

``` sh
mulle-sde install --prefix /usr/local \
   https://github.com/mulle-core/mulle-stacktrace/archive/latest.tar.gz
```

### Manual Installation


Install the requirements:

Requirements                                                 | Description
-------------------------------------------------------------|-----------------------
[mulle-dlfcn](//github.com/mulle-core/mulle-atexit)          | Cross-platform dlfcn support


Install into `/usr/local`:

``` sh
cmake -B build \
      -DCMAKE_INSTALL_PREFIX=/usr/local \
      -DCMAKE_PREFIX_PATH=/usr/local \
      -DCMAKE_BUILD_TYPE=Release &&
cmake --build build --config Release &&
cmake --install build --config Release
```


## Author

[Nat!](//www.mulle-kybernetik.com/weblog) for
[Mulle kybernetiK](//www.mulle-kybernetik.com) and
[Codeon GmbH](//www.codeon.de)
