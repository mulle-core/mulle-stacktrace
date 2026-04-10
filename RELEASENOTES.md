## 0.5.0








* **BREAKING** Stacktrace is now always built for static linkage; symbols are no longer exported for dynamic/shared builds.
* Add explicit backend-selection macros `(MULLE_STRACKTRACE_BACKEND_{NONE,LIBBACKTRACE,EXECINFO})` and improved platform fallbacks; include a nop fallback and a Windows DllMain debug load message.
