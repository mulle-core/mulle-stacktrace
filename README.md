# mulle-stacktrace

#### 👣 Stracktrace support for various OS

The idea here is to have a unified library for stacktraces.
It now defaults to [libbacktrace](https://github.com/ianlancetaylor/libbacktrace)`
as the actual stacktrace implemntation.




| Release Version                                       | Release Notes  | AI Documentation
|-------------------------------------------------------|----------------|---------------
| ![Mulle kybernetiK tag](https://img.shields.io/github/tag/mulle-core/mulle-stacktrace.svg) ![standard: C11](pix/c11.svg) ![apis: ansi](pix/apis-ansi.svg) [![Build Status](https://github.com/mulle-core/mulle-stacktrace/workflows/CI/badge.svg)](//github.com/mulle-core/mulle-stacktrace/actions) | [RELEASENOTES](RELEASENOTES.md) | [DeepWiki for mulle-stacktrace](https://deepwiki.com/mulle-core/mulle-stacktrace)




## Documentation & Guides

* [API Summary](asset/dox/api/toc)

## API

### Core Functions

**mulle_stacktrace_once( fp)** - Print current stacktrace to file pointer
**mulle_stacktrace( stacktrace, fp)** - Print with custom stacktrace configuration
**_mulle_stacktrace( stacktrace, offset, format, fp)** - Full control with format options

### Format Options

| Option                      | Description
|-----------------------------|--------------------------------
| `mulle_stacktrace_normal`   | Full stacktrace
| `mulle_stacktrace_trimmed`  | Remove system frames (default)
| `mulle_stacktrace_linefeed` | One frame per line
| `mulle_stacktrace_csv`      | CSV format

### Example

```c
#include <mulle-stacktrace/mulle-stacktrace.h>

void foo()
{
   // Simple usage - print trimmed stacktrace
   mulle_stacktrace_once( stdout);

   // Full control - print with linefeeds, skip 1 frame
   _mulle_stacktrace( NULL, 1, mulle_stacktrace_linefeed, stdout);
}
```


### You are here

![Overview](overview.dot.svg)





## Add

mulle-stacktrace is a component of the [mulle-core](//github.com/mulle-core/mulle-core) library. So in your code include the mulle-core umbrella header:

``` c
#include <mulle-core/mulle-core.h>
```

### Add mulle-core to a cmake project

Add mulle-core to your `CMakeLists.txt` with
[FetchContent](https://cmake.org/cmake/help/latest/module/FetchContent.html):

``` cmake
include( FetchContent)

set( MULLE__CORE_TAG "master" CACHE STRING "branch or tag to fetch mulle-core from")

FetchContent_Declare( MULLE__CORE
   GIT_REPOSITORY https://github.com/mulle-core/mulle-core.git
   GIT_TAG        "${MULLE__CORE_TAG}")
FetchContent_MakeAvailable( MULLE__CORE)

target_link_libraries( ${PROJECT_NAME} PRIVATE mulle-core)
```


### Add mulle-core to a mulle-sde project

``` sh
mulle-sde add github:mulle-core/mulle-core
```

### Embed mulle-stacktrace with clib

``` sh
clib install --out src mulle-core/mulle-stacktrace
```

Append `src` to your include path (e.g. add `-isystem src`  to your `CFLAGS`)
and compile all the sources that were downloaded.

## Install

Use [mulle-sde](//github.com/mulle-sde) to build and install mulle-stacktrace and all dependencies:

``` sh
mulle-sde install --prefix /usr/local \
   https://github.com/mulle-core/mulle-stacktrace/archive/latest.tar.gz
```

### Manual Installation

Install the requirements:

| Requirements                                 | Description
|----------------------------------------------|-----------------------
| [mulle-dlfcn](https://github.com/mulle-core/mulle-dlfcn)             | ♿️ Shared library helper
| [libbacktrace](https://github.com/mulle-core/libbacktrace)             | A C library that may be linked into a C/C++ program to produce symbolic backtraces

Download the latest [tar](https://github.com/mulle-core/mulle-stacktrace/archive/refs/tags/latest.tar.gz) or [zip](https://github.com/mulle-core/mulle-stacktrace/archive/refs/tags/latest.zip) archive and unpack it.

Install **mulle-stacktrace** into `/usr/local` with [cmake](https://cmake.org):

``` sh
PREFIX_DIR="/usr/local"
cmake -B build                               \
      -DMULLE_SDK_PATH="${PREFIX_DIR}"       \
      -DCMAKE_INSTALL_PREFIX="${PREFIX_DIR}" \
      -DCMAKE_PREFIX_PATH="${PREFIX_DIR}"    \
      -DCMAKE_BUILD_TYPE=Release &&
cmake --build build --config Release &&
cmake --install build --config Release
```


## Author

[Nat!](https://mulle-kybernetik.com/weblog) for Mulle kybernetiK  



