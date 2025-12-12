//  mulle-stacktrace-libbacktrace.c
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

#include "mulle-stacktrace.h"

#ifdef HAVE_LIB_LIBBACKTRACE

#include <backtrace.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Trim leading address or path from symbol string
static char *   _trim_belly_fat( char *s)
{
   char *sym;

   sym = strchr(s, ' ');
   if (sym)
   {
      while (*sym == ' ')
         ++sym;
      if (*sym)
         return(sym);
      return(s);
   }
   return(s);
}

// Trim trailing offset or address from symbol string
static int   _trim_arse_fat( char *s)
{
   char *offset;
   int  len;

   offset = strstr(s, " + ");
   if (offset)
      return((int)(offset - s));
   len = (int)strlen(s);
   return(len);
}

// Filter out uninteresting stack frames
static int   _trim_boring_functions(char *s, int size)
{
   int len;

   if (size == -1)
   {
      len = (int)strlen(s);
      size = len;
   }

   if (size == 3 && !strncmp(s, "0x0", 3))
      return(1);

#define stracktrace_has_prefix(s, prefix) (! strncmp(s, prefix, strlen( prefix)))

//   if (stracktrace_has_prefix(s, "test_calloc_or_raise"))
//      return(1);
//   if (stracktrace_has_prefix(s, "test_realloc_or_raise"))
//      return(1);
   if (stracktrace_has_prefix(s, "test_realloc"))
      return(1);
   if (stracktrace_has_prefix(s, "test_calloc"))
      return(1);
   if (stracktrace_has_prefix(s, "test_free"))
      return(1);
   if (stracktrace_has_prefix(s, "libmulle-testallocator"))
      return(1);
   if (stracktrace_has_prefix(s, "mulle_objc"))
      return(1);
   if (stracktrace_has_prefix(s, "_mulle_objc"))
      return(1);

   return(0);
}

// Default symbolize function (placeholder)
static char *   _symbolize_nothing( void *address,
                                    size_t max,
                                    char *buf,
                                    size_t len,
                                    void **userinfo)
{
   MULLE_C_UNUSED(address);
   MULLE_C_UNUSED(max);
   MULLE_C_UNUSED(buf);
   MULLE_C_UNUSED(len);
   MULLE_C_UNUSED(userinfo);
   return(NULL);
}

// Keep full symbol string (no trimming)
static char   *_keep_belly_fat( char *s)
{
   return( s);
}

// Keep full string length
static int   _keep_arse_fat( char *s)
{
   int   len;

   len = (int)strlen(s);
   return( len);
}

// Keep all functions (no filtering)
static int   keep_boring_functions( char *s, int size)
{
   MULLE_C_UNUSED(s);
   MULLE_C_UNUSED(size);
   return(0);
}

// Global backtrace state (initialized once)
static struct backtrace_state *global_state;

// Error callback for libbacktrace
static void   error_callback( void *data, const char *msg, int errnum)
{
   MULLE_C_UNUSED(data);
   fprintf(stderr, "libbacktrace error: %s (errno: %d)\n", msg, errnum);
}

// Structure to hold stacktrace context for callbacks
struct stacktrace_context
{
   struct mulle_stacktrace *stacktrace;
   FILE                    *fp;
   char                    *delim;
   char                    *delimchar;
   enum mulle_stacktrace_format format;
   int                     frame_count;
   int                     offset;
   int                     frames_processed;
};


