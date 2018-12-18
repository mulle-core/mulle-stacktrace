# mulle-stacktrace, 👣 Stracktrace support for various OS

The idea here is to have a unified library for stacktraces.
This currently just uses `<execinfo.h>`. It will need more
code to support windows.


## Install

Build and install with:

```
mkdir build 2> /dev/null
(
   cd build ;
   cmake .. ;
   make install
)
```

Or let [mulle-sde](//github.com/mulle-sde) do it all for you.


## Author

[Nat!](//www.mulle-kybernetik.com/weblog) for
[Mulle kybernetiK](//www.mulle-kybernetik.com) and
[Codeon GmbH](//www.codeon.de)
