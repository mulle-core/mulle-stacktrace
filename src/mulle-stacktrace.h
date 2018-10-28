#include "include.h"

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

/*
 *  (c) 2018 nat
 *
 *  version:  major, minor, patch
 */
#define MULLE_STACKTRACE_VERSION  ((0 << 20) | (0 << 8) | 1)


static inline unsigned int   mulle_stacktrace_get_version_major( void)
{
   return( MULLE_STACKTRACE_VERSION >> 20);
}


static inline unsigned int   mulle_stacktrace_get_version_minor( void)
{
   return( (MULLE_STACKTRACE_VERSION >> 8) & 0xFFF);
}


static inline unsigned int   mulle_stacktrace_get_version_patch( void)
{
   return( MULLE_STACKTRACE_VERSION & 0xFF);
}


extern uint32_t   mulle_stacktrace_get_version( void);

/*
   Add other library headers here like so, for exposure to library
   consumers.

   # include "foo.h"
*/
struct _mulle_stacktrace
{
   char  *(*trim_belly_fat)( char *s);
   int   (*trim_arse_fat)( char *s);
   int   (*is_boring)( char *s, int size);
};

void   _mulle_stacktrace_init( struct _mulle_stacktrace *stacktrace,
                               char *(*trim_belly_fat)( char *),
                               int (*trim_arse_fat)( char *),
                               int (*is_boring)( char *, int size));

// stacktrace may be NULL
void  _mulle_stacktrace( struct _mulle_stacktrace *stacktrace, int offset, int tracelevel, FILE *fp);

static inline void  mulle_stacktrace( struct _mulle_stacktrace *stacktrace, FILE *fp)
{
   _mulle_stacktrace( stacktrace, 2, 0, fp);
}