// Callback for normal format
static int   normal_callback( void *data,
                              uintptr_t pc,
                              const char *filename,
                              int lineno,
                              const char *function)
{
   struct stacktrace_context   *ctx;
   char                        buf[512];
   char                        *s;
   int                         size;
   char                        user_buf[512];
   void                        *userinfo;

   ctx = (struct stacktrace_context *)data;
   if (ctx->frames_processed < ctx->offset)
   {
      ctx->frames_processed++;
      return(0);
   }

   if (function || filename)
   {
      snprintf(buf, sizeof(buf), "0x%lx %s at %s:%d",
               (unsigned long)pc,
               function ? function : "(unknown function)",
               filename ? filename : "(unknown file)",
               lineno);
   }
   else
   {
      snprintf(buf, sizeof(buf), "0x%lx (no symbol information)", (unsigned long)pc);
   }

   userinfo = NULL;
   s = ctx->stacktrace->symbolize((void *)pc, 0, user_buf, sizeof(user_buf), &userinfo);
   if (s && strcmp(s, function ? function : ""))
   {
      snprintf(buf, sizeof(buf), "0x%lx %s", (unsigned long)pc, s);
   }

   if (ctx->stacktrace->is_boring(s ? s : buf, -1))
   {
      ctx->frames_processed++;
      return(0);
   }

   s = ctx->stacktrace->trim_belly_fat(s ? s : buf);
   size = ctx->stacktrace->trim_arse_fat(s);
   fprintf(ctx->fp, "%s%.*s", ctx->delim, size, s);

   ctx->delim = ctx->delimchar;
   ctx->frame_count++;
   ctx->frames_processed++;
   return(0);
}

// Callback for trimmed format (same as normal, for consistency with original)
static int   trimmed_callback( void *data,
                               uintptr_t pc,
                               const char *filename,
                               int lineno,
                               const char *function)
{
   return(normal_callback(data, pc, filename, lineno, function));
}

// Callback for linefeed format
static int   linefeed_callback( void *data,
                                uintptr_t pc,
                                const char *filename,
                                int lineno,
                                const char *function)
{
   struct stacktrace_context *ctx;
   char                      buf[512];
   char                      *s;
   char                      user_buf[512];
   void                      *userinfo;

   ctx = (struct stacktrace_context *)data;
   if (ctx->frames_processed < ctx->offset)
   {
      ctx->frames_processed++;
      return(0);
   }

   if (function || filename)
   {
      snprintf(buf, sizeof(buf), "0x%lx %s at %s:%d",
               (unsigned long)pc,
               function ? function : "(unknown function)",
               filename ? filename : "(unknown file)",
               lineno);
   }
   else
   {
      snprintf(buf, sizeof(buf), "0x%lx (no symbol information)", (unsigned long)pc);
   }

   userinfo = NULL;
   s = ctx->stacktrace->symbolize((void *)pc, 0, user_buf, sizeof(user_buf), &userinfo);
   if (s && strcmp(s, function ? function : ""))
   {
      snprintf(buf, sizeof(buf), "0x%lx %s", (unsigned long)pc, s);
   }

   if (ctx->stacktrace->is_boring(s ? s : buf, -1))
   {
      ctx->frames_processed++;
      return(0);
   }

   fprintf(ctx->fp, "%s%s", ctx->delim, s ? s : buf);
   ctx->delim = ctx->delimchar;
   ctx->frame_count++;
   ctx->frames_processed++;
   return(0);
}

// Callback for CSV format
static int   csv_callback( void *data,
                           uintptr_t pc,
                           const char *filename,
                           int lineno,
                           const char *function)
{
   struct stacktrace_context *ctx;
   char                      buf[512];
   char                      *s;
   char                      user_buf[512];
   void                      *userinfo;

   ctx = (struct stacktrace_context *)data;
   if (ctx->frames_processed < ctx->offset)
   {
      ctx->frames_processed++;
      return(0);
   }

   if( function || filename)
   {
      snprintf(buf, sizeof(buf), "0x%lx %s at %s:%d",
               (unsigned long)pc,
               function ? function : "(unknown function)",
               filename ? filename : "(unknown file)",
               lineno);
   }
   else
   {
      snprintf(buf, sizeof(buf), "0x%lx (no symbol information)", (unsigned long)pc);
   }

   userinfo = NULL;
   s = ctx->stacktrace->symbolize((void *)pc, 0, user_buf, sizeof(user_buf), &userinfo);
   if (s && strcmp(s, function ? function : ""))
   {
      snprintf(buf, sizeof(buf), "0x%lx %s", (unsigned long)pc, s);
   }

   if (ctx->stacktrace->is_boring(s ? s : buf, -1))
   {
      ctx->frames_processed++;
      return(0);
   }

   fprintf(ctx->fp, "%p,,,%s,,%s\n",
           (void *)pc,
           function ? function : "",
           filename ? filename : "");
   ctx->delim = ctx->delimchar;
   ctx->frame_count++;
   ctx->frames_processed++;
   return(0);
}



