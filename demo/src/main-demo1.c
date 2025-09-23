#include "include-private.h"

#include <stdio.h>

#include "mulle-stacktrace.h"

#include "demo-version.h"


MULLE_C_NEVER_INLINE
static void  foo( char *s)
{
   _mulle_stacktrace( NULL, 1, mulle_stacktrace_linefeed, stdout);
}


int  main( int argc, char *argv[])
{
   foo( "VfL Bochum 1848");
   return( 0);
}
