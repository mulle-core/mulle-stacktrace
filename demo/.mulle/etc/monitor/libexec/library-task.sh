#! /usr/bin/env mulle-bash
# shellcheck shell=bash

#
# This function will be called by mulle-monitor
#
library_task_run()
{
   log_entry "library_task_run" "$@"

   local rc 

   #
   # we want craftinfos that are easily gotten by name
   #
   rexekutor mulle-sourcetree ${MULLE_TECHNICAL_FLAGS} -N rcopy --update .. 'craftinfo/*'
   rc=$?
   if [ $rc -eq 2 ]
   then
      rc=0
   fi

   #
   # we only want linkable stuff (avoids -startup and mulle-objc-list)
   # also we can get rid of stuff like mulle-color with no-demo
   #
   rexekutor mulle-sourcetree ${MULLE_TECHNICAL_FLAGS} -N rcopy --update .. '*' 'ENABLES link AND ENABLES demo AND ENABLES intermediate-link'
   rc=$?
   if [ $rc -eq 2 ]
   then
      rc=0
   fi
   return $rc
}

#
# Convenience to test your script standalone
#
if [ -z "${MULLE_MONITOR_TASK_LOAD}" ]
then
   library_task_run "$@"
fi
