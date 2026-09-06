### 0.5.2





* backend macro renamed to ``MULLE_STRACKTRACE_BACKEND`` so the execinfo backend is now actually enabled on Apple/BSD/Linux (previously ``MULLE_STRACKTRACE_STYLE`` was defined and ignored)
* new ``MULLE_STACKTRACE_NO_LIBBACKTRACE`` define to force-disable the libbacktrace backend at build time



* added missing license headers to internal include files
* corrected project name and copyright years in source file headers
* added API summary documentation link to README

### 0.5.1


feature: add Windows cross-compilation support

* add unwind library dependency for Windows platforms
* move libbacktrace include from public to private headers
* guard libbacktrace usage behind `_WIN32` check for non-Windows only
* rename export macro from `DEMO_GLOBAL/BUILD` to `MULLE__STACKTRACE_GLOBAL`
