//
//  mulle-stacktrace-nop.c
//  mulle-core
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
// Fallback for platforms without libbacktrace or execinfo

#include "include-private.h"

#include "mulle-stacktrace.h"

#if MULLE_STRACKTRACE_BACKEND == MULLE_STRACKTRACE_BACKEND_NONE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


void   _mulle_stacktrace_init( struct mulle_stacktrace *stacktrace,
                               mulle_stacktrace_symbolizer_t *symbolize,
                               char *(*trim_belly_fat)( char *),
                               int (*trim_arse_fat)( char *),
                               int (*is_boring)( char *, int size))
{
   MULLE_C_UNUSED( stacktrace);
   MULLE_C_UNUSED( symbolize);
   MULLE_C_UNUSED( trim_belly_fat);
   MULLE_C_UNUSED( trim_arse_fat);
   MULLE_C_UNUSED( is_boring);
   stacktrace->backend = "nop";
}


void  _mulle_stacktrace( struct mulle_stacktrace *stacktrace,
                         int offset,
                         enum mulle_stacktrace_format format,
                         FILE *fp)
{
   MULLE_C_UNUSED( stacktrace);
   MULLE_C_UNUSED( offset);
   MULLE_C_UNUSED( format);
   MULLE_C_UNUSED( fp);
}


void   _mulle_stacktrace_init_default( struct mulle_stacktrace *stacktrace)
{
   MULLE_C_UNUSED( stacktrace);
   _mulle_stacktrace_init( stacktrace, 0, 0, 0, 0);
}


int   mulle_stacktrace_count_frames( void)
{
   return( 0);
}

#endif  // ! defined( HAVE_LIB_LIBBACKTRACE) && ! (__APPLE__ || __linux__ || BSD)