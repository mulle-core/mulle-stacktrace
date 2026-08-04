# mulle-stacktrace Library Documentation for AI
<!-- Keywords: stacktrace, backtrace, symbolizer, debugging, libbacktrace, execinfo -->

## 1. Introduction & Purpose

- A compact C library to capture and print stack traces (frames + symbols) across platforms.
- Solves unified stacktrace collection and formatting for debugging and logging; defaults to libbacktrace when available and falls back to execinfo on POSIX systems.
- Key features: simple one-shot printing, configurable stacktrace instance (symbolizer callbacks, trimming), multiple output formats (normal, trimmed, linefeed, CSV).
- Component of mulle-core; depends on mulle-dlfcn and libbacktrace (optional).

## 2. Key Concepts & Design Philosophy

- Minimal, backend-abstracted API: callers either use a default behavior (NULL) or provide a configured struct mulle_stacktrace.
- Symbolization is pluggable via a symboleizer callback type; trimming/filtering of frames is provided via callback hooks.
- Formatting is separated from capture via an enum (format choices) so callers can request different textual representations without changing capture logic.
- Lightweight: designed for library embedding and easy use in error/report paths.

## 3. Core API & Data Structures

API surface is in src/mulle-stacktrace.h (public header).

### 3.1. [mulle-stacktrace.h]

typedef mulle_stacktrace_symbolizer_t
- Purpose: function signature for symbolizing an address into a string.
- Signature: char *(fn)(void *address, size_t max, char *buf, size_t len, void **userinfo)

struct mulle_stacktrace
- Purpose: runtime configuration for stacktrace capture & formatting.
- Key fields:
   - symbolize: pointer to symbolizer function (mulle_stacktrace_symbolizer_t *)
   - trim_belly_fat: char *(*)(char *)  — optional string trimmer
   - trim_arse_fat: int (*)(char *)      — optional in-place trimmer/normalizer
   - is_boring: int (*)(char *s, int size) — predicate to hide "boring" frames
   - backend: char *                     — textual backend identifier (borrowed)
- Lifecycle:
   - _mulle_stacktrace_init( struct mulle_stacktrace *stacktrace, mulle_stacktrace_symbolizer_t *symbolize, char *(*trim_belly_fat)(char *), int (*trim_arse_fat)(char *), int (*is_boring)(char *, int))
     - initialize instance with callbacks
   - _mulle_stacktrace_init_default( struct mulle_stacktrace *stacktrace)
     - initialize a default instance (used internally when NULL is passed)
- Core operations:
   - _mulle_stacktrace(struct mulle_stacktrace *stacktrace, int offset, enum mulle_stacktrace_format format, FILE *fp)
     - capture & print a stacktrace; offset skips frames (e.g., caller)
   - mulle_stacktrace(struct mulle_stacktrace *stacktrace, FILE *fp)
     - inline helper: calls _mulle_stacktrace(..., offset=1, format=trimmed)
   - mulle_stacktrace_once(FILE *fp)
     - convenience one-shot: use defaults, print trimmed stacktrace to fp
- Inspection / helpers:
   - mulle_stacktrace_get_backend(struct mulle_stacktrace *stacktrace)
     - returns backend string (guaranteed non-NULL; may init a dummy if NULL passed)
   - mulle_stacktrace_count_frames(void)
     - returns number of frames captured by the backend (implementation detail)
   - mulle_stacktrace_symbolize_nothing(...) 
     - a provided symbolizer that produces minimal output (fallback)

enum mulle_stacktrace_format
- mulle_stacktrace_normal   : full stacktrace
- mulle_stacktrace_trimmed  : remove system frames (default)
- mulle_stacktrace_linefeed : one frame per line
- mulle_stacktrace_csv      : CSV output

Other exported helpers:
- Version helpers: mulle_stacktrace_get_version(), and inline getters for major/minor/patch.

## 4. Performance Characteristics

- Capture and symbolization cost proportional to number of frames O(n) where n is frames captured.
- Symbolization might be expensive (name lookup, I/O); overall time dominated by backend symbol resolution.
- Memory: minimal stack allocations; symbolizer may write into provided buffer (caller-provided stack memory).
- Thread-safety: not universally guaranteed; thread-safety depends on chosen backend (libbacktrace, execinfo) and any global symbolization state. Assume "backend-dependent; do not rely on implicit locking" unless backend docs state otherwise.

## 5. AI Usage Recommendations & Patterns

- Best practices:
   - For one-off logging use mulle_stacktrace_once(stdout) or mulle_stacktrace_once(stderr).
   - For reusable config, call _mulle_stacktrace_init_default(&s) or _mulle_stacktrace_init(...) with custom symbolizer/trimmers and then call _mulle_stacktrace(&s, offset, format, fp).
   - Treat backend and strings returned by API as borrowed; do not free them.
   - Use offset=1 to skip the stacktrace internals (common pattern).
- Common pitfalls:
   - Passing non-thread-safe symbolizers without synchronization.
   - Freeing/owning pointers returned from symbolizer or backend strings—these are typically borrowed.
- Idiomatic "mulle-sde" pattern:
   - Add as a component via mulle-sde add or clib install; rely on mulle-sde/mulle-core build rules for correct backend linking.

## 6. Integration Examples

### Example 1: Simple one-shot print

```c
#include <stdio.h>
#include <mulle-stacktrace/mulle-stacktrace.h>

void
foo()
{
   // print a trimmed stacktrace to stdout
   mulle_stacktrace_once( stdout);
}

int
main()
{
   foo();
   return( 0);
}
```

### Example 2: Custom configured stacktrace with format and offset

```c
#include <stdio.h>
#include <mulle-stacktrace/mulle-stacktrace.h>

int
main()
{
   struct mulle_stacktrace   st;

   // initialize default configuration (symbolizer etc.)
   _mulle_stacktrace_init_default( &st);

   // print full stacktrace (skip 1 frame to omit this wrapper)
   _mulle_stacktrace( &st, 1, mulle_stacktrace_normal, stdout);

   return( 0);
}
```

## 7. Dependencies

- mulle-core/mulle-dlfcn (runtime shared-library helpers)
- mulle-core/libbacktrace (preferred symbolization backend; optional at build time)

## 8. Shortcut

- This TOC was generated from README.md, src/mulle-stacktrace.h, and clib.json (see asset/dox/TOC.md in repository).  If a previous TOC exists, consider comparing changes since its last commit to focus updates.
