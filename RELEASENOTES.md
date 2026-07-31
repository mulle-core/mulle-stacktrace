### 0.5.1


feature: add Windows cross-compilation support

* add unwind library dependency for Windows platforms
* move libbacktrace include from public to private headers
* guard libbacktrace usage behind `_WIN32` check for non-Windows only
* rename export macro from `DEMO_GLOBAL/BUILD` to `MULLE__STACKTRACE_GLOBAL`
