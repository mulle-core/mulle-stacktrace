//
//  mulle-stacktrace.c
//  mulle-core
//
//  Created by Nat! on 28.10.18
//  Copyright (c) 2018 Nat! - Mulle kybernetiK.
//  Copyright (c) 2018 Codeon GmbH.
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
#include "include-private.h"

#include "mulle-stacktrace.h"

#ifdef DEBUG
# define MULLE_STACKTRACE_DEBUG
#endif


uint32_t   mulle_stacktrace_get_version( void)
{
   return( MULLE__STACKTRACE_VERSION);
}



#if defined( _WIN32) && defined( MULLE_INCLUDE_DYNAMIC) && defined( MULLE_STACKTRACE_DEBUG) && ! defined( MULLE__CORE__ALL_LOAD_BUILD)
BOOL WINAPI   DllMain( HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved )
{
   MULLE_C_UNUSED( hinstDLL);
   MULLE_C_UNUSED( fdwReason);
   MULLE_C_UNUSED( lpvReserved);

   if( fdwReason == DLL_PROCESS_ATTACH)
      fprintf( stderr, "mulle-stacktrace DLL loaded\n");
   return TRUE;
}
#endif
