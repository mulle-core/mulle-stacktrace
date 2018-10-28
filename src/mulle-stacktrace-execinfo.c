//
//  mulle_testallocator.c
//  mulle-container
//
//  Created by Nat! on 04.11.15.
//  Copyright (c) 2015 Nat! - Mulle kybernetiK.
//  Copyright (c) 2015 Codeon GmbH.
//  All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are met:
//
//  Redistributions of source code must retain the above copyright notice, this
//  list of conditions and the following disclaimer.
//
//  Redistributions in binary form must reproduce the above copyright notice,
//  this list of conditions and the following disclaimer in the documentation
//  and/or other materials provided with the distribution.
//
//  Neither the name of Mulle kybernetiK nor the names of its contributors
//  may be used to endorse or promote products derived from this software
//  without specific prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
//  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
//  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
//  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
//  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
//  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
//  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
//  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
//  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
//  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
//  POSSIBILITY OF SUCH DAMAGE.
//
#include "mulle-stacktrace.h"

#include <assert.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <execinfo.h>

//
// it gives 0   libMulleStandaloneObjC.dylib        0x000000010ab7e46d stacktrace + 61
// move past the address
static char   *trim_belly_fat( char *s)
{
   char   *hex;
   char   *sym;

   hex = strstr( s, " 0x");
   if( ! hex)
      return( s);
   hex++;

   sym = strchr( hex, ' ');
   if( ! sym)
      return( hex);

   while( *sym == ' ')
      ++sym;
   if( ! *sym)
      return( hex);

   return( sym);
}


static int   trim_arse_fat( char *s)
{
   char   *offset;

   offset = strstr( s, " + ");
   if( ! offset)
      return( (int) strlen( s));

   return( (int) (offset - s));
}


static char   *trim_no_belly_fat( char *s)
{
   return( s);
}


static int   trim_no_arse_fat( char *s)
{
   return( (int) strlen( s));
}


static int  boring( char *s, int size)
{
   if( size == 3 && ! strncmp( s, "0x0", 3))
      return( 1);

   if( ! strncmp( s, "test_calloc_or_raise", strlen( "test_calloc_or_raise")))
      return( 1);
   if( ! strncmp( s, "test_realloc_or_raise", strlen( "test_realloc_or_raise")))
      return( 1);
   if( ! strncmp( s, "test_realloc", strlen( "test_realloc")))
      return( 1);
   if( ! strncmp( s, "test_calloc", strlen( "test_calloc")))
      return( 1);
   if( ! strncmp( s, "test_free", strlen( "test_free")))
      return( 1);

   return( 0);
}

static int  is_not_boring( char *s, int size)
{
   return( 0);
}


void   _mulle_stacktrace( struct _mulle_stacktrace *stacktrace, int offset, int tracelevel, FILE *fp)
{
   static struct _mulle_stacktrace   dummy =
   {
      trim_no_belly_fat,
      trim_no_arse_fat,
      is_not_boring
   };

   if( ! stacktrace)
      stacktrace = &dummy;
   if( ! fp)
      fp = stderr;

   fprintf( fp, " : [");
   {
      void   *callstack[256];
      int    frames;
      char   **strs;
      char   **p;
      char   **sentinel;
      char   *delim;
      char   *s;
      int    size;

      frames   = backtrace( callstack, 256);
      strs     = backtrace_symbols( callstack, frames);
      p        = &strs[ frames];
      sentinel = &strs[ offset];

      delim = "";
      while( p > sentinel)
      {
         s = *--p;
         if( tracelevel > 2)
         {
            fprintf( fp, "%s%s", delim, s);
            delim = "|";
         }
         else
         {
            s    = stacktrace->trim_belly_fat( s);
            size = stacktrace->trim_arse_fat( s);
            if( ! stacktrace->is_boring( s, size))
            {
               fprintf( fp, "%s%.*s", delim, size, s);
               delim = "|";
            }
         }
      }
      free( strs);
   }
   fputc( ']', fp);
}


void   _mulle_stacktrace_init( struct _mulle_stacktrace *stacktrace,
                               char *(*trim_belly_fat)( char *),
                               int (*trim_arse_fat)( char *),
                               int (*is_boring)( char *, int size))
{
   stacktrace->trim_belly_fat = trim_belly_fat ? trim_belly_fat : trim_no_belly_fat;
   stacktrace->trim_arse_fat  = trim_arse_fat ? trim_arse_fat : trim_no_arse_fat;
   stacktrace->is_boring      = is_boring ? is_boring : is_not_boring;
}
