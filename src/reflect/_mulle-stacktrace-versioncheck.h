/*
 *   This file will be regenerated by `mulle-project-versioncheck`.
 *   Any edits will be lost.
 */
#ifndef mulle_stacktrace_versioncheck_h__
#define mulle_stacktrace_versioncheck_h__

#if defined( MULLE__DLFCN_VERSION)
# ifndef MULLE__DLFCN_VERSION_MIN
#  define MULLE__DLFCN_VERSION_MIN  ((0UL << 20) | (0 << 8) | 15)
# endif
# ifndef MULLE__DLFCN_VERSION_MAX
#  define MULLE__DLFCN_VERSION_MAX  ((0UL << 20) | (1 << 8) | 0)
# endif
# if MULLE__DLFCN_VERSION < MULLE__DLFCN_VERSION_MIN || MULLE__DLFCN_VERSION >= MULLE__DLFCN_VERSION_MAX
#  pragma message("MULLE__DLFCN_VERSION     is " MULLE_C_STRINGIFY_MACRO( MULLE__DLFCN_VERSION))
#  pragma message("MULLE__DLFCN_VERSION_MIN is " MULLE_C_STRINGIFY_MACRO( MULLE__DLFCN_VERSION_MIN))
#  pragma message("MULLE__DLFCN_VERSION_MAX is " MULLE_C_STRINGIFY_MACRO( MULLE__DLFCN_VERSION_MAX))
#  if MULLE__DLFCN_VERSION < MULLE__DLFCN_VERSION_MIN
#   error "mulle-dlfcn is too old"
#  else
#   error "mulle-dlfcn is too new"
#  endif
# endif
#endif

#endif