static struct mulle_stacktrace   dummy =
{
   .symbolize      = _symbolize_nothing,
   .trim_belly_fat = _trim_belly_fat,
   .trim_arse_fat  = _trim_arse_fat,
   .is_boring      = _trim_boring_functions,
   .backend        = "backtrace"
};

void   _mulle_stacktrace( struct mulle_stacktrace      *stacktrace,
                          int                          offset,
                          enum mulle_stacktrace_format format,
                          FILE                         *fp)
{
   char                        *delimchar;
   struct stacktrace_context   ctx;
   int                         (*callback)(void *, uintptr_t, const char *, int, const char *);

   if( ! stacktrace)
      stacktrace = &dummy;
   if( ! fp)
      fp = stderr;

   switch (format)
   {
   case mulle_stacktrace_normal:
   case mulle_stacktrace_trimmed:
      fprintf(fp, " : [");
      delimchar = " |";
      callback  = format == mulle_stacktrace_normal ? normal_callback : trimmed_callback;
      break;
   case mulle_stacktrace_linefeed:
      delimchar = "\n";
      callback  = linefeed_callback;
      break;
   default: // mulle_stacktrace_csv
      fprintf(fp, "address,segment_offset,symbol_offset,symbol_address,symbol_name,segment_address,segment_name\n");
      delimchar = "\n";
      callback  = csv_callback;
      break;
   }

   if( ! global_state)
   {
      global_state = backtrace_create_state(NULL, 1, error_callback, NULL);
      if(! global_state)
      {
         fprintf(stderr, "Failed to create backtrace state\n");
         return;
      }
   }

   ctx.stacktrace      = stacktrace;
   ctx.fp              = fp;
   ctx.delim           = "";
   ctx.delimchar       = delimchar;
   ctx.format          = format;
   ctx.frame_count     = 0;
   ctx.offset          = offset;
   ctx.frames_processed = 0;

   backtrace_full(global_state, 0, callback, error_callback, &ctx);

   switch (format)
   {
   case mulle_stacktrace_normal:
   case mulle_stacktrace_trimmed:
      fputc(']', fp);
      break;
   case mulle_stacktrace_linefeed:
      fputc('\n', fp);
      break;
   default:
      break;
   }
}

int   mulle_stacktrace_count_frames(void)
{
   struct stacktrace_context ctx;
   int                      frame_count;

   if (!global_state)
   {
      global_state = backtrace_create_state(NULL, 1, error_callback, NULL);
      if (!global_state)
         return(0);
   }

   ctx.frame_count      = 0;
   ctx.frames_processed = 0;

   backtrace_full( global_state, 0, normal_callback, error_callback, &ctx);
   frame_count = ctx.frame_count;
   return(frame_count);
}

void   _mulle_stacktrace_init_default( struct mulle_stacktrace *stacktrace)
{
   stacktrace->symbolize       = _symbolize_nothing;
   stacktrace->trim_belly_fat  = _trim_belly_fat;
   stacktrace->trim_arse_fat   = _trim_arse_fat;
   stacktrace->is_boring       = _trim_boring_functions;
   stacktrace->backend         = "backtrace";
}

void   _mulle_stacktrace_init( struct mulle_stacktrace *stacktrace,
                               mulle_stacktrace_symbolizer_t *p_symbolize,
                               char *(*p_trim_belly_fat)(char *),
                               int (*p_trim_arse_fat)(char *),
                               int (*p_is_boring)(char *, int size))
{
   stacktrace->symbolize       = p_symbolize ? p_symbolize : _symbolize_nothing;
   stacktrace->trim_belly_fat  = p_trim_belly_fat ? p_trim_belly_fat : _keep_belly_fat;
   stacktrace->trim_arse_fat   = p_trim_arse_fat ? p_trim_arse_fat : _keep_arse_fat;
   stacktrace->is_boring       = p_is_boring ? p_is_boring : keep_boring_functions;
   stacktrace->backend         = "backtrace";
}

#endif
